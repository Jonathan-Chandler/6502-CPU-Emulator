#ifndef CPU_HPP
#define CPU_HPP
#include <iostream>
#include "Memory.hpp"
#include "SDL2/SDL.h" 

class Cpu
{
  public:
    Cpu();
    uint8_t getFlags();
    void setFlags(uint8_t value);

    enum FlagMasks
    {
      carryMask     = 1,
      zeroMask      = 2,
      interruptMask = 4,
      decimalMask   = 8,
      breakMask     = 16,
      unusuedMask   = 32,
      overflowMask  = 64,
      negativeMask  = 128
    };

    enum FlagOffsets
    {
      carryOffset     = 0,
      zeroOffset      = 1,
      interruptOffset = 2,
      decimalOffset   = 3,
      breakOffset     = 4,
      unusuedOffset   = 5,
      overflowOffset  = 6,
      negativeOffset  = 7
    };

    void doInstruction(uint8_t *instrAddr);
    void doInstruction();
    uint16_t getProgramCounter();
    uint8_t getStackPointer();
    uint8_t getA();
    uint8_t *getAddressA();
    uint8_t getX();
    uint8_t getY();
    void setMemory(Memory *memory_controller);
    void setPc(uint16_t counter);
    void reset();
    void printStatus();
    void printStack();
    void printZeroPage();
    void handlePlayerInput(SDL_Event *event);

  private:
    typedef void (Cpu::*OpCode_T)(uint8_t *memoryAddr);

    static const uint8_t OperationCodeLookupTable[];

    static const OpCode_T OperationCodeFunctionTable[];     //
    static const uint8_t SizeLookupTable[];
    static const uint8_t TimingLookupTable[];

    // Bits 7 -> 0:
    // Flags NVss DIZC (AKA SVss DBZC)
    //
    bool negativeFlag;  // N : Set to bit 7 of last operation
    bool overflowFlag;  // V : 1 if last ADC or SBC resulted in signed overflow, or D6 from last BIT
    bool sHigh;         // sx: No effect, used by stack copy
    bool sLow;          // xs: No effect, used by stack copy
    bool decimalFlag;   // D : 1 to enable decimal mode
    bool interruptFlag; // I : 1 to disable maskable interrupts
    bool zeroFlag;      // Z : 1 if last operation resulted in a 0 value
    bool carryFlag;     // C : 1 if last addition or shift resulted in a carry, or if last subtraction resulted in no borrow

    bool breakFlag;     // use internally to signal BREAK
    bool crossedPage;   // signal 255-byte page boundary was crossed
    uint8_t cycles;     // number of cycles to wait before executing next instruction

    Memory  *memory;    // memory_callback
    uint8_t *startAddr; // Program Counter: 16 bits, reference &memory[(0x0 -> 0xFFFF)]
    uint16_t pc;        // Program Counter: 16 bits, reference &memory[(0x0 -> 0xFFFF)]
                        // On reset, reference address given from &memory[(memory[0xFFFD] << 4) | (memory[0xFFFC])]; 
    uint8_t sp;         // Stack Pointer: references &memory[(0x100 -> 0x1FF)]
                        // SP increments high to low: 0x1FF -> 0x100 
    uint8_t a;          // Accumulator register
    uint8_t x;          // X register
    uint8_t y;          // Y register

    // utility functions
    void generateRandomVar();                             // generate random value at 0x00FE
    bool testPageBoundary(uint8_t addressOffset);         // test if incrementing by offset will pass page boundary
    void incrementProgramCounter(uint16_t addressOffset); // increment by addressOffset
    void setProgramCounter(uint16_t addr);                // set PC to memory[addr]
    void setStackPointer(uint8_t addr);                   // set SP to memory[0x1FF - addr]
    uint8_t getTwosComplement(uint8_t value);             // get signed representation of uint8 value converted to signed int type
    uint8_t getOnesComplement(uint8_t value);             // used with sbc
    int getSignedRepresentation(uint8_t value);           // convert uint8_t to int
    int getSignedRepresentation(uint16_t value);          // convert uint16_t to int
    void pushStack(uint8_t value);                        // push 8 bits onto stack and increment stack pointer
    uint8_t popStack();                                   // pop 8 bits from stack and decrement stack pointer

