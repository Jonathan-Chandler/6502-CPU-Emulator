# All projects (default target)

COMPILER_FLAGS := -Wall -std=c++14 -pipe
LINKER_FLAGS := -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

#all: Main.o Rom.o Cpu.o Ppu.o CpuTest.o
#	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) Main.o CpuTest.o Rom.o Cpu.o Ppu.o -o emu.exe

all: Main.o Rom.o Cpu.o Ppu.o
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) Main.o Rom.o Cpu.o Ppu.o -o emu.exe

Main.o : Main.cpp
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -c Main.cpp

Rom.o : Rom.cpp
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -c Rom.cpp

Cpu.o : Cpu.cpp
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -c Cpu.cpp

Ppu.o : Ppu.cpp
	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -c Ppu.cpp

# CpuTest.o : CpuTest.tcc
# 	g++ -g $(COMPILER_FLAGS) $(LINKER_FLAGS) -c CpuTest.tcc

clean: 
	rm *.o *.out *.exe
