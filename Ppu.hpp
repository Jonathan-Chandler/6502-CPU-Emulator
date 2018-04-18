#ifndef PPU_HPP
#define PPU_HPP
#include "SDL2/SDL.h" 
#include <iostream>
#include <vector>

// TODO: emulate NES PPU
// uint8_t streamA;
// uint8_t streamB;

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
};
#endif
