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
//  uint8_t program[] = {0xa2, 0x00, 0xa0, 0x00, 0x8a, 0x99, 0x00, 0x02, 0x48, 0xe8, 0xc8, 0xc0, 0x10, 0xd0, 0xf5, 0x68, 0x99, 0x00, 0x02, 0xc8, 0xc0, 0x20, 0xd0, 0xf7};
  uint8_t program[] = 
  {
    0x20, 0x06, 0x06, 0x20, 0x38, 0x06, 0x20, 0x0d, 0x06, 0x20, 0x2a, 0x06, 0x60, 0xa9, 0x02, 0x85, 
    0x02, 0xa9, 0x04, 0x85, 0x03, 0xa9, 0x11, 0x85, 0x10, 0xa9, 0x10, 0x85, 0x12, 0xa9, 0x0f, 0x85, 
    0x14, 0xa9, 0x04, 0x85, 0x11, 0x85, 0x13, 0x85, 0x15, 0x60, 0xa5, 0xfe, 0x85, 0x00, 0xa5, 0xfe, 
    0x29, 0x03, 0x18, 0x69, 0x02, 0x85, 0x01, 0x60, 0x20, 0x4d, 0x06, 0x20, 0x8d, 0x06, 0x20, 0xc3, 
    0x06, 0x20, 0x19, 0x07, 0x20, 0x20, 0x07, 0x20, 0x2d, 0x07, 0x4c, 0x38, 0x06, 0xa5, 0xff, 0xc9, 
    0x77, 0xf0, 0x0d, 0xc9, 0x64, 0xf0, 0x14, 0xc9, 0x73, 0xf0, 0x1b, 0xc9, 0x61, 0xf0, 0x22, 0x60, 
    0xa9, 0x04, 0x24, 0x02, 0xd0, 0x26, 0xa9, 0x01, 0x85, 0x02, 0x60, 0xa9, 0x08, 0x24, 0x02, 0xd0, 
    0x1b, 0xa9, 0x02, 0x85, 0x02, 0x60, 0xa9, 0x01, 0x24, 0x02, 0xd0, 0x10, 0xa9, 0x04, 0x85, 0x02, 
    0x60, 0xa9, 0x02, 0x24, 0x02, 0xd0, 0x05, 0xa9, 0x08, 0x85, 0x02, 0x60, 0x60, 0x20, 0x94, 0x06, 
    0x20, 0xa8, 0x06, 0x60, 0xa5, 0x00, 0xc5, 0x10, 0xd0, 0x0d, 0xa5, 0x01, 0xc5, 0x11, 0xd0, 0x07, 
    0xe6, 0x03, 0xe6, 0x03, 0x20, 0x2a, 0x06, 0x60, 0xa2, 0x02, 0xb5, 0x10, 0xc5, 0x10, 0xd0, 0x06, 
    0xb5, 0x11, 0xc5, 0x11, 0xf0, 0x09, 0xe8, 0xe8, 0xe4, 0x03, 0xf0, 0x06, 0x4c, 0xaa, 0x06, 0x4c, 
    0x35, 0x07, 0x60, 0xa6, 0x03, 0xca, 0x8a, 0xb5, 0x10, 0x95, 0x12, 0xca, 0x10, 0xf9, 0xa5, 0x02, 
    0x4a, 0xb0, 0x09, 0x4a, 0xb0, 0x19, 0x4a, 0xb0, 0x1f, 0x4a, 0xb0, 0x2f, 0xa5, 0x10, 0x38, 0xe9, 
    0x20, 0x85, 0x10, 0x90, 0x01, 0x60, 0xc6, 0x11, 0xa9, 0x01, 0xc5, 0x11, 0xf0, 0x28, 0x60, 0xe6, 
    0x10, 0xa9, 0x1f, 0x24, 0x10, 0xf0, 0x1f, 0x60, 0xa5, 0x10, 0x18, 0x69, 0x20, 0x85, 0x10, 0xb0, 
    0x01, 0x60, 0xe6, 0x11, 0xa9, 0x06, 0xc5, 0x11, 0xf0, 0x0c, 0x60, 0xc6, 0x10, 0xa5, 0x10, 0x29, 
    0x1f, 0xc9, 0x1f, 0xf0, 0x01, 0x60, 0x4c, 0x35, 0x07, 0xa0, 0x00, 0xa5, 0xfe, 0x91, 0x00, 0x60, 
    0xa6, 0x03, 0xa9, 0x00, 0x81, 0x10, 0xa2, 0x00, 0xa9, 0x01, 0x81, 0x10, 0x60, 0xa2, 0x00, 0xea, 
    0xea, 0xca, 0xd0, 0xfb, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
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
  std::string c = "x";
  while (true)
  {
    nesCpu.printStatus();
//    c = getchar();
    nesCpu.doInstruction();
    nesCpu.printZeroPage();
    nesCpu.printStack();

    nesPpu.updatePixels();
    nesPpu.RenderAll();
  }

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
