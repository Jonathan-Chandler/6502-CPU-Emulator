#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <iostream>

class Memory
{
  public:
    Memory();

    void loadRom(char *fileName);

    void printHeader();
    void printTrainer();
    void printRom();
    void printChr();
    void printInstRom();
    void printProm();

    uint8_t getPrgSize();
    uint8_t *getChrRomData();
    uint8_t *getMemory();

//    // Addressing modes
//    uint8_t *AddressNone(uint8_t *instructionAddr);                   // No addressing type is used for function
//    uint8_t *AddressImmediate(uint8_t *instructionAddr);              // Return the address of the value following the instruction
//    
//    uint8_t *AddressDirectZeroX(uint8_t *instructionAddr);            // Return the address: &memory[VAL + X]; If VAL + X overflows, wrapped to the zero page only (VAL is 1 byte)
//    uint8_t *AddressDirectZeroY(uint8_t *instructionAddr);            // Return the address: &memory[VAL + Y]; If VAL + Y overflows, wrapped to the zero page only (VAL is 1 byte)
//    uint8_t *AddressDirectZeroZ(uint8_t *instructionAddr);            // Return the address: &memory[VAL]; Overflow not possible (VAL is 1 byte)
//    
//    uint8_t *AddressDirectAbsX(uint8_t *instructionAddr);             // Return the address: &memory[VAL + X]; (VAL is 2 bytes)
//    uint8_t *AddressDirectAbsY(uint8_t *instructionAddr);             // Return the address: &memory[VAL + X]; (VAL is 2 bytes)
//    uint8_t *AddressDirectAbsZ(uint8_t *instructionAddr);             // Return the address: &memory[VAL]; (VAL is 2 bytes)
//    
//    uint8_t *AddressIndirectZeroX(uint8_t *instructionAddr);          // Return the address: &memory[*(&memory[VAL + X])]; If VAL + X overflows, wrapped to zero page (VAL is 1 byte)
//    uint8_t *AddressIndirectZeroZ(uint8_t *instructionAddr);          // Return the address: &memory[*(&memory[VAL + X])]; If VAL + X overflows, wrapped to zero page (VAL is 1 byte)
//
//    uint8_t *AddressIndirectZeroIndexY(uint8_t *instructionAddr);     // Return the address: &memory[*(&memory[VAL]) + Y]; (VAL is 1 byte)
//      
//    uint8_t *AddressIndirectAbsX(uint8_t *instructionAddr);           // Return the address: &memory[*(&memory[VAL + X]); (VAL is 2 bytes)
//    uint8_t *AddressIndirectAbsZ(uint8_t *instructionAddr);           // Return the address: &memory[*(&memory[VAL]); (VAL is 2 bytes)
//    uint8_t *AddressRelative(uint8_t *instructionAddr);               // Return the address: &memory[PC + signed(VAL)]; (VAL is 1 byte)

  private:
    uint8_t header[16];
    uint8_t *PrgRomData;
    uint8_t *ChrRomData;
    uint8_t localMem[0x10000];

};
#endif
