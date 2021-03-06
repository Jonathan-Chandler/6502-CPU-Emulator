#include "Cpu.hpp"
#include <string.h>
#include <cstdlib>

enum OperationEnum
{
                                //  description
  BRK,                          // BReaKpoint
  ORA,                          // bitwise OR Accumulator
  COP,                          // COProcessor
  TSB,                          // Test and Set Bits
  ASL,                          // Arithmetic Shift Left
  PHP,                          // PusH Processor status register
  PHD,                          // PusH Direct register
  BPL,                          // Branch if PLus
  TRB,                          // Test and Reset Bits
  CLC,                          // CLear Carry
  INC,                          // INCrement
  TCS,                          // Transfer C accumulator to Stack pointer
  JSR,                          // Jump to SubRoutine
  AND,                          // bitwise AND
  JSL,                          // Jump to Subroutine Long
  BIT,                          // test BITs
  ROL,                          // ROtate Left
  PLP,                          // PulL Processor status register
  PLD,                          // PulL Direct register
  BMI,                          // Branch if MInus
  SEC,                          // SEt Carry
  DEC,                          // DECrement
  TSC,                          // Transfer Stack pointer to C accumulator
  RTI,                          // ReTurn from Interrupt
  EOR,                          // bitwise exclusive OR
  WDM,                          // William D. Mensch, Jr. (2-byte, 2-cycle NOP)
  MVP,                          // MoVe memory Positive
  LSR,                          // Logical Shift Right
  PHA,                          // PusH Accumulator
  PHK,                          // PusH K register
  JMP,                          // JuMP
  BVC,                          // Branch if oVerflow Clear
  MVN,                          // MoVe memory Negative
  CLI,                          // CLear Interrupt disable
  PHY,                          // PusH Y register
  TCD,                          // Transfer C accumulator to Direct register
  RTS,                          // ReTurn from Subroutine
  ADC,                          // ADd with Carry
  PER,                          // Push Effective Relative address
  STZ,                          // STore Zero
  ROR,                          // ROtate Right
  PLA,                          // PulL Accumulator
  RTL,                          // ReTurn from subroutine Long
  BVS,                          // Branch if oVerflow Set
  SEI,                          // SEt Interrupt disable
  PLY,                          // PulL Y register
  TDC,                          // Transfer Direct register to C accumulator
  BRA,                          // BRanch Always
  STA,                          // STore Accumulator
  BRL,                          // BRanch Long
  STY,                          // STore Y register
  STX,                          // STore X register
  DEY,                          // DEcrement Y register
  TXA,                          // Transfer X register to Accumulator
  PHB,                          // PusH data Bank register
  BCC,                          // Branch if Carry Clear
  TYA,                          // Transfer Y register to Accumulator
  TXS,                          // Transfer X register to Stack pointer
  TXY,                          // Transfer X register to Y register
  LDY,                          // LoaD Y register
  LDA,                          // LoaD Accumulator
  LDX,                          // LoaD X register
  TAY,                          // Transfer Accumulator to Y register
  TAX,                          // Transfer Accumulator to X register
  PLB,                          // PulL data Bank register
  BCS,                          // Branch if Carry Set
  CLV,                          // CLear oVerflow
  TSX,                          // Transfer Stack pointer to X register
  TYX,                          // Transfer Y register to X register
  CPY,                          // ComPare to Y register
  CMP,                          // CoMPare (to accumulator)
  REP,                          // REset Processor status bits
  INY,                          // INcrement Y register
  DEX,                          // DEcrement X register
  WAI,                          // WAit for Interrupt
  BNE,                          // Branch if Not Equal
  PEI,                          // Push Effective Indirect address
  CLD,                          // CLear Decimal mode
  PHX,                          // PusH X register
  STP,                          // SToP the clock
  JML,                          // JuMP Long
  CPX,                          // ComPare to X register
  SBC,                          // SuBtract with Carry
  SEP,                          // SEt Processor status bits
  INX,                          // INcrement X register
  NOP,                          // No OPeration
  XBA,                          // eXchange B and A accumulator
  BEQ,                          // Branch if EQual
  PEA,                          // Push Effective Address
  SED,                          // SEt Decimal mode
  PLX,                          // PulL X register
  XCE,                          // eXchange Carry and Emulation flags
};                            

// TOOD: break if invalid instruction executed
#define KIL 0xFF

// high 4 bits for cycles to add if page is crossed, lower 4 bits are always added
const uint8_t Cpu::TimingLookupTable[] = 
{
  //        x0     x1      x2       x3     x4      x5     x6     x7     x8       x9       xA       xB       xC       xD       xE      xF
  /*0x*/    7,     6,      KIL,     8,     3,      3,     5,     5,     3,       2,       2,       2,       4,       4,       6,      6,
  /*1x*/    12,    15,     KIL,     8,     4,      4,     6,     6,     2,       14,      2,       7,       14,      14,      7,      7,
  /*2x*/    6,     6,      KIL,     8,     3,      3,     5,     5,     4,       2,       2,       2,       4,       4,       6,      6,
  /*3x*/    12,    15,     KIL,     8,     4,      4,     6,     6,     2,       14,      2,       7,       14,      14,      7,      7,
  /*4x*/    6,     6,      KIL,     8,     3,      3,     5,     5,     3,       2,       2,       2,       3,       4,       6,      6,
  /*5x*/    12,    15,     KIL,     8,     4,      4,     6,     6,     2,       14,      2,       7,       14,      14,      7,      7,
  /*6x*/    6,     6,      KIL,     8,     3,      3,     5,     5,     4,       2,       2,       2,       5,       4,       6,      6,
  /*7x*/    12,    15,     KIL,     8,     4,      4,     6,     6,     2,       14,      2,       7,       14,      14,      7,      7,
  /*8x*/    2,     6,      2,       6,     3,      3,     3,     3,     2,       2,       2,       2,       4,       4,       4,      4,
  /*9x*/    12,    6,      KIL,     6,     4,      4,     4,     4,     2,       5,       2,       5,       5,       5,       5,      5,
  /*Ax*/    2,     6,      2,       6,     3,      3,     3,     3,     2,       2,       2,       2,       4,       4,       4,      4,
  /*Bx*/    12,    15,     KIL,     15,    4,      4,     4,     4,     2,       14,      2,       14,      14,      14,      14,     14,
  /*Cx*/    2,     6,      2,       8,     3,      3,     5,     5,     2,       2,       2,       2,       4,       4,       6,      6,
  /*Dx*/    12,    15,     KIL,     8,     4,      4,     6,     6,     2,       14,      2,       7,       14,      14,      7,      7,
  /*Ex*/    2,     6,      2,       8,     3,      3,     5,     5,     2,       2,       2,       2,       4,       4,       6,      6,
  /*Fx*/    12,    15,     KIL,     8,     4,      4,     6,     6,     2,       14,      2,       7,       14,      14,      7,      7,
};

