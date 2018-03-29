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

  private:
    T cpuToTest;

    bool testORA();
    bool testLDA();
    bool testAND();
    bool testADC();
};

#endif
