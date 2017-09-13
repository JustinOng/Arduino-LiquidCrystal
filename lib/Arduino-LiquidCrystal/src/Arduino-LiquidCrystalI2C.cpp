#include <Arduino-LiquidCrystalI2C.h>

void LiquidCrystalI2C::begin(uint8_t _address, uint8_t _cols, uint8_t _rows) {
  address = _address;
  cols = _cols;
  rows = _rows;

  rs = 0;
  rw = 1;
  en = 2;
  data_pins[4] = 4;
  data_pins[5] = 5;
  data_pins[6] = 6;
  data_pins[7] = 7;

  LiquidCrystal::begin(_cols, _rows);
}

void LiquidCrystalI2C::initialise_hardware() {
  Serial.println("Initialise called");
  Wire.begin();
}

void LiquidCrystalI2C::write_nibble(uint8_t _rs, uint8_t _data) {
  // 0x08 is backlight control
  uint8_t data = ((_data << 4) & 0xF0) | 0x08 | (_rs & 0x01);

  delayMicroseconds(500);

  /*Serial.print("Writing ");
  Serial.println(data, BIN);

  Serial.println(en);*/

  Wire.beginTransmission(address);
	Wire.write(data | (1<<en));
	Wire.endTransmission();

  Wire.beginTransmission(address);
	Wire.write(data);
	Wire.endTransmission();

  delayMicroseconds(50);
}