    // Operation code instructions
    void iBRK(uint8_t *addr);                           // BReaKpoint
    void iORA(uint8_t *addr);                           // bitwise OR Accumulator
    void iCOP(uint8_t *addr);                           // COProcessor
    void iTSB(uint8_t *addr);                           // Test and Set Bits
    void iASL(uint8_t *addr);                           // Arithmetic Shift Left
    void iPHP(uint8_t *addr);                           // PusH Processor status register
    void iPHD(uint8_t *addr);                           // PusH Direct register
    void iBPL(uint8_t *addr);                           // Branch if PLus
    void iTRB(uint8_t *addr);                           // Test and Reset Bits
    void iCLC(uint8_t *addr);                           // CLear Carry
    void iINC(uint8_t *addr);                           // INCrement
    void iTCS(uint8_t *addr);                           // Transfer C accumulator to Stack pointer
    void iJSR(uint8_t *addr);                           // Jump to SubRoutine
    void iAND(uint8_t *addr);                           // bitwise AND
    void iJSL(uint8_t *addr);                           // Jump to Subroutine Long
    void iBIT(uint8_t *addr);                           // test BITs
    void iROL(uint8_t *addr);                           // ROtate Left
    void iPLP(uint8_t *addr);                           // PulL Processor status register
    void iPLD(uint8_t *addr);                           // PulL Direct register
    void iBMI(uint8_t *addr);                           // Branch if MInus
    void iSEC(uint8_t *addr);                           // SEt Carry
    void iDEC(uint8_t *addr);                           // DECrement
    void iTSC(uint8_t *addr);                           // Transfer Stack pointer to C accumulator
    void iRTI(uint8_t *addr);                           // ReTurn from Interrupt
    void iEOR(uint8_t *addr);                           // bitwise exclusive OR
    void iWDM(uint8_t *addr);                           // William D. Mensch, Jr. (2-byte, 2-cycle NOP)
    void iMVP(uint8_t *addr);                           // MoVe memory Positive
    void iLSR(uint8_t *addr);                           // Logical Shift Right
    void iPHA(uint8_t *addr);                           // PusH Accumulator
    void iPHK(uint8_t *addr);                           // PusH K register
    void iJMP(uint8_t *addr);                           // JuMP
    void iBVC(uint8_t *addr);                           // Branch if oVerflow Clear
    void iMVN(uint8_t *addr);                           // MoVe memory Negative
    void iCLI(uint8_t *addr);                           // CLear Interrupt disable
    void iPHY(uint8_t *addr);                           // PusH Y register
    void iTCD(uint8_t *addr);                           // Transfer C accumulator to Direct register
    void iRTS(uint8_t *addr);                           // ReTurn from Subroutine
    void iADC(uint8_t *addr);                           // ADd with Carry
    void iPER(uint8_t *addr);                           // Push Effective Relative address
    void iSTZ(uint8_t *addr);                           // STore Zero
    void iROR(uint8_t *addr);                           // ROtate Right
    void iPLA(uint8_t *addr);                           // PulL Accumulator
    void iRTL(uint8_t *addr);                           // ReTurn from subroutine Long
    void iBVS(uint8_t *addr);                           // Branch if oVerflow Set
    void iSEI(uint8_t *addr);                           // SEt Interrupt disable
    void iPLY(uint8_t *addr);                           // PulL Y register
    void iTDC(uint8_t *addr);                           // Transfer Direct register to C accumulator
    void iBRA(uint8_t *addr);                           // BRanch Always
    void iSTA(uint8_t *addr);                           // STore Accumulator
    void iBRL(uint8_t *addr);                           // BRanch Long
    void iSTY(uint8_t *addr);                           // STore Y register
    void iSTX(uint8_t *addr);                           // STore X register
    void iDEY(uint8_t *addr);                           // DEcrement Y register
    void iTXA(uint8_t *addr);                           // Transfer X register to Accumulator
    void iPHB(uint8_t *addr);                           // PusH data Bank register
    void iBCC(uint8_t *addr);                           // Branch if Carry Clear
    void iTYA(uint8_t *addr);                           // Transfer Y register to Accumulator
    void iTXS(uint8_t *addr);                           // Transfer X register to Stack pointer
    void iTXY(uint8_t *addr);                           // Transfer X register to Y register
    void iLDY(uint8_t *addr);                           // LoaD Y register
    void iLDA(uint8_t *addr);                           // LoaD Accumulator
    void iLDX(uint8_t *addr);                           // LoaD X register
    void iTAY(uint8_t *addr);                           // Transfer Accumulator to Y register
    void iTAX(uint8_t *addr);                           // Transfer Accumulator to X register
    void iPLB(uint8_t *addr);                           // PulL data Bank register
    void iBCS(uint8_t *addr);                           // Branch if Carry Set
    void iCLV(uint8_t *addr);                           // CLear oVerflow
    void iTSX(uint8_t *addr);                           // Transfer Stack pointer to X register
    void iTYX(uint8_t *addr);                           // Transfer Y register to X register
    void iCPY(uint8_t *addr);                           // ComPare to Y register
    void iCMP(uint8_t *addr);                           // CoMPare (to accumulator)
    void iREP(uint8_t *addr);                           // REset Processor status bits
    void iINY(uint8_t *addr);                           // INcrement Y register
    void iDEX(uint8_t *addr);                           // DEcrement X register
    void iWAI(uint8_t *addr);                           // WAit for Interrupt
    void iBNE(uint8_t *addr);                           // Branch if Not Equal
    void iPEI(uint8_t *addr);                           // Push Effective Indirect address
    void iCLD(uint8_t *addr);                           // CLear Decimal mode
    void iPHX(uint8_t *addr);                           // PusH X register
    void iSTP(uint8_t *addr);                           // SToP the clock
    void iJML(uint8_t *addr);                           // JuMP Long
    void iCPX(uint8_t *addr);                           // ComPare to X register
    void iSBC(uint8_t *addr);                           // SuBtract with Carry
    void iSEP(uint8_t *addr);                           // SEt Processor status bits
    void iINX(uint8_t *addr);                           // INcrement X register
    void iNOP(uint8_t *addr);                           // No OPeration
    void iXBA(uint8_t *addr);                           // eXchange B and A accumulator
    void iBEQ(uint8_t *addr);                           // Branch if EQual
    void iPEA(uint8_t *addr);                           // Push Effective Address
    void iSED(uint8_t *addr);                           // SEt Decimal mode
    void iPLX(uint8_t *addr);                           // PulL X register
    void iXCE(uint8_t *addr);                           // eXchange Carry and Emulation flags
};
#endif
