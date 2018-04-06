#include <iostream>
#include <string>
#include <chrono>
#include "Ppu.hpp"
#include "Cpu.hpp"
#include "Memory.hpp"
#include "CpuTest.tcc"
#include <string.h>

int main()
{
  uint8_t program[] = {0xa2, 0x00, 0xa0, 0x00, 0x8a, 0x99, 0x00, 0x02, 0x48, 0xe8, 0xc8, 0xc0, 0x10, 0xd0, 0xf5, 0x68, 0x99, 0x00, 0x02, 0xc8, 0xc0, 0x20, 0xd0, 0xf7};
  uint8_t *mem;
  mem = (uint8_t *) malloc(0x10000);
  memcpy(&mem[0x600], program, sizeof(program));

//  char file[] = "cpu_dummy_writes_oam.nes";
  Ppu nesPpu;
  Cpu nesCpu;
  nesCpu.setMemory(mem);
  nesCpu.setPc(0x0600);
  uint8_t *chrData = &mem[0x0200];
  nesPpu.SetData(chrData);
  nesPpu.addPixels();
////  nesCpu.loadRom(file);
  std::string c = "";
  while (c != "x")
  {
    nesCpu.printStatus();
    c = getchar();
    nesCpu.doInstruction();
    nesCpu.printZeroPage();
    nesCpu.printStack();

    printf("\n");
    printf("Chr Page:\n");

    for (uint16_t x = 0; x < 0x20; x+=0x10)
    {
      printf("%2x: ", x);
      for (uint16_t y = 0; y < 0x10; y++)
      {
        printf("%2x ", (uint8_t)*(chrData + x + y) & 0xFF);
      }
      printf("\n");
    }

    printf("\n");
    nesPpu.updatePixels();
    nesPpu.RenderAll();
//    nesPpu.RenderAll();
  }
//  CpuTest<Cpu> testCpu(nesCpu);
//  testCpu.testADC();
//  testCpu.testSBC();
//  testCpu.startTest();

//  char file[] = "cpu_dummy_writes_oam.nes";
//  Memory nesMem;
//  nesMem.loadRom(file);

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
