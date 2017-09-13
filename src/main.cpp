#include <Arduino.h>
#include <Arduino-LiquidCrystalI2C.h>

//LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
LiquidCrystalI2C lcd;

void setup() {
  Serial.begin(115200);
  lcd.begin(0x27, 16, 2);
  lcd.throttleUpdates(250);
  //lcd.blinkChars(0, 4, 0);
  lcd.blinkChar(0, 0);
  lcd.blinkChar(7, 0);
  lcd.blinkChars(0, 8, 1);
  lcd.setBlinkInterval(1000);
}

void loop() {
  if (millis() > 5000) {
    lcd.clearBlinkChars();
  }
  
  lcd.clear();
  lcd.print("Hello: ");
  lcd.print(millis());
  lcd.setCursor(0, 1);
  lcd.print("Another line");
  lcd.update();
}
