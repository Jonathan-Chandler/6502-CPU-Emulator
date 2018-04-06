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

//void Ppu::RegisterEntity(Entity *newEntity)
//{
//  pRegisteredEntities.push_back(newEntity);
////  printf("recv entity\n");
////  if (!registeredEntity[0])
////    registeredEntity[0] = newEntity;
//}

void Ppu::SetData(uint8_t *data)
{
  chrData = data;
}

//void Ppu::AddSprites()
//{
//  SDL_Rect *sprite;
//  uint8_t *rgbValues;
//  int y = 0;
//
//  for (int spriteNum = 0; spriteNum < 1024; spriteNum+=16)
//  {
//    for (int x = 0; x < 16; x++)
//    {
//      sprite = new SDL_Rect;
//      rgbValues = new uint8_t[3];
//
//      rgbValues[0] =  chrData[x + y] & ~chrData[x + y + 8];   
//      rgbValues[1] = ~chrData[x + y] &  chrData[x + y + 8];   
//      rgbValues[2] =  chrData[x + y] &  chrData[x + y + 8];   
//
//      sprite->x = 8;
//      sprite->y = 1;
//      sprite->w = 1;
//      sprite->h = 1;
//
//      pRgb.push_back(rgbValues);
//      pSprites.push_back(sprite);
//    }
//  }
//}

void Ppu::addPixels()
{
  int windowHeight;
  int windowWidth;
  SDL_GetWindowSize(pWindow, &windowWidth, &windowHeight);
  int rWidth = windowWidth/256;
  int rHeight = windowHeight/240;
  SDL_Rect *sprite;
  uint8_t rgbValues;
  int y = 0;

  for (int pixelNum = 0; pixelNum < 32; pixelNum++)
  {
    sprite = new SDL_Rect;
    rgbValues = chrData[pixelNum];   

    sprite->x = pixelNum * rWidth;
    sprite->y = 0;
    sprite->w = rWidth;
    sprite->h = rHeight;

    pRgb.push_back(rgbValues);
    pSprites.push_back(sprite);
  }
}

void Ppu::updatePixels()
{
  for (int pixelNum = 0; pixelNum < 32; pixelNum++)
  {
    pRgb[pixelNum] = chrData[pixelNum];
  }
}
//  SDL_Rect *sprite;
//  uint8_t *rgbValues;
//
//  for (int x = 0; x < 4096; x+=16)
//  {
//    for (int y = 0; y < 8; y++)
//    {
//      sprite = new SDL_Rect;
//      rgbValues = new uint8_t[3];
//
//      rgbValues[0] =  chrData[x + y] & ~chrData[x + y + 8];   
//      rgbValues[1] = ~chrData[x + y] &  chrData[x + y + 8];   
//      rgbValues[2] =  chrData[x + y] &  chrData[x + y + 8];   
//      sprite->x = (20*(x/16))%1024;
//      sprite->y = 20*(x/1024) + 20*y;
//      sprite->w = 20;
//      sprite->h = 20;
//
//      pRgb.push_back(rgbValues);
//      pSprites.push_back(sprite);
//    }
//  }

void Ppu::RenderAll()
{
//  for (uint16_t x = 0; x < 10; x++)
//  {
//    printf("%x ", *(chrData + x));
//  }
//  printf("\n");
//}

  SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(pRenderer);
//  SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

  std::vector<SDL_Rect*> pSprites;
  for (size_t i = 0; i < 0x10; i++)
  {
    SDL_SetRenderDrawColor(pRenderer, rgb[pRgb[i]][0], rgb[pRgb[i]][1], rgb[pRgb[i]][2], SDL_ALPHA_OPAQUE);
//    SDL_RenderDrawRect(pRenderer, pSprites[i]);
    SDL_RenderFillRect(pRenderer, pSprites[i]);
//    printf("rgbValues[0] = %02x\n", pRgb[i][0]);
//    printf("rgbValues[1] = %02x\n", pRgb[i][1]);
//    printf("rgbValues[2] = %02x\n", pRgb[i][2]);
//    printf("pSprites->x = %02x\n", pSprites[i]->x);
//    printf("pSprites->y= %02x\n", pSprites[i]->y);
  }
//  for (i = 0; i < pSprites.size(); i++)
//  {
//    SDL_SetRenderDrawColor(pRenderer, pRgb[i][0], pRgb[i][1], pRgb[i][2], SDL_ALPHA_OPAQUE);
////    SDL_RenderDrawRect(pRenderer, pSprites[i]);
//    SDL_RenderFillRect(pRenderer, pSprites[i]);
////    printf("rgbValues[0] = %02x\n", pRgb[i][0]);
////    printf("rgbValues[1] = %02x\n", pRgb[i][1]);
////    printf("rgbValues[2] = %02x\n", pRgb[i][2]);
////    printf("pSprites->x = %02x\n", pSprites[i]->x);
////    printf("pSprites->y= %02x\n", pSprites[i]->y);
//  }

  SDL_RenderPresent(pRenderer);
}


