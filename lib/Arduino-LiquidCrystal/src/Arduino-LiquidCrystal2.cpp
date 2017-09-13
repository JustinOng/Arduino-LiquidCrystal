#include <Arduino-LiquidCrystal2.h>

LiquidCrystal::LiquidCrystal(uint8_t _rs, uint8_t _en, uint8_t _d4, uint8_t _d5, uint8_t _d6, uint8_t _d7) {
  rs = _rs;
  en = _en;
  data_pins[7] = _d7;
  data_pins[6] = _d6;
  data_pins[5] = _d5;
  data_pins[4] = _d4;
}

LiquidCrystal::LiquidCrystal(uint8_t _rs, uint8_t _rw, uint8_t _en, uint8_t _d4, uint8_t _d5, uint8_t _d6, uint8_t _d7) {
  rw = _rw;
  LiquidCrystal(_rs, _en, _d4, _d5, _d6, _d7);
}

LiquidCrystal::LiquidCrystal() {
  
}

void LiquidCrystal::initialise_hardware() {
  pinMode(rs, OUTPUT);

  if (rw != 255) {
    pinMode(rw, OUTPUT);
    digitalWrite(rw, LOW);
  }

  pinMode(en, OUTPUT);
  pinMode(data_pins[4], OUTPUT);
  pinMode(data_pins[5], OUTPUT);
  pinMode(data_pins[6], OUTPUT);
  pinMode(data_pins[7], OUTPUT);

  digitalWrite(en, LOW);
}

void LiquidCrystal::begin(uint8_t _cols, uint8_t _rows) {
  initialise_hardware();

  cols = _cols;
  rows = _rows;

  if (rows > 2) rows = 2;

  screen_buffer = (uint8_t*) malloc(cols*rows);
  pScreen_buffer = (uint8_t*) malloc(cols*rows);

  if (screen_buffer == NULL) {
    screen_buffer_len = 0;
  }
  else {
    screen_buffer_len = cols * rows;
  }

  memset(screen_buffer, 0x32, screen_buffer_len);
  memset(pScreen_buffer, 0x32, screen_buffer_len);

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

  write_byte(0, CLEAR_DISPLAY);

  write_byte(0, ENTRY_MODE | INCREMENT);

  write_byte(0, CLEAR_DISPLAY);
}

void LiquidCrystal::throttleUpdates(uint16_t ms) {
  throttle_time = ms;
}

void LiquidCrystal::update() {
  static uint32_t previous_update_time = 0;

  if ((millis() - previous_update_time) > throttle_time) {
    write_buffer_to_lcd();
    previous_update_time = millis();
  }
}

void LiquidCrystal::clear() {
  //write_byte(0, CLEAR_DISPLAY);

  memset(screen_buffer, 0x32, screen_buffer_len);
  cursor_x = 0;
  cursor_y = 0;
}

void LiquidCrystal::home() {
  write_byte(0, RETURN_HOME);
}

void LiquidCrystal::setCursor(uint8_t _col, uint8_t _row) {
  cursor_x = _col;
  cursor_y = _row;
}

void LiquidCrystal::set_actual_cursor(uint8_t _col, uint8_t _row) {
  write_byte(0, SET_DDRAM_ADDR | (_col + _row * 0x40));
}

size_t LiquidCrystal::write(uint8_t data) {
  //write_byte(1, data);

  screen_buffer[cursor_x + (cursor_y*cols)] = data;

  /*Serial.print("Wrote ");
  Serial.print((char)data);
  Serial.print(" at ");
  Serial.print(cursor_x);
  Serial.print(", ");
  Serial.println(cursor_y);*/

  cursor_x ++;
  return 1;
}

void LiquidCrystal::blinkCursorPos(uint8_t enabled) {
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

void LiquidCrystal::write_buffer_to_lcd() {
  // compares screen_buffer to pScreen_buffer and only writes the char to lcd if it has changed
  uint8_t previous_char_changed = 0;

  uint8_t cur_pos = 0;

  for(uint8_t y = 0; y < rows; y++) {
    for(uint8_t x = 0; x < cols; x++) {
      cur_pos = x+y*cols;

      /*Serial.print(x);
      Serial.print(", ");
      Serial.print(y);
      Serial.print("(");
      Serial.print(cur_pos);
      Serial.print(") is ");
      Serial.println((char) screen_buffer[cur_pos]);*/

      if (screen_buffer[cur_pos] != pScreen_buffer[cur_pos]) {
        // if previous char did not change, meaning cursor is not at this pos
        // need to manually set

        /*Serial.print("Char at ");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.print(" changed from ");
        Serial.print((char) pScreen_buffer[cur_pos]);
        Serial.print(" to ");
        Serial.println((char) screen_buffer[cur_pos]);*/

        if (!previous_char_changed) {
          set_actual_cursor(x, y);
        }

        write_byte(1, screen_buffer[cur_pos]);

        previous_char_changed = true;
      }
      else {
        previous_char_changed = false;
      }
    }

    // set to false so upon shifting to next row actual cursor will be moved
    previous_char_changed = false;
  }

  memcpy(pScreen_buffer, screen_buffer, screen_buffer_len);
  set_actual_cursor(cursor_x, cursor_y);
}
