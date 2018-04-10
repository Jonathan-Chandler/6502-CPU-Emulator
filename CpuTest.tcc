#include "CpuTest.hpp"
#include <stdio.h>

template <class T>
CpuTest<T>::CpuTest(const T &cpuClass)
  : cpuToTest(cpuClass)
{
}

template <class T>
void CpuTest<T>::startTest()
{
  if (!testLDA())
  {
    printf("failed test LDA\n");
  }
  else
  {
    printf("pass test LDA\n");
  }

//  if (!testORA())
//  {
//    printf("failed test ORA");
//  }
}

//template <class T>
//bool CpuTest<T>::testDisplay()
//{
//  uint8_t value = 5;
//
//  if (cpuToTest.getA() == value)
//    return true;
//  return false;
//}


template <class T>
bool CpuTest<T>::testORA()
{
  uint8_t value = 5;

  if (cpuToTest.getA() == value)
    return true;
  return false;
}


template <class T>
bool CpuTest<T>::testLDA()
{
  // LDA #$01
  uint8_t instructions[] = {0xA9, 0x01};
  uint8_t returnedValue = 0;
  uint8_t expectedValue = 1;

  cpuToTest.doInstruction(instructions);
  returnedValue = cpuToTest.getA();

  if (returnedValue == expectedValue)
    return true;

  return false;
}


template <class T>
void CpuTest<T>::testADC()
{
  InstructionTest_t *currentTest = CpuTest<T>::ADC_Instructions;
  size_t numberOfTests = sizeof(ADC_Instructions)/sizeof(ADC_Instructions[0]);
  uint8_t flagRes;
  bool testFailed = false;

  for (size_t i = 0; i < numberOfTests; i++)
  {

    currentTest = &CpuTest<T>::ADC_Instructions[i];
    cpuToTest.reset();
    cpuToTest.setMemory(currentTest->instructionPointer);
    for (uint8_t instructionsDone = 0; instructionsDone < currentTest->instructionCount; instructionsDone++)
    {
      cpuToTest.doInstruction();
    }

    flagRes = cpuToTest.getFlags();

    if (cpuToTest.getA() != currentTest->expectedRegisterA)
    {
      printf("fail addition, res: %x != %x\n", cpuToTest.getA(), currentTest->expectedRegisterA);
      testFailed = true;
    }

    // test negative
    if ((flagRes & T::negativeMask) != (currentTest->expectedFlags & T::negativeMask))
    {
      printf("negative flag fails\n");
      testFailed = true;
    }

    // test overflow
    if ((flagRes & T::overflowMask) != (currentTest->expectedFlags & T::overflowMask))
    {
      printf("overflow flag fails\n");
      testFailed = true;
    }

    // test carry
    if ((flagRes & T::carryMask) != (currentTest->expectedFlags & T::carryMask))
    {
      printf("carry flag fails\n");
      testFailed = true;
    }
  }

  if (testFailed)
    printf("failed adc\n");
  else
    printf("adc success\n");
}
  /*
  uint8_t instructions[] = {0xa9, 0xF0, 0x69, 0xF0}; // 
  cpuToTest.doInstruction(&instructions[0]);
  cpuToTest.doInstruction(&instructions[2]);

  if (cpuToTest.getA() != 5)
  {
    printf("fail addition, res: %x\n", cpuToTest.getA());
  }
  else
  {
    printf("success, addition res: %x\n", cpuToTest.getA());
  }

  uint8_t flagRes = cpuToTest.getFlags();

  // test negative
  if (flagRes & T::negativeMask)
  {
    printf("negative set\n");
  }
  else
  {
    printf("negative unset\n");
  }

  // test overflow
  if (flagRes & T::overflowMask)
  {
    printf("overflow set\n");
  }
  else
  {
    printf("overflow unset\n");
  }

  // test carry
  if (flagRes & T::carryMask)
  {
    printf("carry set\n");
  }
  else
  {
    printf("carry unset\n");
  }
  */

template <class T>
void CpuTest<T>::testSBC()
{
  InstructionTest_t *currentTest = CpuTest<T>::SBC_Instructions;
  size_t numberOfTests = sizeof(SBC_Instructions)/sizeof(SBC_Instructions[0]);
  uint8_t flagRes;
  bool testFailed = false;

  for (size_t i = 0; i < numberOfTests; i++)
  {
    currentTest = &CpuTest<T>::SBC_Instructions[i];
    cpuToTest.reset();
    cpuToTest.setMemory(currentTest->instructionPointer);
    for (uint8_t instructionsDone = 0; instructionsDone < currentTest->instructionCount; instructionsDone++)
    {
      cpuToTest.doInstruction();
    }
    flagRes = cpuToTest.getFlags();

    if (cpuToTest.getA() != currentTest->expectedRegisterA)
    {
      printf("fail subtraction, res: %x != %x\n", cpuToTest.getA(), currentTest->expectedRegisterA);
    }

    // test negative
    if ((flagRes & T::negativeMask) != (currentTest->expectedFlags & T::negativeMask))
    {
      printf("negative flag fails\n");
    }

    // test overflow
    if ((flagRes & T::overflowMask) != (currentTest->expectedFlags & T::overflowMask))
    {
      printf("overflow flag fails\n");
    }

    // test carry
    if ((flagRes & T::carryMask) != (currentTest->expectedFlags & T::carryMask))
    {
      printf("carry flag fails\n");
    }
  }

  if (testFailed)
    printf("failed sbc\n");
  else
    printf("sbc success\n");
}

