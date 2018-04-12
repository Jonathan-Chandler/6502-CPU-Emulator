#ifndef CPU_HPP
#define CPU_HPP
#include <iostream>
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

//BRK b   ORA (d,X)   cop b     ora d,S       Tsb d     ORA d     ASL d     ora [d]     PHP   ORA #     ASL A   phd   Tsb a       ORA a     ASL a     ora al
//BPL r   ORA (d),Y   Ora (d)   ora (d,S),Y   Trb d     ORA d,X   ASL d,X   ora [d],Y   CLC   ORA a,Y   Inc A   tcs   Trb a       ORA a,X   ASL a,X   ora al,X
//JSR a   AND (d,X)   jsl al    and d,S       BIT d     AND d     ROL d     and [d]     PLP   AND #     ROL A   pld   BIT a       AND a     ROL a     and al
//BMI r   AND (d),Y   And (d)   and (d,S),Y   Bit d,X   AND d,X   ROL d,X   and [d],Y   SEC   AND a,Y   Dec A   tsc   Bit a,X     AND a,X   ROL a,X   and al,X
//RTI     EOR (d,X)   wdm       eor d,S       mvp s,d   EOR d     LSR d     eor [d]     PHA   EOR #     LSR A   phk   JMP a       EOR a     LSR a     eor al
//BVC r   EOR (d),Y   Eor (d)   eor (d,S),Y   mvn s,d   EOR d,X   LSR d,X   eor [d],Y   CLI   EOR a,Y   Phy     tcd   jmp al      EOR a,X   LSR a,X   eor al,X
//RTS     ADC (d,X)   per rl    adc d,S       Stz d     ADC d     ROR d     adc [d]     PLA   ADC #     ROR A   rtl   JMP (a)     ADC a     ROR a     adc al
//BVS r   ADC (d),Y   Adc (d)   adc (d,S),Y   Stz d,X   ADC d,X   ROR d,X   adc [d],Y   SEI   ADC a,Y   Ply     tdc   Jmp (a,X)   ADC a,X   ROR a,X   adc al,X
//Bra r   STA (d,X)   brl rl    sta d,S       STY d     STA d     STX d     sta [d]     DEY   Bit #     TXA     phb   STY a       STA a     STX a     sta al
//BCC r   STA (d),Y   Sta (d)   sta (d,S),Y   STY d,X   STA d,X   STX d,Y   sta [d],Y   TYA   STA a,Y   TXS     txy   Stz a       STA a,X   Stz a,X   sta al,X
//LDY #   LDA (d,X)   LDX #     lda d,S       LDY d     LDA d     LDX d     lda [d]     TAY   LDA #     TAX     plb   LDY a       LDA a     LDX a     lda al
//BCS r   LDA (d),Y   Lda (d)   lda (d,S),Y   LDY d,X   LDA d,X   LDX d,Y   lda [d],Y   CLV   LDA a,Y   TSX     tyx   LDY a,X     LDA a,X   LDX a,Y   lda al,X
//CPY #   CMP (d,X)   rep #     cmp d,S       CPY d     CMP d     DEC d     cmp [d]     INY   CMP #     DEX     wai   CPY a       CMP a     DEC a     cmp al
//BNE r   CMP (d),Y   Cmp (d)   cmp (d,S),Y   pei d     CMP d,X   DEC d,X   cmp [d],Y   CLD   CMP a,Y   Phx     stp   jml (a)     CMP a,X   DEC a,X   cmp al,X
//CPX #   SBC (d,X)   sep #     sbc d,S       CPX d     SBC d     INC d     sbc [d]     INX   SBC #     NOP     xba   CPX a       SBC a     INC a     sbc al
//BEQ r   SBC (d),Y   Sbc (d)   sbc (d,S),Y   pea a     SBC d,X   INC d,X   sbc [d],Y   SED   SBC a,Y   Plx     xce   jsr (a,X)   SBC a,X   INC a,X   sbc al,X