const uint8_t Cpu::SizeLookupTable[] = 
{
  BRK,  ORA,  COP,  ORA,  TSB,  ORA,  ASL,  ORA,  PHP,  ORA,  ASL,  PHD,  TSB,  ORA,  ASL,  ORA,
  BPL,  ORA,  ORA,  ORA,  TRB,  ORA,  ASL,  ORA,  CLC,  ORA,  INC,  TCS,  TRB,  ORA,  ASL,  ORA,
  JSR,  AND,  JSL,  AND,  BIT,  AND,  ROL,  AND,  PLP,  AND,  ROL,  PLD,  BIT,  AND,  ROL,  AND,
  BMI,  AND,  AND,  AND,  BIT,  AND,  ROL,  AND,  SEC,  AND,  DEC,  TSC,  BIT,  AND,  ROL,  AND,
  RTI,  EOR,  WDM,  EOR,  MVP,  EOR,  LSR,  EOR,  PHA,  EOR,  LSR,  PHK,  JMP,  EOR,  LSR,  EOR,
  BVC,  EOR,  EOR,  EOR,  MVN,  EOR,  LSR,  EOR,  CLI,  EOR,  PHY,  TCD,  JMP,  EOR,  LSR,  EOR,
  RTS,  ADC,  PER,  ADC,  STZ,  ADC,  ROR,  ADC,  PLA,  ADC,  ROR,  RTL,  JMP,  ADC,  ROR,  ADC,
  BVS,  ADC,  ADC,  ADC,  STZ,  ADC,  ROR,  ADC,  SEI,  ADC,  PLY,  TDC,  JMP,  ADC,  ROR,  ADC,
  BRA,  STA,  BRL,  STA,  STY,  STA,  STX,  STA,  DEY,  BIT,  TXA,  PHB,  STY,  STA,  STX,  STA,
  BCC,  STA,  STA,  STA,  STY,  STA,  STX,  STA,  TYA,  STA,  TXS,  TXY,  STZ,  STA,  STZ,  STA,
  LDY,  LDA,  LDX,  LDA,  LDY,  LDA,  LDX,  LDA,  TAY,  LDA,  TAX,  PLB,  LDY,  LDA,  LDX,  LDA,
  BCS,  LDA,  LDA,  LDA,  LDY,  LDA,  LDX,  LDA,  CLV,  LDA,  TSX,  TYX,  LDY,  LDA,  LDX,  LDA,
  CPY,  CMP,  REP,  CMP,  CPY,  CMP,  DEC,  CMP,  INY,  CMP,  DEX,  WAI,  CPY,  CMP,  DEC,  CMP,
  BNE,  CMP,  CMP,  CMP,  PEI,  CMP,  DEC,  CMP,  CLD,  CMP,  PHX,  STP,  JML,  CMP,  DEC,  CMP,
  CPX,  SBC,  SEP,  SBC,  CPX,  SBC,  INC,  SBC,  INX,  SBC,  NOP,  XBA,  CPX,  SBC,  INC,  SBC,
  BEQ,  SBC,  SBC,  SBC,  PEA,  SBC,  INC,  SBC,  SED,  SBC,  PLX,  XCE,  JSR,  SBC,  INC,  SBC,
};

const uint8_t Cpu::OperationCodeLookupTable[] = 
{
// x0    x1    x2    x3    x4    x5    x6    x7    x8    x9    xA    xB    xC    xD    xE    xF
  BRK,  ORA,  COP,  ORA,  TSB,  ORA,  ASL,  ORA,  PHP,  ORA,  ASL,  PHD,  TSB,  ORA,  ASL,  ORA,  // 0x
  BPL,  ORA,  ORA,  ORA,  TRB,  ORA,  ASL,  ORA,  CLC,  ORA,  INC,  TCS,  TRB,  ORA,  ASL,  ORA,  // 1x
  JSR,  AND,  JSL,  AND,  BIT,  AND,  ROL,  AND,  PLP,  AND,  ROL,  PLD,  BIT,  AND,  ROL,  AND,  // 2x
  BMI,  AND,  AND,  AND,  BIT,  AND,  ROL,  AND,  SEC,  AND,  DEC,  TSC,  BIT,  AND,  ROL,  AND,  // 3x
  RTI,  EOR,  WDM,  EOR,  MVP,  EOR,  LSR,  EOR,  PHA,  EOR,  LSR,  PHK,  JMP,  EOR,  LSR,  EOR,  // 4x
  BVC,  EOR,  EOR,  EOR,  MVN,  EOR,  LSR,  EOR,  CLI,  EOR,  PHY,  TCD,  JMP,  EOR,  LSR,  EOR,  // 5x
  RTS,  ADC,  PER,  ADC,  STZ,  ADC,  ROR,  ADC,  PLA,  ADC,  ROR,  RTL,  JMP,  ADC,  ROR,  ADC,  // 6x
  BVS,  ADC,  ADC,  ADC,  STZ,  ADC,  ROR,  ADC,  SEI,  ADC,  PLY,  TDC,  JMP,  ADC,  ROR,  ADC,  // 7x
  BRA,  STA,  BRL,  STA,  STY,  STA,  STX,  STA,  DEY,  BIT,  TXA,  PHB,  STY,  STA,  STX,  STA,  // 8x
  BCC,  STA,  STA,  STA,  STY,  STA,  STX,  STA,  TYA,  STA,  TXS,  TXY,  STZ,  STA,  STZ,  STA,  // 9x
  LDY,  LDA,  LDX,  LDA,  LDY,  LDA,  LDX,  LDA,  TAY,  LDA,  TAX,  PLB,  LDY,  LDA,  LDX,  LDA,  // Ax
  BCS,  LDA,  LDA,  LDA,  LDY,  LDA,  LDX,  LDA,  CLV,  LDA,  TSX,  TYX,  LDY,  LDA,  LDX,  LDA,  // Bx
  CPY,  CMP,  REP,  CMP,  CPY,  CMP,  DEC,  CMP,  INY,  CMP,  DEX,  WAI,  CPY,  CMP,  DEC,  CMP,  // Cx
  BNE,  CMP,  CMP,  CMP,  PEI,  CMP,  DEC,  CMP,  CLD,  CMP,  PHX,  STP,  JML,  CMP,  DEC,  CMP,  // Dx
  CPX,  SBC,  SEP,  SBC,  CPX,  SBC,  INC,  SBC,  INX,  SBC,  NOP,  XBA,  CPX,  SBC,  INC,  SBC,  // Ex
  BEQ,  SBC,  SBC,  SBC,  PEA,  SBC,  INC,  SBC,  SED,  SBC,  PLX,  XCE,  JSR,  SBC,  INC,  SBC,  // Fx
};

