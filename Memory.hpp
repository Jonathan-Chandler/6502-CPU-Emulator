#ifndef MEMORY_HPP
#define MEMORY_HPP
#include "Cpu.hpp"
#include <iostream>


// shared space
#define PPUCTRL                         0x2000      // (VPHB SINN)
#define PPUCTRL_NMI                     (0x1 << 7)  // V NMI enable - Generate an NMI at the start of the vblank (0:off 1:on)
#define PPUCTRL_MASTER_SLAVE            (0x1 << 6)  // P PPU master/slave (0: read backdrop from EXT pins; 1: output color on EXT pins)
#define PPUCTRL_SPRITE_HEIGHT           (0x1 << 5)  // H sprite height (Sprite size (0: 8x8 pixels; 1: 8x16 pixels)
#define PPUCTRL_BACKGROUND_SELECT       (0x1 << 4)  // B background tile select - table address (0: $0000; 1: $1000)
#define PPUCTRL_SPRITE_TILE_SELECT      (0x1 << 3)  // S sprite tile select (0: $0000; 1: $1000; ignored in 8x16 mode)
#define PPUCTRL_INCREMENT_MODE          (0x1 << 2)  // I increment mode (0: add 1, going across; 1: add 32, going down)
#define PPUCTRL_NAMETABLE_SELECT        (0x3 << 0)  // NN nametable select (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)

#define PPUMASK                         0x2001      // (BGRs bMmG)
#define PPUMASK_COLOR_EMPHASIS_B        (0x1 << 7)  // B Color emphasis (BGR)
#define PPUMASK_COLOR_EMPHASIS_G        (0x1 << 6)  // G Color emphasis (BGR)
#define PPUMASK_COLOR_EMPHASIS_R        (0x1 << 5)  // R Color emphasis (BGR)
#define PPUMASK_SPRITE_ENABLE           (0x1 << 4)  // s show sprites
#define PPUMASK_BACKGROUND_ENABLE       (0x1 << 3)  // b show background
#define PPUMASK_LEFT_COLUMN_ENABLE      (0x1 << 2)  // M show sprites in leftmost 8 pixels of screen
#define PPUMASK_BG_LEFT_COLUMN_ENABLE   (0x1 << 1)  // m show background in leftmost 8 pixels of screen
#define PPUMASK_GREYSCALE               (0x1 << 0)  // G produce greyscale

#define PPUSTATUS                       0x2002      // read resets write pair for 0x2005/0x2006
#define PPUSTATUS_VBLANK                (0x1 << 7)  // vblank
#define PPUSTATUS_HIT                   (0x1 << 6)  // sprite 0 hit
#define PPUSTATUS_OVERFLOW              (0x1 << 5)  // sprite overflow

#define OAMADDR                         0x2003      // OAM read/write address

#define OAMDATA                         0x2004      // OAM data read/write

#define PPUSCROLL                       0x2005      // fine scroll position (two writes: X scroll, Y scroll) Add 256 to the X scroll position
#define PPUSCROLL_X                     (0x1 << 0)  // Add 256 to the x scroll position
#define PPUSCROLL_Y                     (0x1 << 1)  // Add 240 to the y scroll position

#define PPUADDR                         0x2006      // (memory mapped) PPU read/write address (two writes: most significant byte, least significant byte)

#define PPUDATA                         0x2007      // (memory mapped) PPU data read/write

#define OAMDMA                          0x4014      // OAM DMA high address

class Memory
{
  public:
    Memory();

    void loadRom(char *fileName);

    void printHeader();
    void printTrainer();
    void printRom();
    void printChr();
    void printInstRom();
    void printProm();

    uint8_t getPrgSize();
    uint8_t *getChrRomData();
    uint8_t *getMemory();

