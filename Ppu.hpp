#ifndef PPU_HPP
#define PPU_HPP
#include "SDL2/SDL.h" 
#include <iostream>
#include <vector>

typedef uint8_t data[16];
typedef uint8_t colors[3];
typedef uint8_t sprite[8][8];
typedef uint8_t sprite2[4][4];

// uint8_t streamA;
// uint8_t streamB;

// streamA = data[0];
// streamB = data[8];
// 
// streamA & streamB = blue;
// streamA & ~streamB = red;
// ~streamA & streamB = green;
// ~streamA & ~streamB = transparent;


class Ppu
{
  public:
    Ppu();
    ~Ppu();
    void RenderAll();
    void SetData(uint8_t *data);
    void AddSprites();
      
  private:
    static const colors rgb[];
    uint8_t *chrData;
    uint8_t pixels[8][8];
    size_t pixelCount;
    uint8_t sprite[8];
    std::vector<SDL_Rect*> pSprites;
    std::vector<uint8_t*> pRgb;

    // SDL window elements
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
    //std::vector<Entity*> pRegisteredEntities;
};
#endif
