#include "Arduino-LiquidCrystal2.h"

LiquidCrystal::LiquidCrystal(uint8_t _rs, uint8_t _en, uint8_t _d4, uint8_t _d5, uint8_t _d6, uint8_t _d7) {
  rs = _rs;
  en = _en;
  data_pins[7] = _d7;
  data_pins[6] = _d6;
  data_pins[5] = _d5;
  data_pins[4] = _d4;

  pinMode(_rs, OUTPUT);
  pinMode(_en, OUTPUT);
  pinMode(_d4, OUTPUT);
  pinMode(_d5, OUTPUT);
  pinMode(_d6, OUTPUT);
  pinMode(_d7, OUTPUT);
}

void LiquidCrystal::begin(uint8_t _cols, uint8_t _rows) {
  cols = _cols;
  rows = _rows;

  if (rows > 2) rows = 2;

  digitalWrite(en, LOW);
  delay(50);
  write_nibble(0, 0x03);
  delay(5);

  write_nibble(0, 0x03);
  delayMicroseconds(100);

  write_nibble(0, 0x03);
  write_nibble(0, 0x02);

  write_byte(0, FUNCTION_SET | MODE_4BIT | (rows == 2 ? MODE_2LINE:MODE_1LINE) | MODE_5X8_FONT);

  _display_ctrl = DISPLAY_CTRL | DISPLAY_ON | CURSOR_OFF | BLINK_CURSOR_OFF;
  write_byte(0, _display_ctrl);

  clear();

  write_byte(0, ENTRY_MODE | INCREMENT);

  clear();
}

void LiquidCrystal::clear() {
  write_byte(0, CLEAR_DISPLAY);
}

void LiquidCrystal::home() {
  write_byte(0, RETURN_HOME);
}

void LiquidCrystal::setCursor(uint8_t _col, uint8_t _row) {
  write_byte(0, SET_DDRAM_ADDR | (_col + _row * 0x40));
}

size_t LiquidCrystal::write(uint8_t data) {
  write_byte(1, data);
  return 1;
}

void LiquidCrystal::blink_cursor_pos(uint8_t enabled) {
  _display_ctrl &= ~0x01;
  _display_ctrl |= enabled & 0x01;
  write_byte(0, _display_ctrl);
}

void LiquidCrystal::cursor(uint8_t enabled) {
  _display_ctrl &= ~0x02;
  _display_ctrl |= (enabled<<1) & 0x02;
  write_byte(0, _display_ctrl);
}

void LiquidCrystal::display(uint8_t enabled) {
  _display_ctrl &= ~0x04;
  _display_ctrl |= (enabled<<2) & 0x04;
  write_byte(0, _display_ctrl);
}

void LiquidCrystal::write_byte(uint8_t _rs, uint8_t data) {
  /*Serial.print("Writing ");
  Serial.print(data);
  Serial.print(" with rs=");
  Serial.println(_rs);*/
  write_nibble(_rs, data >> 4);
  write_nibble(_rs, data & 0x0F);
  delayMicroseconds(500);
}

void LiquidCrystal::write_nibble(uint8_t _rs, uint8_t data) {
  digitalWrite(rs, _rs);
  digitalWrite(en, HIGH);
  delayMicroseconds(1);

  for(uint8_t i = 4; i < 8; i++) {
    digitalWrite(data_pins[i], data & (1<<(i-4)));
  }

  delayMicroseconds(1);

  digitalWrite(en, LOW);
  delay(1);
}
