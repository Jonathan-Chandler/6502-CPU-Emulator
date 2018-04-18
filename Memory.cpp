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
  memset(localMem, 0, sizeof(localMem));
  memset(localMem, 0xFF, 0x2000);
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
  fread(&localMem[0x8000], (16384*headerData.PrgRomPages), 1, infile);

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
  return localMem;
}
