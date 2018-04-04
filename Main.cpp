#include <iostream>
#include "Ppu.hpp"
#include "Cpu.h"
#include "Rom.h"
#include <chrono>
#include "CpuTest.tcc"

int main()
{
  Cpu nesCpu;
  CpuTest<Cpu> testCpu(nesCpu);
//  testCpu.startTest();
  testCpu.testADC();
  testCpu.testSBC();

//  char fileName[] = "./New folder/tutor.nes";
//  char fileName[] = "./New folder/Legend of Zelda, The (USA).nes";
//  char fileName[] = "./New folder/Double Dragon 2 - The Revenge (U).nes";
//  Ppu nesPpu;
//  Rom nesRom;
  //nesRom.load();
//  nesRom.load(fileName);
//  nesPpu.SetData(nesRom.getChrRomData());
//  nesPpu.AddSprites();
//  nesPpu.RenderAll();

  /*
  size_t frameCount = 0;
  typedef std::chrono::high_resolution_clock Time;
  using std::chrono::microseconds;
  using std::chrono::duration_cast;
  auto currentTime = Time::now();
  bool mRequestExit = SDL_FALSE;

  microseconds t(0);
  microseconds dT(16667); // equals 1 second/60 frames -> microseconds
  microseconds accumulator(0);
  microseconds netAccumulator(0);
  microseconds frameTimer(0);

  while (mRequestExit != SDL_TRUE) 
  {
    SDL_Event event;
    auto newTime = Time::now();
    microseconds frameTime = duration_cast<microseconds>(newTime - currentTime);
    frameCount += 1;

    if (frameTimer < dT)
    {
      frameTimer += frameTime;
    }
    else
    {
      nesPpu.RenderAll();
      frameTimer -= dT;
      frameCount = 0;
    }

    currentTime = newTime;

    accumulator += frameTime;
    netAccumulator += frameTime;

    while (SDL_PollEvent(&event)) 
    {
      if (event.type == SDL_KEYDOWN
          || event.type == SDL_KEYUP) 
      {
        //PlayerAction Action;
      }

      if (event.type == SDL_QUIT) 
      {
        mRequestExit = SDL_TRUE;
      }
    }

    while (accumulator >= dT)
    {
      t += dT;
      accumulator -= dT;
    }
  }
  */
}
