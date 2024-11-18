#include <Wire.h>
#include "RTClib.h"

// Create RTC object
RTC_DS1307 rtc;

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  Serial.println("Initializing RTC...");

  // Initialize I2C (Wire library)
  Wire.begin(D2, D1); // SDA = D2 (GPIO4), SCL = D1 (GPIO5)

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    while (1); // Stop execution
  }

  // Check if the RTC lost power
  // if (!rtc.isrunning()) {
    // Serial.println("RTC lost power, setting the time!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set time from compilation time
  // }
}

void loop() {
  // Get current time from RTC
  DateTime now = rtc.now();

  // Print current date and time
  Serial.print("Date: ");
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print(" Time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000); // Update every second
}


