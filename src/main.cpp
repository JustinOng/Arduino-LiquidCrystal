#include <Arduino.h>
#include "Arduino-LiquidCrystal2.h"

LiquidCrystal lcd(14, 15, 16, 17, 18, 19);

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.throttleUpdates(100);
}

void loop() {
  lcd.clear();
  lcd.print("Hello: ");
  lcd.print(millis());
  lcd.update();
}
