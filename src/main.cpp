#include <Arduino.h>
#include "Arduino-LiquidCrystal2.h"

LiquidCrystal lcd(14, 15, 16, 17, 18, 19);

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.setCursor(8, 0);
  lcd.cursor(1);
  lcd.blink_cursor_pos(1);
  Serial.println("Printing text");
  lcd.print("hello");
  lcd.setCursor(0, 1);
  lcd.print('b');
  lcd.setCursor(0, 0);
  Serial.println("Setup!");
}

void loop() {

}