    // CPU Memory
    // ======
    // 0x100   => Zero Page (0x0 -> 0xFF)
    // 0x200   => Stack     (0x100 -> 0x1FF)
    // 0x800   => RAM       (0x200 -> )
    // 0x2000  => Mirrors (0-0x7FF)
    // 0x2008  => I/O Registers
    // 0x4000  => Mirrors (0x2000-0x2007)
    // 0x4020  => I/O Registers
    // 0x6000  => Expansion ROM
    // 0x8000  => SRAM
    // 0xC000  => PRG-ROM (Lower Bank)
    // 0x10000 => PRG-ROM (Upper Bank)
    // 
    // CPU Memory
    // ======
    // $0000-$07FF  $0800   2KB internal RAM
    // $0800-$0FFF  $0800   Mirrors of $0000-$07FF
    // $1000-$17FF  $0800
    // $1800-$1FFF  $0800
    // $2000-$2007  $0008   NES PPU registers
    // $2008-$3FFF  $1FF8   Mirrors of $2000-2007 (repeats every 8 bytes)
    // $4000-$4017  $0018   NES APU and I/O registers
    // $4018-$401F  $0008   APU and I/O functionality that is normally disabled. See CPU Test Mode.
    // $4020-$FFFF  $BFE0   Cartridge space: PRG ROM, PRG RAM, and mapper registers (See Note)
    //
    // CPU Memory
    // ======
    // $0000-$000F  16 bytes    Local variables and function arguments
    // $0010-$00FF  240 bytes   Global variables accessed most often, including certain pointer tables
    // $0100-$019F  160 bytes   Data to be copied to nametable during next vertical blank (see The frame and NMIs)
    // $01A0-$01FF  96 bytes    Stack
    // $0200-$02FF  256 bytes   Data to be copied to OAM during next vertical blank
    // $0300-$03FF  256 bytes   Variables used by sound player, and possibly other variables
    // $0400-$07FF  1024 bytes  Arrays and less-often-accessed global variables

    // Addressing modes
    uint8_t *AddressNone(uint8_t *instructionAddr);                   // No addressing type is used for function
    uint8_t *AddressImmediate(uint8_t *instructionAddr);              // Return the address of the value following the instruction
    
    uint8_t *AddressDirectZeroX(uint8_t *instructionAddr);            // Return the address: &memory[VAL + X]; If VAL + X overflows, wrapped to the zero page only (VAL is 1 byte)
    uint8_t *AddressDirectZeroY(uint8_t *instructionAddr);            // Return the address: &memory[VAL + Y]; If VAL + Y overflows, wrapped to the zero page only (VAL is 1 byte)
    uint8_t *AddressDirectZeroZ(uint8_t *instructionAddr);            // Return the address: &memory[VAL]; Overflow not possible (VAL is 1 byte)
    
    uint8_t *AddressDirectAbsX(uint8_t *instructionAddr);             // Return the address: &memory[VAL + X]; (VAL is 2 bytes)
    uint8_t *AddressDirectAbsY(uint8_t *instructionAddr);             // Return the address: &memory[VAL + X]; (VAL is 2 bytes)
    uint8_t *AddressDirectAbsZ(uint8_t *instructionAddr);             // Return the address: &memory[VAL]; (VAL is 2 bytes)
    
    uint8_t *AddressIndirectZeroX(uint8_t *instructionAddr);          // Return the address: &memory[*(&memory[VAL + X])]; If VAL + X overflows, wrapped to zero page (VAL is 1 byte)
    uint8_t *AddressIndirectZeroZ(uint8_t *instructionAddr);          // Return the address: &memory[*(&memory[VAL + X])]; If VAL + X overflows, wrapped to zero page (VAL is 1 byte)

    uint8_t *AddressIndirectZeroIndexY(uint8_t *instructionAddr);     // Return the address: &memory[*(&memory[VAL]) + Y]; (VAL is 1 byte)
      
    uint8_t *AddressIndirectAbsX(uint8_t *instructionAddr);           // Return the address: &memory[*(&memory[VAL + X]); (VAL is 2 bytes)
    uint8_t *AddressIndirectAbsZ(uint8_t *instructionAddr);           // Return the address: &memory[*(&memory[VAL]); (VAL is 2 bytes)
    uint8_t *AddressRelative(uint8_t *instructionAddr);               // Return the address: &memory[PC + signed(VAL)]; (VAL is 1 byte)
    uint8_t *AddressRegisterA(uint8_t *instructionAddr);              // Return the address of CPU register A

    bool testPageBoundary(uint8_t addressOffset);

  private:
    class Cpu *cpu_callback;
    uint8_t header[16];
    uint8_t *PrgRomData;
    uint8_t *ChrRomData;
    uint8_t cpu_mem[0x10000];
    uint8_t ppu_mem[0x10000];
};
#endif
