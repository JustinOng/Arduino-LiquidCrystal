#ifndef LCDI2C_H
#define LCDI2C_H

#include <Arduino-LiquidCrystal2.h>

class LiquidCrystalI2C: public LiquidCrystal {
  public:
    LiquidCrystalI2C():LiquidCrystal() {};
    void begin(uint8_t _address, uint8_t _cols, uint8_t _rows);
  protected:
    uint8_t address = 0;

    void initialise_hardware();
    void write_nibble(uint8_t _rs, uint8_t data);
};

#endif