const Cpu::OpCode_T Cpu::OperationCodeFunctionTable[] = 
{
  &Cpu::iBRK,                          // BReaKpoint
  &Cpu::iORA,                          // bitwise OR Accumulator
  &Cpu::iCOP,                          // COProcessor
  &Cpu::iTSB,                          // Test and Set Bits
  &Cpu::iASL,                          // Arithmetic Shift Left
  &Cpu::iPHP,                          // PusH Processor status register
  &Cpu::iPHD,                          // PusH Direct register
  &Cpu::iBPL,                          // Branch if PLus
  &Cpu::iTRB,                          // Test and Reset Bits
  &Cpu::iCLC,                          // CLear Carry
  &Cpu::iINC,                          // INCrement
  &Cpu::iTCS,                          // Transfer C accumulator to Stack pointer
  &Cpu::iJSR,                          // Jump to SubRoutine
  &Cpu::iAND,                          // bitwise AND
  &Cpu::iJSL,                          // Jump to Subroutine Long
  &Cpu::iBIT,                          // test BITs
  &Cpu::iROL,                          // ROtate Left
  &Cpu::iPLP,                          // PulL Processor status register
  &Cpu::iPLD,                          // PulL Direct register
  &Cpu::iBMI,                          // Branch if MInus
  &Cpu::iSEC,                          // SEt Carry
  &Cpu::iDEC,                          // DECrement
  &Cpu::iTSC,                          // Transfer Stack pointer to C accumulator
  &Cpu::iRTI,                          // ReTurn from Interrupt
  &Cpu::iEOR,                          // bitwise exclusive OR
  &Cpu::iWDM,                          // William D. Mensch, Jr. (2-byte, 2-cycle NOP)
  &Cpu::iMVP,                          // MoVe memory Positive
  &Cpu::iLSR,                          // Logical Shift Right
  &Cpu::iPHA,                          // PusH Accumulator
  &Cpu::iPHK,                          // PusH K register
  &Cpu::iJMP,                          // JuMP
  &Cpu::iBVC,                          // Branch if oVerflow Clear
  &Cpu::iMVN,                          // MoVe memory Negative
  &Cpu::iCLI,                          // CLear Interrupt disable
  &Cpu::iPHY,                          // PusH Y register
  &Cpu::iTCD,                          // Transfer C accumulator to Direct register
  &Cpu::iRTS,                          // ReTurn from Subroutine
  &Cpu::iADC,                          // ADd with Carry
  &Cpu::iPER,                          // Push Effective Relative address
  &Cpu::iSTZ,                          // STore Zero
  &Cpu::iROR,                          // ROtate Right
  &Cpu::iPLA,                          // PulL Accumulator
  &Cpu::iRTL,                          // ReTurn from subroutine Long
  &Cpu::iBVS,                          // Branch if oVerflow Set
  &Cpu::iSEI,                          // SEt Interrupt disable
  &Cpu::iPLY,                          // PulL Y register
  &Cpu::iTDC,                          // Transfer Direct register to C accumulator
  &Cpu::iBRA,                          // BRanch Always
  &Cpu::iSTA,                          // STore Accumulator
  &Cpu::iBRL,                          // BRanch Long
  &Cpu::iSTY,                          // STore Y register
  &Cpu::iSTX,                          // STore X register
  &Cpu::iDEY,                          // DEcrement Y register
  &Cpu::iTXA,                          // Transfer X register to Accumulator
  &Cpu::iPHB,                          // PusH data Bank register
  &Cpu::iBCC,                          // Branch if Carry Clear
  &Cpu::iTYA,                          // Transfer Y register to Accumulator
  &Cpu::iTXS,                          // Transfer X register to Stack pointer
  &Cpu::iTXY,                          // Transfer X register to Y register
  &Cpu::iLDY,                          // LoaD Y register
  &Cpu::iLDA,                          // LoaD Accumulator
  &Cpu::iLDX,                          // LoaD X register
  &Cpu::iTAY,                          // Transfer Accumulator to Y register
  &Cpu::iTAX,                          // Transfer Accumulator to X register
  &Cpu::iPLB,                          // PulL data Bank register
  &Cpu::iBCS,                          // Branch if Carry Set
  &Cpu::iCLV,                          // CLear oVerflow
  &Cpu::iTSX,                          // Transfer Stack pointer to X register
  &Cpu::iTYX,                          // Transfer Y register to X register
  &Cpu::iCPY,                          // ComPare to Y register
  &Cpu::iCMP,                          // CoMPare (to accumulator)
  &Cpu::iREP,                          // REset Processor status bits
  &Cpu::iINY,                          // INcrement Y register
  &Cpu::iDEX,                          // DEcrement X register
  &Cpu::iWAI,                          // WAit for Interrupt
  &Cpu::iBNE,                          // Branch if Not Equal
  &Cpu::iPEI,                          // Push Effective Indirect address
  &Cpu::iCLD,                          // CLear Decimal mode
  &Cpu::iPHX,                          // PusH X register
  &Cpu::iSTP,                          // SToP the clock
  &Cpu::iJML,                          // JuMP Long
  &Cpu::iCPX,                          // ComPare to X register
  &Cpu::iSBC,                          // SuBtract with Carry
  &Cpu::iSEP,                          // SEt Processor status bits
  &Cpu::iINX,                          // INcrement X register
  &Cpu::iNOP,                          // No OPeration
  &Cpu::iXBA,                          // eXchange B and A accumulator
  &Cpu::iBEQ,                          // Branch if EQual
  &Cpu::iPEA,                          // Push Effective Address
  &Cpu::iSED,                          // SEt Decimal mode
  &Cpu::iPLX,                          // PulL X register
  &Cpu::iXCE,                          // eXchange Carry and Emulation flags
};

