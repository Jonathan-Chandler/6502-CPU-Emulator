#include "Cpu.h"
#include <string.h>

#define EIGHT_BIT_MASK  0xFF
#define EIGHT_BIT_SHIFT 8

enum AddressModesEnum
{
// name                             desc                          symbol
  None,                       // No address/value
  Immediate,                  // immediate value:                 #$
  DirectZeroX,                // direct zero addr + X:            d, X
  DirectZeroY,                // direct zero addr + Y:            d, Y
  DirectZeroZ,                // direct zero addr:                d
  DirectAbsoluteX,            // absolute address + X:            a, X
  DirectAbsoluteY,            // absolute address + Y:            a, Y
  DirectAbsoluteZ,            // absolute address:                a
  IndirectZeroX,              // indirect zero page address + X   (d, X)
  IndirectZeroZ,              // indirect zero page address       (d)
  IndirectZeroIndexY,         // indirect zero page ddress[Y]     (d), Y
  IndirectAbsoluteX,          // indirect absolute address + x    (a, X)
  IndirectAbsoluteZ,          // indirect absolute address        (a)
  ImplementedCount,           // No address used
  A,                          // No address used
  al,                         // No address used
  alX,                        // No address used
  b,                          // No address used
  r,                          // No address used
  rl,                         // No address used
  pdSpY,                      // No address used
  dS,                         // No address used
  sd,                         // No address used
  bdb,                        // No address used
  bdby,                       // No address used
  AddressModeCount,           // No address used
};                            

