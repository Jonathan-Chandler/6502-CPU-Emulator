#include "Cpu.hpp"
#include "Memory.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <fstream>
#include <iostream>
#include <string>

#define INES_ROM_HEADER = {'N','E','S',1A};    // ines always equals 'N', 'E', 'S', 1A

struct InesHeader_T
{
  uint32_t  RomType;                      // ines always equals 'N', 'E', 'S', 1A
	uint8_t   PrgRomPages;                  // PRG-ROM pages (16KB each, minimum 1 page) - PrgRomPages * 16384
	uint8_t   ChrRomPages;                  // CHR-ROM pages (8KB each, minimum 1 page) - ChrBytePages * 8192

	uint8_t   FlagsByte6;                   // Flags at byte 6
#define MirroringVertical         0 << 0  //  0: horizontal (vertical arrangement) (CIRAM A10 = PPU A11)
#define MirroringHorizontal       1 << 0  //  1: vertical (horizontal arrangement) (CIRAM A10 = PPU A10)
#define BatteryBacked             1 << 1  //  Cartridge contains battery-backed PRG RAM ($6000-7FFF) or other persistent memory
#define TrainerPresent            1 << 2  //  512-byte trainer at $7000-$71FF (stored before PRG data)
#define IgnoreMirroringControl    1 << 3  //  Ignore mirroring control or above mirroring bit; instead provide four-screen VRAM
#define LowerMapperNybble       0xF << 4  //  Lower nybble of mapper number

	uint8_t   FlagsByte7;                   // flags at byte 7
#define VsUnisystem               1 << 0  // 
#define PlayChoice                1 << 1  //  PlayChoice-10 (8KB of Hint Screen data stored after CHR data)
#define NesFormat                 3 << 2  //  If equal to 2, flags 8-15 are in NES 2.0 format
#define NesFormat2                2 << 2  //  If equal to 2, flags 8-15 are in NES 2.0 format
#define UpperMapperNybble       0xF << 4  //  Upper nybble of mapper number

	uint8_t   RamPages;                     // RAM pages (8KB each)
	uint8_t   Unused[7];                    // unused padding
};

Memory::Memory()
{
  memset(cpu_mem, 0, sizeof(cpu_mem));
  memset(cpu_mem, 0xFF, 0x2000);
}

// byte 8 holds size of PRG RAM in 8 KB units (Value 0 infers 8 KB)

// flags at byte 9 - not used
#define TvSystem                  1 << 0 //
#define TvSystemNtsc              0 << 0 //
#define TvSystemPal               1 << 0 //

// flags at byte 10 - unofficial
// flags at bytes 11-15 - zero

//  https://wiki.nesdev.com/w/index.php/INES
//  If byte 7 AND $0C = $08, and the size taking into account byte 9 does not exceed the actual size of the ROM image, then NES 2.0.
//  If byte 7 AND $0C = $00, and bytes 12-15 are all 0, then iNES.
//  Otherwise, archaic iNES.

void Memory::loadRom(char *file)
{
  FILE *infile;
  infile = fopen(file, "rb");

  if (!infile)
  {
    std::cout << "fail to open file" << std::endl;
    return;
  }
  else
  {
    std::cout << "opened file: " << file << std::endl;
  }

  InesHeader_T headerData;
  uint8_t trainerData[512];

  size_t result = fread(&headerData, sizeof(headerData), 1, infile);
  if (result != 1)
  {
    std::cout << "fail to read header" << std::endl;
    return;
  }

  if ((headerData.FlagsByte6 & TrainerPresent) == 1)
  {
    std::cout << "trainer present" << std::endl;
    fread(trainerData, sizeof(trainerData), 1, infile);
  }

  if (headerData.PrgRomPages == 0)
    headerData.PrgRomPages = 1;
  PrgRomData = (uint8_t *) malloc(16384*headerData.PrgRomPages);
  fread(&cpu_mem[0x8000], (16384*headerData.PrgRomPages), 1, infile);

  if (headerData.ChrRomPages == 0)
  {
    headerData.ChrRomPages = 1;
  }
  ChrRomData = (uint8_t *) malloc(1024*8*headerData.ChrRomPages);
  result = fread(ChrRomData, (1024*8*headerData.ChrRomPages), 1, infile);

  fclose(infile);
}

