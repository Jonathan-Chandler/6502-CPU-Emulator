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

