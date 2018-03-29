#include "Rom.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <fstream>
#include <iostream>
#include <string>

//#define iNesRomType

#define INES_ROM_T = {'N','E','S',1A};    // ines always equals 'N', 'E', 'S', 1A
//using namespace std;

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

Rom::Rom()
{
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

void Rom::load()
{
//  std::ifstream infile;
//  infile.open("./New folder/cpu_dummy_writes_oam.nes", std::ios::in);
  FILE *infile;
  infile = fopen("./New folder/cpu_dummy_writes_oam.nes", "rb");
  uint8_t data[0x10] = {0};
//  size_t readSize = 0;
  fread(data, sizeof(data), 1, infile);

  for (int i = 0; i < 0x10; i++)
  {
    uint8_t c = data[i];
//    infile.get(c);
    printf("%x ", c & 0xFF);
  }
  std::cout << std::endl;
  fclose(infile);

  uint8_t PrgRomSize = data[4];
  uint8_t ChrRomSize = data[5];
//  uint8_t flags6 = data[6];
//  uint8_t flags7 = data[7];

  using namespace std;
  std::cout << "prgromsize: " << PrgRomSize*16 << "kb" << endl;
  std::cout << "chrromsize: " << ChrRomSize*8 << "kb" << endl;

}

void Rom::load(char *file)
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
  printf("read %lx bytes\n", sizeof(headerData)*result);
  printf("RomType: %x\n", (uint32_t)headerData.RomType);
  printf("PrgRomPages: %x\n", (uint8_t)headerData.PrgRomPages);
  printf("ChrRomPages: %x\n", (uint8_t)headerData.ChrRomPages);
  printf("FlagByte6: %x\n", (uint8_t)headerData.FlagsByte6);
  printf("FlagByte7: %x\n", (uint8_t)headerData.FlagsByte7);
  printf("RamPages: %x\n", (uint8_t)headerData.RamPages);

  std::cout << "RomType:" << std::hex << (uint32_t)headerData.RomType << std::endl;
  std::cout << "PrgRomPages:" << std::hex << (uint8_t)headerData.PrgRomPages << std::endl;
  std::cout << "ChrRomPages:" << std::hex << (uint8_t)headerData.ChrRomPages << std::endl;
  std::cout << "FlagByte6:" << std::hex << (uint8_t)headerData.FlagsByte6 << std::endl;
  std::cout << "FlagByte7:" << std::hex << (uint8_t)headerData.FlagsByte7 << std::endl;
  std::cout << "RamPages:" << std::hex << (uint8_t)headerData.RamPages << std::endl;

  if ((headerData.FlagsByte6 & TrainerPresent) == 1)
  {
    std::cout << "trainer present" << std::endl;
    fread(trainerData, sizeof(trainerData), 1, infile);
  }

  if (headerData.PrgRomPages == 0)
    headerData.PrgRomPages = 1;
  PrgRomData = (uint8_t *) malloc(1024*16*headerData.PrgRomPages);
  fread(PrgRomData, (1024*16*headerData.PrgRomPages), 1, infile);

  if (headerData.ChrRomPages == 0)
    headerData.ChrRomPages = 1;
  ChrRomData = (uint8_t *) malloc(1024*8*headerData.ChrRomPages);
  result = fread(ChrRomData, (1024*8*headerData.ChrRomPages), 1, infile);
  std::cout << "read:" << result << std::endl;

  std::cout << "prgromsize: " << std::dec << headerData.PrgRomPages*16 << "kb" << std::endl;
  std::cout << "chrromsize: " << std::dec << headerData.ChrRomPages*8 << "kb" << std::endl;

//  for (int i = 0; i < 8*1024*headerData.ChrRomPages; i += 16)
//  for (int i = 0; i < 16*1024*headerData.PrgRomPages; i += 16)
  for (int i = 0; i < 1024; i += 16)
  {
    for (int x = 0; x < 16; x++)
    {
//      printf("%02x ", PrgRomData[i+x]);
      printf("%02x ", ChrRomData[i+x]);
    }
    printf("\n");
  }
//  uint16_t PlayChoiceInstRomSize = (PlayChoice & data[7]) ? 8*1024 : 0;
//  uint16_t PlayChoicePromSize = (PlayChoice & data[7]) ? 8*1024 : 0;

  fclose(infile);
}

uint8_t *Rom::getChrRomData()
{
  return ChrRomData;
}

