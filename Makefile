# All projects (default target)

COMPILER_FLAGS := -Wall -std=c++14 -pipe
LINKER_FLAGS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

all: Main.o Memory.o Cpu.o Ppu.o
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) Main.o Memory.o Cpu.o Ppu.o -o Emulator.exe

Main.o : Main.cpp
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -c Main.cpp

Memory.o : Memory.cpp
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -c Memory.cpp

Cpu.o : Cpu.cpp
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -c Cpu.cpp

Ppu.o : Ppu.cpp
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -c Ppu.cpp

clean: 
	rm *.o *.out *.exe