Cpu::Cpu()
:
  negativeFlag(false),
  overflowFlag(false),
  sHigh(true),
  sLow(true),
  decimalFlag(false),
  interruptFlag(false),
  zeroFlag(false),
  carryFlag(false),
  breakFlag(false),
  crossedPage(false),
  cycles(0),
//  startAddr(memory),
  pc(0),
  sp(0xFF),
  a(0),
  x(0),
  y(0)
{
}

void Cpu::setPc(uint16_t counter)
{
  pc = counter;
}

// TODO (match NES reset values): Reset untested
void Cpu::reset()
{
  // memset(memory, 0, sizeof(memory));

  carryFlag = false;
  zeroFlag = true;
  interruptFlag = true;
  decimalFlag = false;
  overflowFlag = false;
  negativeFlag = false;

  // pc = &memory[0x34];
  sp = 0xFF; 

  // On reset, reference address given from reset vector memory[0xFFFD] << 8) | memory[0xFFFC]; 
  // uint16_t tempAddr = ((memory[0xFFFD] & EIGHT_BIT_MASK) << EIGHT_BIT_SHIFT);
  // pc |= (memory[0xFFFC] & EIGHT_BIT_MASK);
  pc = 0;

  a = 0;
  x = 0;
  y = 0;
  pc = 0xe831;
}

void Cpu::printStatus()
{
  printf("nf: %x\n", negativeFlag);
  printf("of: %x\n", overflowFlag);
  printf("df: %x\n", decimalFlag);
  printf("if: %x\n", interruptFlag);
  printf("zf: %x\n", zeroFlag);
  printf("cf: %x\n", carryFlag);
  printf("A: %x\n", a);
  printf("X: %x\n", x);
  printf("Y: %x\n", y);
  printf("sp: %x\n", sp);
  printf("pc: %x\n", pc);
  printf("*pc+0: %x\n", (uint8_t)*(startAddr + pc) & 0xFF);
  printf("*pc+1: %x\n", (uint8_t)*(startAddr + (pc + 1)) & 0xFF);
  printf("*pc+2: %x\n", (uint8_t)*(startAddr + (pc + 2)) & 0xFF);
  printf("\n");
}

void Cpu::printZeroPage()
{
  printf("Zero Page:\n");

  for (uint16_t x = 0; x < 0xFF; x+=0x10)
  {
    printf("%2x: ", x);
    for (uint16_t y = 0; y < 16; y++)
    {
      printf("%2x ", (uint8_t)*(startAddr + x + y) & 0xFF);
    }
    printf("\n");
  }

  printf("\n");
}

void Cpu::printStack()
{
  printf("Stack:\n");

  for (uint16_t x = 0; x < 0xFF; x+=0x10)
  {
    printf("%2x: ", (0xFF - x));

    for (uint16_t y = 0; y < 16; y++)
    {
      printf("%2x ", (uint8_t)*(startAddr + 0x1FF - x - y) & 0xFF);
    }
    printf("\n");
  }
}

uint8_t Cpu::getFlags()
{
  uint8_t value = 0;
  value |= carryFlag ? carryMask : 0;
  value |= zeroFlag ? zeroMask : 0;
  value |= interruptFlag ? interruptMask : 0;
  value |= decimalFlag ? decimalMask : 0;
  value |= breakFlag ? breakMask : 0;
  value |= overflowFlag ? overflowMask : 0;
  value |= negativeFlag ? negativeMask : 0;
  return value;
}

void Cpu::setFlags(uint8_t value)
{
  carryFlag = value & carryMask;
  zeroFlag = value & zeroMask;
  interruptFlag = value & interruptMask;
  decimalFlag = value & decimalMask;
  breakFlag = value & breakMask;
  overflowFlag = value & overflowMask;
  negativeFlag = value & negativeMask;
}

void Cpu::setMemory(Memory *memory_controller)
{
  pc = 0;
  memory = memory_controller;    // memory_callback
  startAddr = memory_controller->get_memory();
}

// TODO (Move controls out of CPU): Handle with new controls class, allow alternate controls
void Cpu::handlePlayerInput(SDL_Event *event)
{
  // SDL event is upper case, add 0x20 to change to lower case
  if (event->type == SDL_KEYDOWN)
  {
    *(startAddr + 0xFF) = (*SDL_GetKeyName(event->key.keysym.sym) + 0x20) & 0xFF;
  }
}

void Cpu::doInstruction()
{
  // do nothing until break is false or required cycles is 0
  if (cycles != 0 || breakFlag)
  {
    cycles --;
    return;
  }

  // extract the instruction operation code 
  uint8_t operationCode = 0xFF & startAddr[pc];

  // get address mode ID from instruction operation code
  uint8_t addressModeId = memory->AddressModeLookupTable[operationCode];

  // get required operation function ID from table
  uint8_t operationCodeId = (this->OperationCodeLookupTable[operationCode]);

  // get required address by using memory address function table with operation code
  uint8_t *address = (memory->*Memory::AddressModeFunctionTable[addressModeId])(startAddr + pc + 1);

  uint8_t requiredCycles = (this->TimingLookupTable[operationCode]);

  // increment pc by required bytes: 1 for opcode + 0..2 for address mode
  pc += Memory::AddressModeSizeTable[addressModeId] + 1;
  
  // randomize 0xFE
  generateRandomVar();

  // call required function ID with address
  (this->*OperationCodeFunctionTable[operationCodeId])(address);

  // cycles required for instruction (branching instructions add 1 if branch taken)
  cycles += requiredCycles % 10;
 
  // add 1 if crossed boundary and instruction requires extra cycles
  cycles += crossedPage ? (requiredCycles/10) : 0;

  if (cycles == 0xFFFF)
  {
    printf("invalid instruction");
  }
}

