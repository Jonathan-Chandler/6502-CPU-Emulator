#ifndef ROM_H
#define ROM_H
#include <iostream>

class Rom
{
  public:
    Rom();
    void load(char *fileName);
    void load();
    void printHeader();
    void printTrainer();
    void printRom();
    void printChr();
    void printInstRom();
    void printProm();
    uint8_t getPrgSize();
    uint8_t *getChrRomData();

  private:
    uint8_t header[16];
    uint8_t *PrgRomData;
    uint8_t *ChrRomData;
};
#endif