//    {
//      b,            IndirZeroX,        b,             dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  A,      None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//      r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectZ,     ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         A,      None,   AbsoluteZ,  AbsoluteX,   AbsoluteX,   alX,
//      AbsoluteZ,    IndirZeroX,        al,            dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  A,      None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//      r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectX,     ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         A,      None,   AbsoluteX,  AbsoluteX,   AbsoluteX,   alX,
//      None,         IndirZeroX,        None,          dS,      Sd,              ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  A,      None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//      r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   Sd,              ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         None,   None,   al,         AbsoluteX,   AbsoluteX,   alX,
//      None,         IndirZeroX,        rl,            dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  A,      None,   pap,        AbsoluteZ,   AbsoluteZ,   al,
//      r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectX,     ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         None,   None,   paXp,       AbsoluteX,   AbsoluteX,   alX,
//      r,            IndirZeroX,        rl,            dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  None,   None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//      r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectX,     ZeroDirectX,    ZeroDirectY,  None,   None,   aY,         None,   None,   AbsoluteZ,  AbsoluteX,   AbsoluteX,   alX,
//      Immediate,    IndirZeroX,        Immediate,     dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  None,   None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//      r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectX,     ZeroDirectX,    ZeroDirectY,  None,   None,   aY,         None,   None,   AbsoluteX,  AbsoluteX,   AbsoluteY,   alX,
//      Immediate,    IndirZeroX,        Immediate,     dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  None,   None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//      r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectZ,     ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         None,   None,   pap,        AbsoluteX,   AbsoluteX,   alX,
//      Immediate,    IndirZeroX,        Immediate,     dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  None,   None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//      r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   AbsoluteZ,       ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         None,   None,   paXp,       AbsoluteX,   AbsoluteX,   alX,
//    };

    void doInstruction(uint8_t *instrAddr);
    void doInstruction();
    uint16_t getProgramCounter();
    uint8_t getStackPointer();
    uint8_t getA();
    uint8_t getX();
    uint8_t getY();
    void setMemory(uint8_t *memoryAddr);
    void setPc(uint16_t counter);
    void reset();
    void printStatus();
    void printStack();
    void printZeroPage();
    void loadRom(char *filename);
    void handlePlayerInput(SDL_Event *event);

  private:
    typedef uint8_t* (Cpu::*AddressMode_T)(uint8_t *instructionAddr);
    typedef void (Cpu::*OpCode_T)(uint8_t *memoryAddr);

    static const uint8_t OperationCodeLookupTable[];
    static const uint8_t AddressModeLookupTable[];
    static const uint8_t AddressModeSizeTable[];

    static const AddressMode_T AddressModeFunctionTable[]; // 
    static const OpCode_T OperationCodeFunctionTable[]; //
    static const uint8_t SizeLookupTable[];
    static const uint8_t TimingLookupTable[];

    // Bits 7 -> 0:
    // Flags NVss DIZC (also SVss DBZC)
    //
    bool carryFlag;     // C : 1 if last addition or shift resulted in a carry, or if last subtraction resulted in no borrow
    bool zeroFlag;      // Z : 1 if last operation resulted in a 0 value
    bool interruptFlag; // I : 1 to disable maskable interrupts
    bool decimalFlag;   // D : 1 to enable decimal mode
    bool overflowFlag;  // V : 1 if last ADC or SBC resulted in signed overflow, or D6 from last BIT
    bool negativeFlag;  // N : Set to bit 7 of last operation
    bool sHigh;         // sx: No effect, used by stack copy
    bool sLow;          // xs: No effect, used by stack copy

    bool breakFlag;     // use internally to signal BREAK
    bool crossedPage;   // signal 255-byte page boundary was crossed
    uint8_t cycles;     // number of cycles to wait before executing next instruction

    uint8_t *startAddr; // Program Counter: 16 bits, reference &memory[(0x0 -> 0xFFFF)]
    uint16_t pc;        // Program Counter: 16 bits, reference &memory[(0x0 -> 0xFFFF)]

