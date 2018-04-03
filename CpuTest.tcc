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
  printf("test recomp9\n");

//  if (!testORA())
//  {
//    printf("failed test ORA");
//  }
}

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
  // ADC, start with C clear
  // 1st     2nd    result      N        V        C
  // -----------------------------------------------
  //  7      -2        5        0        0        1
  //uint8_t instructions[] = {0xa9, 0x07, 0x69, 0xfe};
  //  7       2        9        0        0        0
  //uint8_t instructions[] = {0xa9, 0x07, 0x69, 0x02};
  //  7      80       87        1        0        0
  //uint8_t instructions[] = {0xa9, 0x07, 0x69, 0x80};
  //  7      -9       -2        1        0        0
  //uint8_t instructions[] = {0xa9, 0x07, 0x69, 0xF7}; // F7 = 1111 0111 = -9
  //  7      7A       81        1        1        0
  //uint8_t instructions[] = {0xa9, 0x07, 0x69, 0x7a}; // 7a = 0111 1010
  // 80      90       10        0        1        1
  // F0      F0       E0        1        0        1
  // F8      0A        2        0        0        1
  // 
  uint8_t instructions[] = {0xa9, 0x07, 0x69, 0x7A}; // F7 = 0111 1010 = 7A
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
  if (flagRes & Cpu::negativeMask)
  {
    printf("negative set\n");
  }
  else
  {
    printf("negative unset\n");
  }

  // test overflow
  if (flagRes & Cpu::overflowMask)
  {
    printf("overflow set\n");
  }
  else
  {
    printf("overflow unset\n");
  }

  // test carry
  if (flagRes & Cpu::carryMask)
  {
    printf("carry set\n");
  }
  else
  {
    printf("carry unset\n");
  }
}