uint16_t Cpu::getProgramCounter()
{
  // return memory index pointed to by PC
  return pc;
}

uint8_t Cpu::getStackPointer()
{
  // return index pointed to by SP range 0x0 -> 0xFF
  // sp starts at 0x1FF, moves to lower addresses
  return sp;
}

void Cpu::setProgramCounter(uint16_t addr)
{
  // return memory index pointed to by PC
  pc = addr;
}

void Cpu::setStackPointer(uint8_t addr)
{
  // return index pointed to by SP range 0x0 -> 0xFF
  // setSp(0xff) resets to top of stack
  sp = addr;
}

uint8_t *Cpu::getAddressA()
{
  return &a;
}

uint8_t Cpu::getA()
{
  return a;
}

uint8_t Cpu::getX()
{
  return x;
}

uint8_t Cpu::getY()
{
  return y;
}

// return signed int equal to given 8-bit signed representation from 8-bit unsigned representation
uint8_t Cpu::getOnesComplement(uint8_t value)
{
  return ~value;
}

int Cpu::getSignedRepresentation(uint16_t value)
{
  int result;

  if (value > 0x8000)
  {
    // get 2's complement
    value = ~value;
    result = value + 1;

    // return as signed negative
    result *= -1;
  }
  else
  {
    result = value;
  }

  return result;
}

int Cpu::getSignedRepresentation(uint8_t value)
{
  int result;

  if (value > 0x80)
  {
    // get 2's complement
    value = ~value;
    result = value + 1;

    // return as signed negative
    result *= -1;
  }
  else
  {
    result = value;
  }

  return result;
}

// return signed int equal to given 8-bit signed representation from 8-bit unsigned representation
uint8_t Cpu::getTwosComplement(uint8_t value)
{
  int newValue = 0;

  value = ~value;
  newValue = value + 1;

  return newValue;
}

// push 8 bits onto stack and increment stack pointer
void Cpu::pushStack(uint8_t value)
{
  startAddr[0x100 + sp] = value;
  sp--;
}

// pop 8 bits from stack and decrement stack pointer
uint8_t Cpu::popStack()
{
  uint8_t value;

  sp++;
  value = *(startAddr + 0x100 + sp);

  return value;
}

// randomize 0xFE
void Cpu::generateRandomVar()
{
  *(startAddr + 0xFE) = (std::rand() % 0xFF);
}

void Cpu::incrementProgramCounter(uint16_t addressOffset)
{
  pc += addressOffset;
}

// BReaKpoint
void Cpu::iBRK(uint8_t *addr)
{
  printf("break\n");
  breakFlag = true;
}

// bitwise OR Accumulator
// Affects Flags: S Z
void Cpu::iORA(uint8_t *addr)
{
  uint8_t value = *addr;

  // accumulator = accumulator | *memoryAddr
  a |= value;

  // S flag
  negativeFlag = (a >= 0x80);

  // Z flag
  zeroFlag = (a == 0);
}

// COProcessor
void Cpu::iCOP(uint8_t *addr)
{
  // unused
}

// Test and Set Bits
void Cpu::iTSB(uint8_t *addr)
{
  // unused
}

// Arithmetic Shift Left
// Affects Flags: S Z C
void Cpu::iASL(uint8_t *addr)
{
  uint8_t temp = *addr;

  // C flag is set to bit 7 before shifting
  carryFlag = (temp >= 0x80);
  
  // shift left once
  temp <<= 1;

  // Z flag set if result was 0
  zeroFlag = (temp == 0);

  // S flag set if bit 7 of the result is set
  negativeFlag = (temp >= 0x80);
  
  *addr = temp;
}

// PusH Processor status register
// Affects Flags: none
void Cpu::iPHP(uint8_t *addr)
{
  uint8_t value = getFlags();

  // push 8 bit flags onto stack and increment pointer
  pushStack(value);
}

// PusH Direct register
void Cpu::iPHD(uint8_t *addr)
{
  // unused
}

// Branch if PLus
// Affects Flags: none
void Cpu::iBPL(uint8_t *addr)
{
  if (!negativeFlag)
  {
    // add 1 cycle since branch was taken
    cycles += 1;

    // add 1 cycle if crossing the page boundary
    if (crossedPage)
    {
      cycles += 1;
    }

    pc = (uint16_t)(addr - startAddr);
  }
}

// Test and Reset Bits
void Cpu::iTRB(uint8_t *addr)
{
  // unused
}

// CLear Carry
// Affects Flags: C
void Cpu::iCLC(uint8_t *addr)
{
  carryFlag = false;
}

// INCrement
// Affects flags S, Z
void Cpu::iINC(uint8_t *addr)
{
  uint8_t value = *addr;
  value += 1;
  *addr = value;

  // Z: value became zero
  zeroFlag = (value == 0);

  // S: value is negative
  negativeFlag = (value >= 0x80);
}

// Transfer C accumulator to Stack pointer
void Cpu::iTCS(uint8_t *addr)
{
  // unused
}

// Jump to SubRoutine
void Cpu::iJSR(uint8_t *addr)
{
  uint16_t returnAddressFull = getProgramCounter() - 1;
  uint8_t returnAddressLow = returnAddressFull & 0x00FF;
  uint8_t returnAddressHigh = (returnAddressFull & 0xFF00) >> 8;

  // RTS pulls the top two bytes off the stack (low byte first)
  pushStack(returnAddressHigh);
  pushStack(returnAddressLow);

  // jump
  pc = (uint16_t)(addr - startAddr);
}

// bitwise AND
// affects flags S, Z
void Cpu::iAND(uint8_t *addr)
{
  uint8_t value = *addr;

  // accumulator = accumulator & *memoryAddr
  a &= value;

  // Z: value became zero
  zeroFlag = (a == 0);

  // S: value became negative
  negativeFlag = (a >= 0x80);
}

