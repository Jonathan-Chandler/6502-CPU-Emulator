#include "Ppu.hpp"
#include "SDL2/SDL.h"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

const colors Ppu::rgb[] = 
{
  {101, 101, 101},
  {0, 45, 105},
  {19, 31, 127},
  {60, 19, 124},
  {96, 11, 98},
  {115, 10, 55},
  {113, 15, 7},
  {90, 26, 0},
  {52, 40, 0},
  {11, 52, 0},
  {0, 60, 0},
  {0, 61, 16},
  {0, 56, 64},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
};

Ppu::Ppu()
: pWindow(NULL),
  pRenderer(NULL)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    throw "Couldn't initialize SDL";
  }

  if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &pWindow, &pRenderer) != 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize renderer/window: %s", SDL_GetError());
    throw "Couldn't initialize renderer/window";
  }
}

Ppu::~Ppu()
{
  deletePixels();

  if (pRenderer)
  {
    SDL_DestroyRenderer(pRenderer);
  }

  if (pWindow)
  {
    SDL_DestroyWindow(pWindow);
  }

  SDL_Quit();
}

void Ppu::SetData(uint8_t *data)
{
  chrData = data;
}

void Ppu::addPixels()
{
  int windowHeight;
  int windowWidth;
  SDL_GetWindowSize(pWindow, &windowWidth, &windowHeight);

  // field is 32 pixels x 32 pixels
  int rWidth = windowWidth/32;
  int rHeight = windowHeight/32;
  SDL_Rect *sprite;
  uint8_t rgbValues;

  for (int pixelRow = 0; pixelRow < 32; pixelRow++)
  {
    for (int pixelCol = 0; pixelCol < 32; pixelCol++)
    {
      sprite = new SDL_Rect();
      rgbValues = chrData[pixelRow + pixelCol];

      sprite->x = pixelCol * rWidth;
      sprite->y = pixelRow * rHeight;
      sprite->w = rWidth;
      sprite->h = rHeight;

      pRgb.push_back(rgbValues);
      pSprites.push_back(sprite);
    }
  }
}

void Ppu::deletePixels()
{
  SDL_Rect *sprite = NULL;

  while (!pSprites.empty())
  {
    sprite = pSprites.back();
    pSprites.pop_back();
    delete sprite;
  }
}

void Ppu::updatePixels()
{
  for (int pixelNum = 0; pixelNum < 1024; pixelNum++)
  {
    pRgb[pixelNum] = chrData[pixelNum];
  }
}

void Ppu::RenderAll()
{
  SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(pRenderer);

  for (size_t i = 0; i < 1024; i++)
  {
    SDL_SetRenderDrawColor(pRenderer, rgb[pRgb[i]][0], rgb[pRgb[i]][1], rgb[pRgb[i]][2], SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(pRenderer, pSprites[i]);
  }

  SDL_RenderPresent(pRenderer);
}


