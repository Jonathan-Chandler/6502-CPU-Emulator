#ifndef PPU_HPP
#define PPU_HPP
#include "SDL2/SDL.h" 
#include <iostream>
#include <vector>

typedef uint8_t data[16];
typedef uint8_t colors[3];
typedef uint8_t sprite[8][8];
typedef uint8_t sprite2[4][4];

// streamA = data[0];
// streamB = data[8];
// 
// streamA & streamB = blue;
// streamA & ~streamB = red;
// ~streamA & streamB = green;
// ~streamA & ~streamB = transparent;

// Memory (section 3)
// 
// Memory display. Can also be used to modify memory.
// 
//     3F00-3F1F — Palettes. Some of the cells are mirrors.
//     S000-S11F — OAM. For example, S000 holds the y position for sprite 0.
//         S000-S0FF — Primary OAM.
//         S100-S11F — Secondary OAM (normally not directly accessible).
//     0000-03FF — Pattern tables. This 1KB segment is mirrored eight times to fill out the entire CHR space.
//     2000-23FF — Nametables. The simulation uses a kind of "one-screen low" mirroring, and the data here is mirrored to fill out the entire nametable space.
//         This layout for pattern and name tables happens to be the same as the PPU A13 variant of iNES Mapper 218.


// Object Attribute Memory can be viewed as an array with 64 entries
typedef struct 
{
  uint8_t y;
  uint8_t tile_number;
  // For 8x8 sprites, this is the tile number of this sprite within the pattern table selected in bit 3 of PPUCTRL ($2000).
  // For 8x16 sprites, the PPU ignores the pattern table selection and selects a pattern table from bit 0 of this number.
  uint8_t attribute;
#define OAM_PALETTE           (0x3 << 0)    // Palette (4 to 7) of sprite
#define OAM_PRIORITY          (0x1 << 5)    // Priority (0: in front of background; 1: behind background)
#define OAM_FLIP_HORIZONTALLY (0x1 << 6)    // Flip sprite horizontally
#define OAM_FLIP_VERTICALLY   (0x1 << 7)    // Flip sprite vertically
  uint8_t x;
} oam_t;

typedef struct
{
  uint8_t data;
} tile_t;

#define TABLE_TILE_ROW_COUNT 30
#define TABLE_TILE_COL_COUNT 32
#define TABLE_ATTRIBUTE_ROW_COUNT 8
#define TABLE_ATTRIBUTE_COL_COUNT 8

typedef struct
{
  uint8_t tile[TABLE_TILE_ROW_COUNT][TABLE_TILE_COL_COUNT];                       // 960 bytes
  uint8_t attribute_table[TABLE_ATTRIBUTE_ROW_COUNT][TABLE_ATTRIBUTE_ROW_COUNT];  // 64 bytes
#define TOP_LEFT_QUADRANT_MASK      (0x3 << 0)
#define TOP_RIGHT_QUADRANT_MASK     (0x3 << 2)
#define BOTTOM_LEFT_QUADRANT_MASK   (0x3 << 4)
#define BOTTOM_RIGHT_QUADRANT_MASK  (0x3 << 6)
} name_table_t;

#define TOP_LEFT_NAME_TABLE       1     // 0x2000
#define TOP_RIGHT_NAME_TABLE      2     // 0x2400
#define BOTTOM_LEFT_NAME_TABLE    3     // 0x2800
#define BOTTOM_RIGHT_NAME_TABLE   4     // 0x2C00
//       (0,0)     (256,0)     (511,0)
//         +-----------+-----------+
//         |           |           |
//         |           |           |
//         |   $2000   |   $2400   |
//         |           |           |
//         |           |           |
//  (0,240)+-----------+-----------+(511,240)
//         |           |           |
//         |           |           |
//         |   $2800   |   $2C00   |
//         |           |           |
//         |           |           |
//         +-----------+-----------+
//       (0,479)   (256,479)   (511,479)

// memory map
// 0x0000 - 0x3FFF - addressable area
// (2k ram mapped to name table)

// CHR-ROM
// Pattern table address format: 000H RRRR CCCC PTTT
#define PATTERN_TABLE_0       0x0000      // 0x1000 bytes - Left pattern table
#define PATTERN_TABLE_1       0x1000      // 0x1000 bytes - Right pattern table
#define PATTERN_FINE_Y_OFFSET (0x3 << 0)  // row number within tile
#define PATTERN_BIT_PLANE     (0x1 << 3)  // lower/upper = 0/1
#define PATTERN_TILE_COLUMN   (0xF << 4)  // tile column
#define PATTERN_TILE_ROW      (0xF << 8)  // tile row
#define PATTERN_TABLE_HALF    (0x1 << 12) // pattern table half

// VRAM (mapped to 2kb internal VRAM)
#define NAME_TABLE_0          0x2000    // 0x400 bytes
#define NAME_TABLE_1          0x2400    // 0x400 bytes
#define NAME_TABLE_2          0x2800    // 0x400 bytes
#define NAME_TABLE_3          0x2C00    // 0x400 bytes

#define MIRROR_1_BEGIN        0x3000    // 0x400 bytes
#define MIRROR_1_END          0x3EFF    // 

#define PALETTE_RAM_INDEXES   0x3F00    // 0x20 bytes
// $3F00        Universal background color
// $3F01-$3F03 	Background palette 0
// $3F05-$3F07 	Background palette 1
// $3F09-$3F0B 	Background palette 2
// $3F0D-$3F0F 	Background palette 3
// $3F11-$3F13 	Sprite palette 0
// $3F15-$3F17 	Sprite palette 1
// $3F19-$3F1B 	Sprite palette 2
// $3F1D-$3F1F 	Sprite palette 3

#define MIRROR_2_BEGIN        0x3F20    // 0xE0 bytes - mirrors palette indexes 3F00 - 3F1F
#define MIRROR_2_END          0x3FFF

class Ppu
{
  public:
    Ppu();
    ~Ppu();
    void RenderAll();
    void SetData(uint8_t *data);
    void AddSprites();
    void addPixels();
    void updatePixels();
    void deletePixels();
    void clear_screen();
      
  private:
    static const colors rgb[];
    uint8_t *chrData;
    uint8_t pixels[8][8];
    size_t pixelCount;
    uint8_t sprite[8];
    std::vector<SDL_Rect*> pSprites;
    std::vector<uint8_t> pRgb;

    // SDL window elements
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    name_table_t name_tables[4];
    oam_t oam_entries[64];

};
#endif