uint8_t *Memory::getChrRomData()
{
  return ChrRomData;
}

uint8_t *Memory::getMemory()
{
  return cpu_mem;
}

// No address/value
uint8_t *Memory::AddressNone(uint8_t *instructionAddr)
{
  return nullptr;
}

// immediate value: #$
uint8_t *Memory::AddressImmediate(uint8_t *instructionAddr)
{
  uint8_t *value = nullptr;

  // value is byte following instructionAddr
  value = instructionAddr;

  return value;
}

// direct zero addr + X: d, X
uint8_t *Memory::AddressDirectZeroX(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = 0;
  uint8_t x = cpu_callback->getX();

  // zero page address is in byte following instruction, add X
  zeroPageAddr = *(instructionAddr) + x;

  tempAddr = cpu_mem + zeroPageAddr;

  return tempAddr;
}

// direct zero addr + Y: d, Y
uint8_t *Memory::AddressDirectZeroY(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = 0;
  uint8_t y = cpu_callback->getY();

  // zero page address is in byte following instruction, add y
  zeroPageAddr = *(instructionAddr) + y;

  tempAddr = cpu_mem + zeroPageAddr;

  return tempAddr;
}

// direct zero addr: d
uint8_t *Memory::AddressDirectZeroZ(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = *instructionAddr;

  // low byte of zero address follows instruction byte
  // use the low byte as index into memory to get the address of the zero page
  tempAddr = cpu_mem + zeroPageAddr;

  return tempAddr;
}

