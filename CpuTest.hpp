#ifndef CPU_TEST_HPP
#define CPU_TEST_HPP
#include <string.h>
#include <iostream>

template <class T>
class CpuTest
{
  public:
    CpuTest(const T &cpuToTest);
    void startTest();
    void testADC();
    void testSBC();

    typedef struct 
    {
      uint8_t instructionCount;
      uint8_t instructionPointer[5];
      uint8_t testedFlags;
      uint8_t testedRegisters;
      uint8_t expectedFlags;
      uint8_t expectedRegisterA;
      uint8_t expectedRegisterX;
      uint8_t expectedRegisterY;
    } InstructionTest_t;

  private:
    T cpuToTest;

    enum TestedRegisters
    {
      TestRegisterA     = 1,
      TestRegisterX     = 2,
      TestRegisterY     = 4,
    };

    enum TestedFlags
    {
      TestCarryFlag     = 1,
      TestZeroFlag      = 2,
      TestInterruptFlag = 4,
      TestDecimalFlag   = 8,
      TestBreakFlag     = 16,
      TestUnusuedFlag   = 32,
      TestOverflowFlag  = 64,
      TestNegativeFlag  = 128
    };

    static InstructionTest_t ADC_Instructions[];
    static InstructionTest_t SBC_Instructions[];
//    {
//      // instruction_count, {instructions}, tested_flags, tested_registers, expected_flags, expected_A, expected_x, expected_y
//      {2, {0xa9, 0x07, 0x69, 0xfe}, {TestNegativeFlag | TestOverflowFlag | TestCarryFlag}, {TestRegisterA}, {T::carryMask}, 5, 0, 0},
//      {2, {0xa9, 0x07, 0x69, 0x02}
//      {2, {0xa9, 0x07, 0x69, 0x80}
//      {2, {0xa9, 0x07, 0x69, 0xF7}
//      {2, {0xa9, 0x07, 0x69, 0x7a}
//      {2, {0xa9, 0x80, 0x69, 0x90}
//      {2, {0xa9, 0xF0, 0x69, 0xF0}
//      {2, {0xa9, 0xF8, 0x69, 0x0A}
//    };
//
//
    bool testORA();
    bool testLDA();
    bool testAND();
};

#endif