//template <class T>
//void CpuTest<T>::testLDA()
//{
//  InstructionTest_t *currentTest = CpuTest<T>::SBC_Instructions;
//  size_t numberOfTests = sizeof(SBC_Instructions)/sizeof(SBC_Instructions[0]);
//  uint8_t flagRes;
//  bool testFailed = false;
//
//  for (size_t i = 0; i < numberOfTests; i++)
//  {
//    currentTest = &CpuTest<T>::SBC_Instructions[i];
//    cpuToTest.reset();
//    cpuToTest.setMemory(currentTest->instructionPointer);
//    for (uint8_t instructionsDone = 0; instructionsDone < currentTest->instructionCount; instructionsDone++)
//    {
//      cpuToTest.doInstruction();
//    }
//    flagRes = cpuToTest.getFlags();
//
//    if (cpuToTest.getA() != currentTest->expectedRegisterA)
//    {
//      printf("fail subtraction, res: %x != %x\n", cpuToTest.getA(), currentTest->expectedRegisterA);
//    }
//
//    // test negative
//    if ((flagRes & T::negativeMask) != (currentTest->expectedFlags & T::negativeMask))
//    {
//      printf("negative flag fails\n");
//    }
//
//    // test overflow
//    if ((flagRes & T::overflowMask) != (currentTest->expectedFlags & T::overflowMask))
//    {
//      printf("overflow flag fails\n");
//    }
//
//    // test carry
//    if ((flagRes & T::carryMask) != (currentTest->expectedFlags & T::carryMask))
//    {
//      printf("carry flag fails\n");
//    }
//  }
//
//  if (testFailed)
//    printf("failed sbc\n");
//  else
//    printf("sbc success\n");
//}
// 1st     2nd    result      N        V        C
// -----------------------------------------------
//  7      -2        5        0        0        1
//  7       2        9        0        0        0
//  7      80       87        1        0        0
//  7      -9       -2        1        0        0
//  7      7A       81        1        1        0
// 80      90       10        0        1        1
// F0      F0       E0        1        0        1
// F8      0A        2        0        0        1
// -1      1         0        0        0        1

template <class T>
typename CpuTest<T>::InstructionTest_t CpuTest<T>::ADC_Instructions[] = 
{
  // instruction_count, {instructions}, tested_flags, tested_registers, expected_flags, expected_A, expected_x, expected_y
  {2, {0xa9, 0x07, 0x69, 0xfe}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::carryMask), 0x5, 0, 0},
  {2, {0xa9, 0x07, 0x69, 0x02}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (0), 0x9, 0, 0},
  {2, {0xa9, 0x07, 0x69, 0x80}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::negativeMask), 0x87, 0, 0},
  {2, {0xa9, 0x07, 0x69, 0xF7}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::negativeMask), 0xFE, 0, 0},
  {2, {0xa9, 0x07, 0x69, 0x7a}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::negativeMask | T::overflowMask), 0x81, 0, 0},
  {2, {0xa9, 0x80, 0x69, 0x90}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::overflowMask | T::carryMask), 0x10, 0, 0},
  {2, {0xa9, 0xF0, 0x69, 0xF0}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::negativeMask | T::carryMask), 0xE0, 0, 0},
  {2, {0xa9, 0xF8, 0x69, 0x0A}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::carryMask), 0x2, 0, 0},
  {2, {0xa9, 0xFF, 0x69, 0x01}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::carryMask | T::zeroMask), 0x0, 0, 0},
};

template <class T>
typename CpuTest<T>::InstructionTest_t CpuTest<T>::SBC_Instructions[] = 
{
  // instruction_count, {instructions}, tested_flags, tested_registers, expected_flags, expected_A, expected_x, expected_y
  {3, {0x38, 0xa9, 0xF8, 0xE9, 0x0A}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::negativeMask | T::carryMask), 0xEE, 0, 0},
  {3, {0x38, 0xa9, 0x81, 0xE9, 0x07}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::overflowMask | T::carryMask), 0x7A, 0, 0},
  {3, {0x38, 0xa9, 0x07, 0xE9, 0x02}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::carryMask), 0x05, 0, 0},
  {3, {0x38, 0xa9, 0x07, 0xE9, 0xFE}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (0), 0x09, 0, 0},
  {3, {0x38, 0xa9, 0x07, 0xE9, 0x09}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::negativeMask), 0xFE, 0, 0},
  {3, {0x38, 0xa9, 0x07, 0xE9, 0x90}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (0), 0x77, 0, 0},
  {3, {0x38, 0xa9, 0x10, 0xE9, 0x90}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::negativeMask | T::overflowMask), 0x80, 0, 0},
  {3, {0x38, 0xa9, 0x10, 0xE9, 0x91}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (0), 0x7F, 0, 0},
  {3, {0x38, 0xa9, 0xFF, 0xE9, 0xFF}, (TestNegativeFlag | TestOverflowFlag | TestCarryFlag | TestZeroFlag), (TestRegisterA), (T::carryMask | T::zeroMask), 0x00, 0, 0},
};

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
// -1      -1       00        0        0        1