// Jump to Subroutine Long
void Cpu::iJSL(uint8_t *addr)
{
  // unused
}

// test BITs
// Affects flags N V Z
void Cpu::iBIT(uint8_t *addr)
{
  uint8_t value = *addr;

  // Z: set as though the value were ANDed with the accumulator
  zeroFlag = ((value & a) == 0);

  // N: set to match bit 7 in the value stored at the tested address
  negativeFlag = (value & negativeMask);

  // V: set to match bit 6 in the value stored at the tested address
  overflowFlag = (value & overflowMask);
}

// ROtate Left
// Affects Flags: S Z C
void Cpu::iROL(uint8_t *addr)
{
  uint8_t value = *addr;
  bool prevCarry = carryFlag;

  // C: the original bit 7 is shifted into carryFlag
  carryFlag = (value & negativeMask);

  // rotate 1 bit
  value <<= 1;

  // original carry value is shifted into bit 0
  value |= (prevCarry) ? 1 : 0;

  // Z: resulting value affects zeroFlag
  zeroFlag = (value == 0);

  // S: resulting value affects signFlag
  negativeFlag = (value >= 0x80);

  *addr = value;
}

// PulL Processor status register
// Affects all flags
void Cpu::iPLP(uint8_t *addr)
{
  uint8_t value = popStack();

  // push 8 bit flags onto stack and increment pointer
  setFlags(value);
}

// PulL Direct register
void Cpu::iPLD(uint8_t *addr)
{
  // unused
}

// Branch if MInus
// Affects flags: none
void Cpu::iBMI(uint8_t *addr)
{
  if (negativeFlag)
  {
    // add 1 cycle since branch was taken
    cycles += 1;

    // add 1 cycle if crossing the page boundary
    if (crossedPage)
    {
      cycles += 1;
    }

    pc = (uint16_t)(addr - startAddr);
  }
}

// SEt Carry
// Affects flags C
void Cpu::iSEC(uint8_t *addr)
{
  carryFlag = true;
}

// DECrement
// Affects Flags: S Z
void Cpu::iDEC(uint8_t *addr)
{
  uint8_t value = *addr;
  
  value -= 1;
  *addr = value;

  // Z: value became zero
  zeroFlag = (value == 0);

  // S: value is negative
  negativeFlag = (value >= 0x80);
}

// Transfer Stack pointer to C accumulator
void Cpu::iTSC(uint8_t *addr)
{
  // unused
}

// ReTurn from Interrupt
// retrieves the Processor Status Word (flags), then Program Counter from the stack
void Cpu::iRTI(uint8_t *addr)
{
  // get flags from register
  setFlags(popStack());

  // get program counter from stack, low byte first
  uint16_t newPC = popStack();
  newPC |= (popStack() << 8);
  setProgramCounter(newPC);
}

// bitwise exclusive OR
// Affects Flags: S Z
void Cpu::iEOR(uint8_t *addr)
{
  uint8_t value = *addr;

  // accumulator = accumulator ^ *memoryAddr
  a ^= value;

  // S: a became negative
  negativeFlag = (a >= 0x80);

  // Z: a became zero
  zeroFlag = (a == 0);
}

// William D. Mensch, Jr. (2-byte, 2-cycle NOP)
void Cpu::iWDM(uint8_t *addr)
{
  // unused
}

// MoVe memory Positive
void Cpu::iMVP(uint8_t *addr)
{
  // unused
}

// Logical Shift Right
// Affects Flags: S Z C
void Cpu::iLSR(uint8_t *addr)
{
  uint8_t value = *addr;

  // C: original bit 0 is shifted into the Carry
  carryFlag = (value & 1);

  value >>= 1;

  // Z: value became zero
  zeroFlag = (value == 0);

  // S: value became negative (never?)
  negativeFlag = (value >= 0x80);

  *addr = value;
}

// PusH Accumulator
// Affects Flags: none
void Cpu::iPHA(uint8_t *addr)
{
  // push accumulator
  pushStack(a);
}

// PusH K register
void Cpu::iPHK(uint8_t *addr)
{
  // unused
}

// JuMP
// Affects Flags: none
void Cpu::iJMP(uint8_t *addr)
{
  // absolute address is in 2 byte following instruction in reverse byte order
  pc = (uint16_t)(addr - startAddr);
}

// Branch if oVerflow Clear
void Cpu::iBVC(uint8_t *addr)
{
  if (!overflowFlag)
  {
    // add 1 cycle since branch was taken
    cycles += 1;

    // add 1 cycle if crossing the page boundary
    if (crossedPage)
    {
      cycles += 1;
    }

    pc = (uint16_t)(addr - startAddr);
  }
}

// MoVe memory Negative
void Cpu::iMVN(uint8_t *addr)
{
  // unused
}

// CLear Interrupt disable
void Cpu::iCLI(uint8_t *addr)
{
  interruptFlag = false;
}

// PusH Y register
void Cpu::iPHY(uint8_t *addr)
{
  // unused
}

// Transfer C accumulator to Direct register
void Cpu::iTCD(uint8_t *addr)
{
  // unused
}

// ReTurn from Subroutine
void Cpu::iRTS(uint8_t *addr)
{
  uint8_t returnAddressLow;
  uint8_t returnAddressHigh;
  uint16_t returnAddressFull;

  // RTS pulls the top two bytes off the stack (low byte first)
  returnAddressLow = popStack();
  returnAddressHigh = popStack();
  returnAddressFull = returnAddressLow | (returnAddressHigh << 8);

  pc = returnAddressFull + 1;
}


// A        B         RESULT                V
// 0x7F     0x7F      (127 + 127 = -1)      T
// <0x80    <0x80     >0x80                 T
// >=0x80   >=0x80    < 0x80                T
//
// 0xF0     0xF0      (-128 - 128 = -1)     T
// 0xF0     0xF0      (-128 - 128 = -1)     T
// >90      >90