enum OperationEnum
{
// name                             desc                  
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

const uint8_t Cpu::TimingLookupTable[] = 
{
  b,                IndirectZeroX,        b,                dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          A,      None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroZ,      DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    A,      None,   DirectAbsoluteZ,    DirectAbsoluteX,  DirectAbsoluteX,  alX,
  DirectAbsoluteZ,  IndirectZeroX,        al,               dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          A,      None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroX,      DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    A,      None,   DirectAbsoluteX,    DirectAbsoluteX,  DirectAbsoluteX,  alX,
  None,             IndirectZeroX,        None,             dS,       sd,               DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          A,      None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    sd,               DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    None,   None,   al,                 DirectAbsoluteX,  DirectAbsoluteX,  alX,
  None,             IndirectZeroX,        rl,               dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          A,      None,   IndirectAbsoluteZ,  DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroX,      DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    None,   None,   IndirectAbsoluteX,  DirectAbsoluteX,  DirectAbsoluteX,  alX,
  r,                IndirectZeroX,        rl,               dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          None,   None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroX,      DirectZeroX,  DirectZeroY,    bdby,   None,   DirectAbsoluteY,    None,   None,   DirectAbsoluteZ,    DirectAbsoluteX,  DirectAbsoluteX,  alX,
  Immediate,        IndirectZeroX,        Immediate,        dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          None,   None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroX,      DirectZeroX,  DirectZeroY,    bdby,   None,   DirectAbsoluteY,    None,   None,   DirectAbsoluteX,    DirectAbsoluteX,  DirectAbsoluteY,  alX,
  Immediate,        IndirectZeroX,        Immediate,        dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          None,   None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroZ,      DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    None,   None,   IndirectAbsoluteZ,  DirectAbsoluteX,  DirectAbsoluteX,  alX,
  Immediate,        IndirectZeroX,        Immediate,        dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          None,   None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectAbsoluteZ,  DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    None,   None,   IndirectAbsoluteX,  DirectAbsoluteX,  DirectAbsoluteX,  alX,
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

const uint8_t Cpu::AddressModeLookupTable[] = 
{
  b,                IndirectZeroX,        b,                dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          A,      None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroZ,      DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    A,      None,   DirectAbsoluteZ,    DirectAbsoluteX,  DirectAbsoluteX,  alX,
  DirectAbsoluteZ,  IndirectZeroX,        al,               dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          A,      None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroX,      DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    A,      None,   DirectAbsoluteX,    DirectAbsoluteX,  DirectAbsoluteX,  alX,
  None,             IndirectZeroX,        None,             dS,       sd,               DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          A,      None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    sd,               DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    None,   None,   al,                 DirectAbsoluteX,  DirectAbsoluteX,  alX,
  None,             IndirectZeroX,        rl,               dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          A,      None,   IndirectAbsoluteZ,  DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroX,      DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    None,   None,   IndirectAbsoluteX,  DirectAbsoluteX,  DirectAbsoluteX,  alX,
  r,                IndirectZeroX,        rl,               dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          None,   None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroX,      DirectZeroX,  DirectZeroY,    bdby,   None,   DirectAbsoluteY,    None,   None,   DirectAbsoluteZ,    DirectAbsoluteX,  DirectAbsoluteX,  alX,
  Immediate,        IndirectZeroX,        Immediate,        dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          None,   None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroX,      DirectZeroX,  DirectZeroY,    bdby,   None,   DirectAbsoluteY,    None,   None,   DirectAbsoluteX,    DirectAbsoluteX,  DirectAbsoluteY,  alX,
  Immediate,        IndirectZeroX,        Immediate,        dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          None,   None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectZeroZ,      DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    None,   None,   IndirectAbsoluteZ,  DirectAbsoluteX,  DirectAbsoluteX,  alX,
  Immediate,        IndirectZeroX,        Immediate,        dS,       DirectZeroZ,      DirectZeroZ,  DirectZeroZ,    bdb,    None,   Immediate,          None,   None,   DirectAbsoluteZ,    DirectAbsoluteZ,  DirectAbsoluteZ,  al,
  r,                IndirectZeroIndexY,   IndirectZeroZ,    pdSpY,    DirectAbsoluteZ,  DirectZeroX,  DirectZeroX,    bdby,   None,   DirectAbsoluteY,    None,   None,   IndirectAbsoluteX,  DirectAbsoluteX,  DirectAbsoluteX,  alX,
};

const uint8_t Cpu::OperationCodeLookupTable[] = 
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

const Cpu::AddressMode_T Cpu::AddressModeFunctionTable[] = 
{
//        name                                    desc                          symbol
  &Cpu::AddressNone,                        // No address/value
  &Cpu::AddressImmediate,                   // immediate value:                 #$
  &Cpu::AddressDirectZeroX,                 // direct zero addr + X:            d, X
  &Cpu::AddressDirectZeroY,                 // direct zero addr + Y:            d, Y
  &Cpu::AddressDirectZeroZ,                 // direct zero addr:                d
  &Cpu::AddressDirectAbsX,                  // absolute address + X:            a, X
  &Cpu::AddressDirectAbsY,                  // absolute address + Y:            a, Y
  &Cpu::AddressDirectAbsZ,                  // absolute address:                a
  &Cpu::AddressIndirectZeroX,               // indirect zero page address + X   (d, X)
  &Cpu::AddressIndirectZeroZ,               // indirect zero page address       (d)
  &Cpu::AddressIndirectZeroIndexY,          // indirect zero page ddress[Y]     (d), Y
  &Cpu::AddressIndirectAbsX,                // indirect absolute address + x    (a, X)
  &Cpu::AddressIndirectAbsZ,                // indirect absolute address        (a)
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

// {
//   b,            IndirZeroX,        b,             dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  A,      None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//   r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectZ,     ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         A,      None,   AbsoluteZ,  AbsoluteX,   AbsoluteX,   alX,
//   AbsoluteZ,    IndirZeroX,        al,            dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  A,      None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//   r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectX,     ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         A,      None,   AbsoluteX,  AbsoluteX,   AbsoluteX,   alX,
//   None,         IndirZeroX,        None,          dS,      Sd,              ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  A,      None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//   r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   Sd,              ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         None,   None,   al,         AbsoluteX,   AbsoluteX,   alX,
//   None,         IndirZeroX,        rl,            dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  A,      None,   pap,        AbsoluteZ,   AbsoluteZ,   al,
//   r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectX,     ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         None,   None,   paXp,       AbsoluteX,   AbsoluteX,   alX,
//   r,            IndirZeroX,        rl,            dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  None,   None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//   r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectX,     ZeroDirectX,    ZeroDirectY,  None,   None,   aY,         None,   None,   AbsoluteZ,  AbsoluteX,   AbsoluteX,   alX,
//   Immediate,    IndirZeroX,        Immediate,     dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  None,   None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//   r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectX,     ZeroDirectX,    ZeroDirectY,  None,   None,   aY,         None,   None,   AbsoluteX,  AbsoluteX,   AbsoluteY,   alX,
//   Immediate,    IndirZeroX,        Immediate,     dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  None,   None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//   r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   ZeroDirectZ,     ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         None,   None,   pap,        AbsoluteX,   AbsoluteX,   alX,
//   Immediate,    IndirZeroX,        Immediate,     dS,      ZeroDirectZ,     ZeroDirectZ,    ZeroDirectZ,  None,   None,   Immediate,  None,   None,   AbsoluteZ,  AbsoluteZ,   AbsoluteZ,   al,
//   r,            IndirZeroIndexY,   IndirZeroZ,    pdSpY,   AbsoluteZ,       ZeroDirectX,    ZeroDirectX,  None,   None,   aY,         None,   None,   paXp,       AbsoluteX,   AbsoluteX,   alX,
// };

Cpu::Cpu()
:
  carryFlag(false),
  zeroFlag(true),
  interruptFlag(true),
  decimalFlag(false),
  overflowFlag(false),
  negativeFlag(false),
  pc(&memory[0x34]),
  sp(&memory[0xFD]),
  a(0),
  x(0),
  y(0)
{
  memset(memory, 0, sizeof(memory));
  memset(memory, 0xFF, 0x2000);
}

void Cpu::reset()
{
  memset(memory, 0, sizeof(memory));
  memset(memory, 0xFF, 0x2000);

  carryFlag = false;
  zeroFlag = true;
  interruptFlag = true;
  decimalFlag = false;
  overflowFlag = false;
  negativeFlag = false;

  // TODO: FD 34 or 1ff etc
  // sp = &memory[0xFD];
  // pc = &memory[0x34];
  sp = &memory[0x1FF];    

  // On reset, reference address given from &memory[(memory[0xFFFD] << 8) | (memory[0xFFFC])]; 
  uint16_t tempAddr = ((memory[0xFFFD] & EIGHT_BIT_MASK) << EIGHT_BIT_SHIFT);
  tempAddr |= (memory[0xFFFC] & EIGHT_BIT_MASK);
//  pc = &memory[(memory[tempAddr])];
  pc = &memory[tempAddr];

  a = 0;
  x = 0;
  y = 0;
}

uint8_t Cpu::getFlags()
{
  uint8_t value = 0;
  value |= carryFlag ? carryMask : 0;
  value |= zeroFlag ? zeroMask : 0;
  value |= interruptFlag ? interruptMask : 0;
  value |= decimalFlag ? decimalMask : 0;
  value |= breakFlag ? breakMask : 0;
  value |= overflowFlag ? zeroMask : 0;
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

void Cpu::doInstruction(uint8_t *instructionAddr)
{
  // extract the instruction operation code
  uint8_t operationCode = *instructionAddr & 0xFF;

  // get address mode ID from instruction operation code
  uint8_t addressModeId = AddressModeLookupTable[operationCode];
  // get required operation function ID from table
  uint8_t operationCodeId = (this->OperationCodeLookupTable[operationCode]);

  // get required address by using memory address function table with operation code
  uint8_t *address = (this->*AddressModeFunctionTable[addressModeId])(instructionAddr);
  
  // call required function ID with address
  (this->*OperationCodeFunctionTable[operationCodeId])(address);
}

uint16_t Cpu::getProgramCounter()
{
  // return memory index pointed to by PC
  return pc - &memory[0];
}

uint8_t Cpu::getStackPointer()
{
  // return index pointed to by SP range 0x0 -> 0xFF
  // sp starts at 0x1FF, moves to lower addresses
  return sp - &memory[0x0100];
}

void Cpu::setProgramCounter(uint16_t addr)
{
  // return memory index pointed to by PC
  pc = &memory[addr];
  return;
}

void Cpu::setStackPointer(uint8_t addr)
{
  // return index pointed to by SP range 0x0 -> 0xFF
  // setSp(0xff) resets to top of stack
  sp = &memory[0x0100 + addr];
  return;
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

// return signed int equal to given 8-bit signed representation from 8-bit unsigned representation
int Cpu::getTwosComplement(uint8_t value)
{
  int newValue = 0;

  // test sign bit: 1000 0000
  if (value & 0x80)
  {
    // negative value
    value = ~value;
    newValue = value + 1;
  }
  else
  {
    newValue = value;
  }

  return newValue;
}

// push 8 bits onto stack and increment stack pointer
void Cpu::pushStack(uint8_t value)
{
  *sp = value;
  sp--;
}

// pop 8 bits from stack and decrement stack pointer
uint8_t Cpu::popStack()
{
  uint8_t value;
  value = *sp;
  sp++;
  return value;
}

// No address/value
uint8_t *Cpu::AddressNone(uint8_t *instructionAddr)
{
  return nullptr;
}

// immediate value: #$
uint8_t *Cpu::AddressImmediate(uint8_t *instructionAddr)
{
  uint8_t *value = nullptr;

  // value is byte following instructionAddr
  value = (instructionAddr+1);

  printf("Address mode #immediate -> *%p = %x\n", value, *value);
  return value;
}

// direct zero addr + X: d, X
uint8_t *Cpu::AddressDirectZeroX(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = 0;

  // zero page address is in byte following instruction, add X
  zeroPageAddr = *(instructionAddr+1) + x;

  tempAddr = &memory[zeroPageAddr];

  printf("Address mode direct ZeroPage, X -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// direct zero addr + Y: d, Y
uint8_t *Cpu::AddressDirectZeroY(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = 0;

  // zero page address is in byte following instruction, add y
  zeroPageAddr = *(instructionAddr+1) + y;

  tempAddr = &memory[zeroPageAddr];

  printf("Address mode direct ZeroPage, Y -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// direct zero addr: d
uint8_t *Cpu::AddressDirectZeroZ(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;

  // low byte of zero address follows instruction byte
  // use the low byte as index into memory to get the address of the zero page
  tempAddr = &memory[*(instructionAddr + 1)];

  printf("Address mode ZeroPage direct -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// absolute address + X: a, X
uint8_t *Cpu::AddressDirectAbsX(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t absoluteAddr = 0;

  // absolute address is in 2 byte following instruction in reverse byte order
  absoluteAddr = (*(instructionAddr+2) & 0xFF) << 8;
  absoluteAddr |= *(instructionAddr+1) & 0xFF;
  absoluteAddr += x;

  // memory address of given absolute address + y register
  tempAddr = &memory[absoluteAddr];

  printf("Address mode AbsoluteAddr, Y -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// absolute address + Y: a, Y
uint8_t *Cpu::AddressDirectAbsY(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t absoluteAddr = 0;

  // absolute address is in 2 byte following instruction in reverse byte order
  absoluteAddr = (*(instructionAddr+2) & 0xFF) << 8;
  absoluteAddr |= *(instructionAddr+1) & 0xFF;
  absoluteAddr += y;

  // memory address of given absolute address + y register
  tempAddr = &memory[absoluteAddr];

  printf("Address mode AbsoluteAddr, Y -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// absolute address: a
uint8_t *Cpu::AddressDirectAbsZ(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t memoryAddress = 0;

  // address follows instruction byte, byte order is reversed
  memoryAddress = *(instructionAddr+2) & 0xFF << 8;
  memoryAddress |= *(instructionAddr+1) & 0xFF;

  // use this as index into memory to get the absolute address
  tempAddr = &memory[memoryAddress];

  printf("Address mode absolute -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// indirect zero page address + X: (d, X)
uint8_t *Cpu::AddressIndirectZeroX(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = 0;
  uint16_t memoryAddress = 0;

  // zero page address follows instruction byte, add X
  zeroPageAddr = *(instructionAddr + 1);
  zeroPageAddr += x;

  // get the address given at this location on the zero page, in reverse byte order
  memoryAddress = (memory[zeroPageAddr + 1] & 0xFF) << 8;
  memoryAddress = memory[zeroPageAddr] & 0xFF;

  // return address of the dereferenced address
  tempAddr = &memory[memoryAddress];
  
  printf("Address mode (ZeroPage, X) -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// indirect zero page address: (d)
uint8_t *Cpu::AddressIndirectZeroZ(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint8_t zeroPageAddr = 0;
  uint16_t memoryAddress = 0;

  // zero page address follows instruction byte
  zeroPageAddr = *(instructionAddr + 1);

  // get the address given at this location on the zero page, in reverse byte order
  memoryAddress = (memory[zeroPageAddr + 1] & 0xFF) << 8;
  memoryAddress = memory[zeroPageAddr] & 0xFF;

  // return address of the dereferenced address
  tempAddr = &memory[memoryAddress];
  
  printf("Address mode (ZeroPage) -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// indirect zero page address[Y]: (d), Y
uint8_t *Cpu::AddressIndirectZeroIndexY(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t zeroPageDereferencedAddr = 0;
  
  // dereference value at given zero page address, read 2 bytes in reverse byte order
  zeroPageDereferencedAddr = (memory[*(instructionAddr+1) + 1] & 0xFF) << 8;
  zeroPageDereferencedAddr |= memory[*(instructionAddr+1)] & 0xFF;

  // increment Y bytes after dereferencing the zero page's address
  tempAddr = &memory[zeroPageDereferencedAddr + y];

  printf("Address mode indexed (d), Y -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}
  
// indirect absolute address + x: (a, X)
uint8_t *Cpu::AddressIndirectAbsX(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t absoluteAddr = 0;

  // absolute address is in 2 byte following instruction in reverse byte order
  absoluteAddr = (*(instructionAddr+2) & 0xFF) << 8;
  absoluteAddr |= *(instructionAddr+1) & 0xFF;
  absoluteAddr += x;

  // memory address of given absolute address + x register
  tempAddr = &memory[absoluteAddr];

  printf("Address mode indirect AbsoluteAddr, X -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// indirect absolute address: (a)
uint8_t *Cpu::AddressIndirectAbsZ(uint8_t *instructionAddr)
{
  uint8_t *tempAddr = nullptr;
  uint16_t absoluteAddr = 0;

  // absolute address is in 2 byte following instruction in reverse byte order
  absoluteAddr = (*(instructionAddr+2) & 0xFF) << 8;
  absoluteAddr |= *(instructionAddr+1) & 0xFF;

  // memory address of given absolute address + x register
  tempAddr = &memory[absoluteAddr];

  printf("Address mode indirect AbsoluteAddr -> *%p = %x\n", tempAddr, *tempAddr);
  return tempAddr;
}

// BReaKpoint
void Cpu::iBRK(uint8_t *addr)
{
  printf("break\n");
}

// bitwise OR Accumulator
void Cpu::iORA(uint8_t *addr)
{
  uint8_t value = *addr;
  printf("ORA: %x |= %x", a, value);

  // accumulator = accumulator | *memoryAddr
  a |= value;

  printf(" = %x\n", a);
  return;
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
void Cpu::iASL(uint8_t *addr)
{
  uint8_t temp = *addr;
  printf("*addr = %x", temp);

  // bit 7 shifts into carry
  if (temp & 0x80)
    carryFlag = true;
  else
    carryFlag = false;
  
  // shift left once
  temp <<= 1;
  *addr = temp;

  printf(" ASL: %x carry: %x\n", temp, carryFlag);
}

// PusH Processor status register
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
void Cpu::iBPL(uint8_t *addr)
{
  // TODO - branch
  // branch, otherwise continue to next instruction
//  if (!negativeFlag)
//    pc = &memory[*addr];
}

// Test and Reset Bits
void Cpu::iTRB(uint8_t *addr)
{
  // unused
  
}

// CLear Carry
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

  // value became zero (incremented from 0xFF)
  if (value == 0)
  {
    zeroFlag = true;
  }
}

// Transfer C accumulator to Stack pointer
void Cpu::iTCS(uint8_t *addr)
{
  // unused
}

// Jump to SubRoutine
void Cpu::iJSR(uint8_t *addr)
{
  // TODO - branch
}

// bitwise AND
// affects flags S, Z
void Cpu::iAND(uint8_t *addr)
{
  uint8_t value = *addr;
  printf("AND: %x &= %x", a, value);

  // accumulator = accumulator & *memoryAddr
  a &= value;

  // Z: value became zero
  if (a == 0)
  {
    zeroFlag = true;
  }

  // S: value became negative
  if (getTwosComplement(a) < 0)
  {
    negativeFlag = true;
  }

  printf(" = %x\n", a);
  return;
}

// Jump to Subroutine Long
void Cpu::iJSL(uint8_t *addr)
{
}

// test BITs
// Affects flags N V Z
void Cpu::iBIT(uint8_t *addr)
{
  uint8_t value = *addr;

  // Z set as though the value were ANDed with the accumulator
  if ((value & a) == 0)
  {
    zeroFlag = true;
  }

  // S set to match bit 7 in the value stored at the tested address
  negativeFlag = (value & negativeMask);

  // V set to match bit 6 in the value stored at the tested address
  overflowFlag = (value & overflowMask);
}

// ROtate Left
// Affects Flags: S Z C
void Cpu::iROL(uint8_t *addr)
{
  uint8_t value = *addr;
  bool prevCarry = carryFlag;

  // the original bit 7 is shifted into carryFlag
  carryFlag = (value & negativeMask);

  // rotate 1 bit
  value <<= 1;

  // original carry value is shifted into bit 0
  if (prevCarry)
  {
    value |= 1;
  }

  // resulting value affects zeroFlag
  if (value == 0)
    zeroFlag = true;

  *addr = value;
}

// PulL Processor status register
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
void Cpu::iBMI(uint8_t *addr)
{
  // branch, otherwise continue to next instruction
  if (negativeFlag)
    pc = &memory[*addr];
}

// SEt Carry
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

  // value became zero
  if (value == 0)
  {
    zeroFlag = true;
  }
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

  // get program counter from stack
  setProgramCounter(popStack());
}

// bitwise exclusive OR
void Cpu::iEOR(uint8_t *addr)
{
  uint8_t value = *addr;

  // accumulator = accumulator ^ *memoryAddr
  a ^= value;

  return;
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

  // original bit 0 is shifted into the Carry
  carryFlag = (value & 1);

  value >>= 1;

  // affects zero flag
  zeroFlag = (value == 0);

  *addr = value;
}

// PusH Accumulator
void Cpu::iPHA(uint8_t *addr)
{
  uint8_t value = *addr;

  // push onto stack
  pushStack(value);
}

// PusH K register
void Cpu::iPHK(uint8_t *addr)
{
  // unused
}

// JuMP
// always uses 16 bits, addressing modes can only be absolute or indirect
void Cpu::iJMP(uint8_t *addr)
{
  uint16_t value = 0;

  // absolute address is in 2 byte following instruction in reverse byte order
  value = (*(addr+1) & 0xFF) << 8;
  value |= *(addr) & 0xFF;

  setProgramCounter(value);
}

// Branch if oVerflow Clear
void Cpu::iBVC(uint8_t *addr)
{
  // TODO - branch
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
}

// Transfer C accumulator to Direct register
void Cpu::iTCD(uint8_t *addr)
{
}

// ReTurn from Subroutine
void Cpu::iRTS(uint8_t *addr)
{
}

// ADd with Carry
void Cpu::iADC(uint8_t *addr)
{
  uint8_t value = *addr;
  printf("ADC: %x += %x", a, value);

  // accumulator = accumulator + *memoryAddr
  a += value;

  printf(" = %x\n", a);
  return;
}

// Push Effective Relative address
void Cpu::iPER(uint8_t *addr)
{
}

// STore Zero
void Cpu::iSTZ(uint8_t *addr)
{
}

// ROtate Right
void Cpu::iROR(uint8_t *addr)
{
}

// PulL Accumulator
void Cpu::iPLA(uint8_t *addr)
{
}

// ReTurn from subroutine Long
void Cpu::iRTL(uint8_t *addr)
{
}

// Branch if oVerflow Set
void Cpu::iBVS(uint8_t *addr)
{
}

// SEt Interrupt disable
void Cpu::iSEI(uint8_t *addr)
{
  interruptFlag = true;
}

// PulL Y register
void Cpu::iPLY(uint8_t *addr)
{
}

// Transfer Direct register to C accumulator
void Cpu::iTDC(uint8_t *addr)
{
}

// BRanch Always
void Cpu::iBRA(uint8_t *addr)
{
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

  zeroFlag = (a == 0);
}

// PusH data Bank register
void Cpu::iPHB(uint8_t *addr)
{
  // unused
}

// Branch if Carry Clear
void Cpu::iBCC(uint8_t *addr)
{
  // TODO - branch
}

// Transfer Y register to Accumulator
// Affect Flags: S Z
void Cpu::iTYA(uint8_t *addr)
{
  a = y;

  zeroFlag = (a == 0);
}

// Transfer X register to Stack pointer
void Cpu::iTXS(uint8_t *addr)
{
  *sp = x;
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

  zeroFlag = (y == 0);
}

// LoaD Accumulator
void Cpu::iLDA(uint8_t *addr)
{
  uint8_t value = *addr;

  // a = value at *memoryAddr
  a = value;

  zeroFlag = (a == 0);
}

// LoaD X register
// Affects Flags: S Z
void Cpu::iLDX(uint8_t *addr)
{
  uint8_t value = *addr;

  x = value;

  zeroFlag = (x == 0);
}

// Transfer Accumulator to Y register
// Affects Flags: S Z
void Cpu::iTAY(uint8_t *addr)
{
  uint8_t value = a;

  y = value;

  zeroFlag = (y == 0);
}

// Transfer Accumulator to X register
void Cpu::iTAX(uint8_t *addr)
{
  uint8_t value = a;

  x = value;

  zeroFlag = (x == 0);
}

// PulL data Bank register
void Cpu::iPLB(uint8_t *addr)
{
}

// Branch if Carry Set
void Cpu::iBCS(uint8_t *addr)
{
}

// CLear oVerflow
void Cpu::iCLV(uint8_t *addr)
{
  overflowFlag = false;
}

// Transfer Stack pointer to X register
void Cpu::iTSX(uint8_t *addr)
{
}

// Transfer Y register to X register
void Cpu::iTYX(uint8_t *addr)
{
}

// ComPare to Y register
void Cpu::iCPY(uint8_t *addr)
{
}

// CoMPare (to accumulator)
void Cpu::iCMP(uint8_t *addr)
{
//  printf("CMP: %x ? %x", a, value);
  // compare value at *memoryAddr with accumulator
  printf("CMP\n");

  return;
}

// REset Processor status bits
void Cpu::iREP(uint8_t *addr)
{
}

// INcrement Y register
void Cpu::iINY(uint8_t *addr)
{
}

// DEcrement X register
void Cpu::iDEX(uint8_t *addr)
{
}

// WAit for Interrupt
void Cpu::iWAI(uint8_t *addr)
{
}

// Branch if Not Equal
void Cpu::iBNE(uint8_t *addr)
{
}

// Push Effective Indirect address
void Cpu::iPEI(uint8_t *addr)
{
}

// CLear Decimal mode
void Cpu::iCLD(uint8_t *addr)
{
  decimalFlag = false;
}

// PusH X register
void Cpu::iPHX(uint8_t *addr)
{
}

// SToP the clock
void Cpu::iSTP(uint8_t *addr)
{
}

// JuMP Long
void Cpu::iJML(uint8_t *addr)
{
}

// ComPare to X register
void Cpu::iCPX(uint8_t *addr)
{
}

// SuBtract with Carry
// Affects Flags: S V Z C
void Cpu::iSBC(uint8_t *addr)
{
  uint8_t value = *addr;
  uint8_t onesComplement = getOnesComplement(value);
  uint16_t checkOverflow = value + onesComplement;

  // sbc uses ones complement with overflow flag
  a += getOnesComplement(value);

  // value overflowed 8 bits
  carryFlag = (checkOverflow > 0xFF);

  negativeFlag = (getTwosComplement(a) < 0);

  zeroFlag = (a == 0);

  return;
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

  zeroFlag = (x == 0);
}

// No OPeration
void Cpu::iNOP(uint8_t *addr)
{
}

// eXchange B and A accumulator
void Cpu::iXBA(uint8_t *addr)
{
  // unused
}

// Branch if EQual
void Cpu::iBEQ(uint8_t *addr)
{
  // TODO - branch
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