//    uint8_t *pc;        // Program Counter: 16 bits, reference &memory[(0x0 -> 0xFFFF)]
//                        // On reset, reference address given from &memory[(memory[0xFFFD] << 4) | (memory[0xFFFC])]; 
    uint8_t sp;         // Stack Pointer: references &memory[(0x100 -> 0x1FF)]
                        // SP increments high to low: 0x1FF -> 0x100 
    uint8_t a;          // Accumulator register
    uint8_t x;          // X register
    uint8_t y;          // Y register

    // Memory
    // ======
    // 0x100   => Zero Page (0x0 -> 0xFF)
    // 0x200   => Stack     (0x100 -> 0x1FF)
    // 0x800   => RAM       (0x200 -> )
    // 0x2000  => Mirrors (0-0x7FF)
    // 0x2008  => I/O Registers
    // 0x4000  => Mirrors (0x2000-0x2007)
    // 0x4020  => I/O Registers
    // 0x6000  => Expansion ROM
    // 0x8000  => SRAM
    // 0xC000  => PRG-ROM (Lower Bank)
    // 0x10000 => PRG-ROM (Upper Bank)
    // 
    // Memory
    // ======
    // $0000-$07FF  $0800   2KB internal RAM
    // $0800-$0FFF  $0800   Mirrors of $0000-$07FF
    // $1000-$17FF  $0800
    // $1800-$1FFF  $0800
    // $2000-$2007  $0008   NES PPU registers
    // $2008-$3FFF  $1FF8   Mirrors of $2000-2007 (repeats every 8 bytes)
    // $4000-$4017  $0018   NES APU and I/O registers
    // $4018-$401F  $0008   APU and I/O functionality that is normally disabled. See CPU Test Mode.
    // $4020-$FFFF  $BFE0   Cartridge space: PRG ROM, PRG RAM, and mapper registers (See Note)
    //
    // Memory
    // ======
    // $0000-$000F  16 bytes    Local variables and function arguments
    // $0010-$00FF  240 bytes   Global variables accessed most often, including certain pointer tables
    // $0100-$019F  160 bytes   Data to be copied to nametable during next vertical blank (see The frame and NMIs)
    // $01A0-$01FF  96 bytes    Stack
    // $0200-$02FF  256 bytes   Data to be copied to OAM during next vertical blank
    // $0300-$03FF  256 bytes   Variables used by sound player, and possibly other variables
    // $0400-$07FF  1024 bytes  Arrays and less-often-accessed global variables
    //
#define MEMORY_SIZE 0x10000
    uint8_t memory[0x10000]; // 0 -> 0xFFFF

    // 0x0000 -> 0x000F - header
    // 0x0010 -> 0x0D70 - blank
    // 0x0D70 -> 0x2008 - ?
    // 0x2009 -> 0x3F6F - blank
    // 0x3F60 -> 0x3FCF - ?
    // 0x3FD0 -> 0x4009 - blank
    // 0x400A -> 0x4CBD - ?
    // 0x4CBE -> 0x4D0F - blank
    // 0x4D10 -> 0xaaaa - ?
    // D60
    //

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

    // Addressing modes
    uint8_t *AddressNone(uint8_t *instructionAddr);                   // No addressing type is used for function
    uint8_t *AddressImmediate(uint8_t *instructionAddr);              // Return the address of the value following the instruction
    
    uint8_t *AddressDirectZeroX(uint8_t *instructionAddr);            // Return the address: &memory[VAL + X]; If VAL + X overflows, wrapped to the zero page only (VAL is 1 byte)
    uint8_t *AddressDirectZeroY(uint8_t *instructionAddr);            // Return the address: &memory[VAL + Y]; If VAL + Y overflows, wrapped to the zero page only (VAL is 1 byte)
    uint8_t *AddressDirectZeroZ(uint8_t *instructionAddr);            // Return the address: &memory[VAL]; Overflow not possible (VAL is 1 byte)
    
    uint8_t *AddressDirectAbsX(uint8_t *instructionAddr);             // Return the address: &memory[VAL + X]; (VAL is 2 bytes)
    uint8_t *AddressDirectAbsY(uint8_t *instructionAddr);             // Return the address: &memory[VAL + X]; (VAL is 2 bytes)
    uint8_t *AddressDirectAbsZ(uint8_t *instructionAddr);             // Return the address: &memory[VAL]; (VAL is 2 bytes)
    
    uint8_t *AddressIndirectZeroX(uint8_t *instructionAddr);          // Return the address: &memory[*(&memory[VAL + X])]; If VAL + X overflows, wrapped to zero page (VAL is 1 byte)
    uint8_t *AddressIndirectZeroZ(uint8_t *instructionAddr);          // Return the address: &memory[*(&memory[VAL + X])]; If VAL + X overflows, wrapped to zero page (VAL is 1 byte)

    uint8_t *AddressIndirectZeroIndexY(uint8_t *instructionAddr);     // Return the address: &memory[*(&memory[VAL]) + Y]; (VAL is 1 byte)
      
    uint8_t *AddressIndirectAbsX(uint8_t *instructionAddr);           // Return the address: &memory[*(&memory[VAL + X]); (VAL is 2 bytes)
    uint8_t *AddressIndirectAbsZ(uint8_t *instructionAddr);           // Return the address: &memory[*(&memory[VAL]); (VAL is 2 bytes)
    uint8_t *AddressRelative(uint8_t *instructionAddr);               // Return the address: &memory[PC + signed(VAL)]; (VAL is 1 byte)
    uint8_t *AddressRegisterA(uint8_t *instructionAddr);              // Return the address of CPU register A
};
#endif
