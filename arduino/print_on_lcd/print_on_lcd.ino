#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with the I2C address, number of columns, and number of rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin(16, 2);      // Define 16 columns and 2 rows for the LCD
  lcd.backlight();       // Power ON the backlight
  Serial.begin(9600);    // Initialize serial communication
}

void loop() {
  lcd.clear();
  lcd.print("Rohan");
  Serial.print("Rohan\n");
  lcd.setCursor(0, 1);
  lcd.print("Hello");
  Serial.print("Hello\n");
  delay(1000);
}
