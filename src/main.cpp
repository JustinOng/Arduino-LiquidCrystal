#include <Arduino.h>
#include <Arduino-LiquidCrystalI2C.h>

//LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
LiquidCrystalI2C lcd;

void setup() {
  Serial.begin(115200);
  lcd.begin(0x27, 16, 2);
  lcd.throttleUpdates(250);
}

void loop() {
  lcd.clear();
  lcd.print("Hello: ");
  lcd.print(millis());
  lcd.update();
}