// ADd with Carry
// Affects Flags: S V Z C
void Cpu::iADC(uint8_t *addr)
{
  uint8_t value = *addr;
  uint16_t carryTestValue = value + a;
  uint8_t result = value + a;

  // add 1 if carry was set
  result += (carryFlag) ? 1 : 0;
  carryTestValue += (carryFlag) ? 1 : 0;

  // V: check if overflow - adding positives equals negative OR adding negatives equals positive
  if ((value >= 0x80 && a >= 0x80 && result < 0x80)
      || (value < 0x80 && a < 0x80 && result >= 0x80))
  {
    overflowFlag = true;
  }
  else
  {
    overflowFlag = false;
  }

  // do addition
  a = result;

  // C: check if carry
  carryFlag = (carryTestValue >= 0x100);

  // S: check if negative
  negativeFlag = (a >= 0x80);

  // Z: zero flag
  zeroFlag = (result == 0);
}

// Push Effective Relative address
void Cpu::iPER(uint8_t *addr)
{
  // unused
}

// STore Zero
void Cpu::iSTZ(uint8_t *addr)
{
  // unused
}

// ROtate Right
// Affects Flags: S Z C
void Cpu::iROR(uint8_t *addr)
{
  uint8_t value = *addr;
  bool newCarryFlag = (value & 1);

  // ROR shifts all bits right one position
  value >>= 1;

  // Carry is shifted into bit 7
  if (carryFlag)
  {
    value |= 0x80;
  }

  // C: original bit 0 is shifted into the Carry
  carryFlag = newCarryFlag;

  // Z: result was zero
  zeroFlag = (value == 0);

  // S: result was negative
  negativeFlag = (value >= 0x80);

  *addr = value;
}

// PulL Accumulator
// Affects Flags: S Z
void Cpu::iPLA(uint8_t *addr)
{
  a = popStack();

  // Z: result was zero
  zeroFlag = (a == 0);

  // S: result was negative
  negativeFlag = (a >= 0x80);
}

// ReTurn from subroutine Long
void Cpu::iRTL(uint8_t *addr)
{
  // unused
}

// Branch if oVerflow Set
void Cpu::iBVS(uint8_t *addr)
{
  if (overflowFlag)
  {
    // add 1 cycle since branch was taken
    cycles += 1;

    // add 1 cycle if crossing the page boundary
    if (crossedPage)
    {
      cycles += 1;
    }

    pc = (uint16_t)(addr - startAddr);
  }
}

// SEt Interrupt disable
void Cpu::iSEI(uint8_t *addr)
{
  interruptFlag = true;
}

// PulL Y register
void Cpu::iPLY(uint8_t *addr)
{
  // unused
}

// Transfer Direct register to C accumulator
void Cpu::iTDC(uint8_t *addr)
{
  // unused
}

// BRanch Always
void Cpu::iBRA(uint8_t *addr)
{
  // unused
}

// STore Accumulator
// Affects Flags: none
void Cpu::iSTA(uint8_t *addr)
{
  uint8_t value = a;

  *addr = value;
}

// BRanch Long
void Cpu::iBRL(uint8_t *addr)
{
  // unused
}

// STore Y register
// Affects Flags: none
void Cpu::iSTY(uint8_t *addr)
{
  uint8_t value = y;

  *addr = value;
}

// STore X register
void Cpu::iSTX(uint8_t *addr)
{
  uint8_t value = x;

  *addr = value;
}

// DEcrement Y register
// Affect Flags: S Z
void Cpu::iDEY(uint8_t *addr)
{
  y--;
  
  zeroFlag = (y == 0);
}

// Transfer X register to Accumulator
// Affect Flags: S Z
void Cpu::iTXA(uint8_t *addr)
{
  a = x;

  // Z: 
  zeroFlag = (a == 0);

  // S: 
  negativeFlag = (a >= 0x80);
}

// PusH data Bank register
void Cpu::iPHB(uint8_t *addr)
{
  // unused
}

// Branch if Carry Clear
void Cpu::iBCC(uint8_t *addr)
{
  if (!carryFlag)
  {
    // add 1 cycle since branch was taken
    cycles += 1;

    // add 1 cycle if crossing the page boundary
    if (crossedPage)
    {
      cycles += 1;
    }

    pc = (uint16_t)(addr - startAddr);
  }
}

// Transfer Y register to Accumulator
// Affect Flags: S Z
void Cpu::iTYA(uint8_t *addr)
{
  a = y;

  // Z: 
  zeroFlag = (a == 0);

  // S: 
  negativeFlag = (a >= 0x80);
}

// Transfer X register to Stack pointer
// Affect Flags: S Z
void Cpu::iTXS(uint8_t *addr)
{
  *(startAddr + 0x100 + sp) = x;

  // Z: 
  zeroFlag = (x == 0);

  // S: 
  negativeFlag = (x >= 0x80);
}

// Transfer X register to Y register
void Cpu::iTXY(uint8_t *addr)
{
  // unused
}

// LoaD Y register
// Affects Flags: S Z
void Cpu::iLDY(uint8_t *addr)
{
  uint8_t value = *addr;

  y = value;

  // loaded zero
  zeroFlag = (y == 0);

  // loaded negative value
  negativeFlag = (y >= 0x80);
}

// LoaD Accumulator
// Affects Flags: S Z
void Cpu::iLDA(uint8_t *addr)
{
  uint8_t value = *addr;

  // a = value at *memoryAddr
  a = value;

  // Z: loaded zero
  zeroFlag = (a == 0);

  // S: loaded negative
  negativeFlag = (a >= 0x80);
}

// LoaD X register
// Affects Flags: S Z
void Cpu::iLDX(uint8_t *addr)
{
  uint8_t value = *addr;

  x = value;

  // Z: loaded zero
  zeroFlag = (x == 0);

  // S: loaded negative
  negativeFlag = (x >= 0x80);
}

// Transfer Accumulator to Y register
// Affects Flags: S Z
void Cpu::iTAY(uint8_t *addr)
{
  uint8_t value = a;

  y = value;

  // Z: transfer zero
  zeroFlag = (y == 0);

  // S: transfer negative
  negativeFlag = (y >= 0x80);
}