// absolute address + X: a, X
uint8_t *Memory::AddressDirectAbsX(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t absoluteAddr = 0;
  uint8_t x = cpu_callback->getX();

  // absolute address is in 2 byte following instruction in reverse byte order
  absoluteAddr = *(instructionAddr+1) & 0xFF;
  absoluteAddr <<= 8;
  absoluteAddr |= *instructionAddr & 0xFF;
  absoluteAddr += x;

  // memory address of given absolute address + y register
  tempAddr = cpu_mem + absoluteAddr;

//  printf("Address mode AbsoluteAddr, Y -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// absolute address + Y: a, Y
uint8_t *Memory::AddressDirectAbsY(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t absoluteAddr = 0;
  uint8_t y = cpu_callback->getY();

  // absolute address is in 2 byte following instruction in reverse byte order
  absoluteAddr = *(instructionAddr+1) & 0xFF;
  absoluteAddr <<= 8;
  absoluteAddr |= *(instructionAddr) & 0xFF;
  absoluteAddr += y;

  // memory address of given absolute address + y register
  tempAddr = cpu_mem + absoluteAddr;

  //printf("Address mode AbsoluteAddr, Y -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// absolute address: a
uint8_t *Memory::AddressDirectAbsZ(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t absoluteAddr = 0;

  // address follows instruction byte, byte order is reversed
  absoluteAddr = *(instructionAddr+1) & 0xFF;
  absoluteAddr <<= 8;
  absoluteAddr |= *(instructionAddr) & 0xFF;

  // use this as index into memory to get the absolute address
  tempAddr = cpu_mem + absoluteAddr;

  return tempAddr;
}

// indirect zero page address + X: (d, X)
uint8_t *Memory::AddressIndirectZeroX(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = *instructionAddr;
  uint16_t absoluteAddr = 0;
  uint8_t x = cpu_callback->getX();

  // zero page address incremented by x before dereference
  zeroPageAddr += x;

  // get the address given at this location on the zero page, in reverse byte order
  absoluteAddr = *(cpu_mem + (zeroPageAddr + 1)) & 0xFF;
  absoluteAddr <<= 8;
  absoluteAddr |= *(cpu_mem + zeroPageAddr) & 0xFF;

  // return address of the dereferenced address
  tempAddr = cpu_mem + absoluteAddr;
  
  return tempAddr;
}

// indirect zero page address: (d)
uint8_t *Memory::AddressIndirectZeroZ(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = *instructionAddr;
  uint16_t absoluteAddr = 0;

  // get the address given at this location on the zero page, in reverse byte order
  absoluteAddr = *(cpu_mem + (zeroPageAddr + 1)) & 0xFF;
  absoluteAddr <<= 8;
  absoluteAddr |= *(cpu_mem + zeroPageAddr) & 0xFF;

  // return address of the dereferenced address
  tempAddr = cpu_mem + absoluteAddr;
  
  return tempAddr;
}

// indirect zero page address[Y]: (d), Y
uint8_t *Memory::AddressIndirectZeroIndexY(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = *instructionAddr;
  uint16_t absoluteAddr = 0;
  uint8_t y = cpu_callback->getY();
  
  // dereference value at given zero page address, read 2 bytes in reverse byte order
  absoluteAddr = *(cpu_mem + (zeroPageAddr + 1)) & 0xFF;
  absoluteAddr <<= 8;
  absoluteAddr |= *(cpu_mem + zeroPageAddr) & 0xFF;

  // add index
  absoluteAddr += y;

  tempAddr = cpu_mem + absoluteAddr;

  return tempAddr;
}
  
// indirect absolute address + x: (a, X)
uint8_t *Memory::AddressIndirectAbsX(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t directAddr = 0;
  uint16_t absoluteAddr = 0;
  uint8_t x = cpu_callback->getX();

  // add x to direct address
  directAddr = *(instructionAddr+1) & 0xFF;
  directAddr <<= 8;
  directAddr |= *(instructionAddr) & 0xFF;
  directAddr += x;

  // get absolute address by dereferencing directAddr
  absoluteAddr = *(cpu_mem + (directAddr + 1)) & 0xFF;
  absoluteAddr <<= 8;
  absoluteAddr |= *(cpu_mem + directAddr) & 0xFF;

  // memory address of given absolute address + x register
  tempAddr = cpu_mem + absoluteAddr;

  return tempAddr;
}

// indirect absolute address: (a)
uint8_t *Memory::AddressIndirectAbsZ(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t directAddr = 0;
  uint16_t absoluteAddr = 0;

  // get direct address
  directAddr = *(instructionAddr+1) & 0xFF;
  directAddr <<= 8;
  directAddr |= *(instructionAddr) & 0xFF;

  // dereference direct address to get absolute address
  absoluteAddr = *(cpu_mem + (directAddr + 1)) & 0xFF;
  absoluteAddr <<= 8;
  absoluteAddr |= *(cpu_mem + directAddr) & 0xFF;

  // memory address of given absolute address + x register
  tempAddr = cpu_mem + absoluteAddr;

  return tempAddr;
}

// relative address: (a)
// Return the address: &memory[PC + signed(VAL)]; (VAL is 1 byte)
uint8_t *Memory::AddressRelative(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t absoluteAddr;
  uint16_t offset;
  uint16_t pc = cpu_callback->getProgramCounter();

  // relative address is 1 byte following instruction
  offset = (*instructionAddr) & 0xFF;

  // extend signed 8-bit representation to signed 16-bit
  if (offset >= 0x80)
  {
    offset |= 0xFF00;
  }

  // get PC and add the signed offset, add opcode and address size here
  absoluteAddr = pc + offset + 2;

  // memory address of current PC address + signed(VAL)
  tempAddr = cpu_mem + absoluteAddr;

  return tempAddr;
}

// Return the address of CPU register A
uint8_t *Memory::AddressRegisterA(uint8_t *instructionAddr)
{
  return cpu_callback->getAddressA();
}

bool Memory::testPageBoundary(uint8_t addressOffset)
{
  uint16_t currentPc = cpu_callback->getProgramCounter();
  uint16_t branchAddress = currentPc + addressOffset;

  return (currentPc/100 != branchAddress/100);
}

