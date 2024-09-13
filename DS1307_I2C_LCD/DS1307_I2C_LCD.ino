#include <Bonezegei_DS1307.h>  //Library used
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

Bonezegei_DS1307 rtc(0x68);          //
LiquidCrystal_I2C lcd(0x50, 16, 2);  // set the LCD address to 0x27 if 0x3F not working

void setup() {
  int error;
  int show = -1;
  Serial.begin(9600);

  rtc.begin();
  //rtc.setFormat(12);        //Set 12 Hours Format
  // rtc.setAMPM(1);           //Set AM or PM    0 = AM  1 =PM
  //rtc.setTime("11:59:30");  //Set Time    Hour:Minute:Seconds
  // rtc.setDate("1/27/24");  //Set Date    Month/Date/Year

  Wire.begin();
  Wire.beginTransmission(0x50);  // set the LCD address to 0x27 if 0x3F not working
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    show = 0;
    lcd.begin(16, 2);  // Use begin() instead of init() to initialize the lcd
    lcd.backlight();
  } else {
    Serial.println(": LCD not found.");
  }
}

void loop() {

  if (rtc.getTime()) {
    lcd.clear();  // Here after clearing the LCD we take the time from the module and print it on the screen with usual LCD functions

    lcd.setCursor(0, 0);
    lcd.print("Date: ");
    lcd.print(twoDigit(rtc.getDate()));
    lcd.print("/");
    lcd.print(twoDigit(rtc.getMonth()));
    lcd.print("/");
    lcd.print("20" + String(rtc.getYear()));

    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(twoDigit(rtc.getHour()));
    lcd.print(":");
    lcd.print(twoDigit(rtc.getMinute()));
    lcd.print(":");
    lcd.print(twoDigit(rtc.getSeconds()));


    if (rtc.getFormat() == 12) {  // returns 12 or 24 hour format

      if (rtc.getAMPM()) {  // return 0 = AM  1 = PM
        lcd.print("PM");
      } else {
        lcd.print("AM");
      }
    }
  }

  delay(1000);
  if (rtc.getTime()) {
      Serial.print("Date: ");
      Serial.print(twoDigit(rtc.getDate()));
      Serial.print("/");
      Serial.print(twoDigit(rtc.getMonth()));
      Serial.print("/");
      Serial.print("20" + String(rtc.getYear()));
      
      Serial.print("    ::    Time: ");
      Serial.print(twoDigit(rtc.getHour()));
      Serial.print(":");
      Serial.print(twoDigit(rtc.getMinute()));
      Serial.print(":");
      Serial.print(twoDigit(rtc.getSeconds()));
    

    if (rtc.getFormat() == 12) {  // returns 12 or 24 hour format

      if (rtc.getAMPM()) {  //return 1 = AM  0 = PM
        Serial.println("PM");
      } else {
        Serial.println("AM");
      }
    }
  }
}

String twoDigit(int number) {
  if (number < 10) {              // If it's a single-digit number
    return "0" + String(number);  // Add a leading zero
  } else {
    return String(number);  // Otherwise, just return the number as is
  }
}