// Transfer Accumulator to X register
// Affects Flags: S Z
void Cpu::iTAX(uint8_t *addr)
{
  uint8_t value = a;

  x = value;

  // Z: transfer zero
  zeroFlag = (x == 0);

  // S: transfer negative
  negativeFlag = (x >= 0x80);
}

// PulL data Bank register
void Cpu::iPLB(uint8_t *addr)
{
  // unused
}

// Branch if Carry Set
void Cpu::iBCS(uint8_t *addr)
{
  if (carryFlag)
  {
    // add 1 cycle since branch was taken
    cycles += 1;

    // add 1 cycle if crossing the page boundary
    if (crossedPage)
    {
      cycles += 1;
    }

    pc = (uint16_t)(addr - startAddr);
  }
}

// CLear oVerflow
void Cpu::iCLV(uint8_t *addr)
{
  overflowFlag = false;
}

// Transfer Stack pointer to X register
// Affect Flags: S Z
void Cpu::iTSX(uint8_t *addr)
{
  x = *(startAddr + 0x100 + sp);

  // Z: 
  zeroFlag = (x == 0);

  // S: 
  negativeFlag = (x >= 0x80);
}

// Transfer Y register to X register
void Cpu::iTYX(uint8_t *addr)
{
  // unused
}

// ComPare to Y register
// Affects Flags: S Z C
void Cpu::iCPY(uint8_t *addr)
{
  uint8_t result;
  uint8_t value = *addr;
  uint16_t carryTest = y;
  
  // Z:
  zeroFlag = (y == value);

  value = getTwosComplement(value);
  carryTest += value;
  result = value + y;

  // C:
  carryFlag = (carryTest >= 0x100);

  // N:
  negativeFlag = (result >= 0x80);
}

// CoMPare (to accumulator)
// Affects Flags: S Z C
void Cpu::iCMP(uint8_t *addr)
{
  uint8_t result;
  uint8_t value = *addr;
  uint16_t carryTest = a;
  
  // Z:
  zeroFlag = (a == value);

  value = getTwosComplement(value);
  carryTest += value;
  result = value + a;

  // C:
  carryFlag = (carryTest >= 0x100);

  // N:
  negativeFlag = (result >= 0x80);
}

// REset Processor status bits
void Cpu::iREP(uint8_t *addr)
{
  // unused
}

// INcrement Y register
// Affect Flags: S Z
void Cpu::iINY(uint8_t *addr)
{
  y += 1;
  
  // S: increment to negative
  negativeFlag = (y >= 0x80);

  // Z: increment to zero
  zeroFlag = (y == 0);
}

// DEcrement X register
// Affects Flags: S Z 
void Cpu::iDEX(uint8_t *addr)
{
  x -= 1;
  
  // S: increment to negative
  negativeFlag = (x >= 0x80);

  // Z: increment to zero
  zeroFlag = (x == 0);
}

// WAit for Interrupt
void Cpu::iWAI(uint8_t *addr)
{
  // unused
}

// Branch if Not Equal
void Cpu::iBNE(uint8_t *addr)
{
  if (!zeroFlag)
  {
    // add 1 cycle since branch was taken
    cycles += 1;

    // add 1 cycle if crossing the page boundary
    if (crossedPage)
    {
      cycles += 1;
    }

    pc = (uint16_t)(addr - startAddr);
  }
}

// Push Effective Indirect address
void Cpu::iPEI(uint8_t *addr)
{
  // unused
}

// CLear Decimal mode
void Cpu::iCLD(uint8_t *addr)
{
  decimalFlag = false;
}

// PusH X register
void Cpu::iPHX(uint8_t *addr)
{
  // unused
}

// SToP the clock
void Cpu::iSTP(uint8_t *addr)
{
  // unused
}

// JuMP Long
void Cpu::iJML(uint8_t *addr)
{
  // unused
}

// ComPare to X register
// Affects Flags: S Z C
void Cpu::iCPX(uint8_t *addr)
{
  uint8_t result;
  uint8_t value = *addr;
  uint16_t carryTest = x;
  
  // Z:
  zeroFlag = (x == value);

  value = getTwosComplement(value);
  carryTest += value;
  result = value + x;

  // C:
  carryFlag = (carryTest >= 0x100);

  // N:
  negativeFlag = (result >= 0x80);
}

// SBC, starting with C set:
// 1st     2nd    result      N        V        C
// -----------------------------------------------
// F8      0A       EE        1        0        1
// 81       7       7A        0        1        1
//  7       2        5        0        0        1
//  7      -2        9        0        0        0
//  7       9       FE        1        0        0
//  7      90       77        0        0        0
// 10      90       80        1        1        0
// 10      91       7F        0        0        0

// SuBtract with Carry
// Affects Flags: S V Z C
void Cpu::iSBC(uint8_t *addr)
{
  uint8_t value = *addr;

  // get the 1's complement of the value being subtracted and add
  value = getOnesComplement(value);

  iADC(&value);
}

// SEt Processor status bits
void Cpu::iSEP(uint8_t *addr)
{
  // unused
}

// INcrement X register
// Affect Flags: S Z
void Cpu::iINX(uint8_t *addr)
{
  x++;

  // Z: increment to zero
  zeroFlag = (x == 0);

  // S: increment to negative
  negativeFlag = (x >= 0x80);
}

// No OPeration
void Cpu::iNOP(uint8_t *addr)
{
  // do nothing
}

// eXchange B and A accumulator
void Cpu::iXBA(uint8_t *addr)
{
  // unused
}

// Branch if EQual
void Cpu::iBEQ(uint8_t *addr)
{
  if (zeroFlag)
  {
    // add 1 cycle since branch was taken
    cycles += 1;

    // add 1 cycle if crossing the page boundary
    if (crossedPage)
    {
      cycles += 1;
    }

    pc = (uint16_t)(addr - startAddr);
  }
}

// Push Effective Address
void Cpu::iPEA(uint8_t *addr)
{
  // unused
}

// SEt Decimal mode
void Cpu::iSED(uint8_t *addr)
{
  decimalFlag = true;
}

// PulL X register
void Cpu::iPLX(uint8_t *addr)
{
  // unused
}

// eXchange Carry and Emulation flags
void Cpu::iXCE(uint8_t *addr)
{
  // unused
}

