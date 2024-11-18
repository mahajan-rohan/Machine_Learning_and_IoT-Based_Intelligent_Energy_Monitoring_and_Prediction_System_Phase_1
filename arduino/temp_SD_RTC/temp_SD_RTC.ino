#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

// Temperature Sensor
#define ONE_WIRE_BUS 2   // Data wire for temperature sensor on pin 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// RTC Module
RTC_DS1307 rtc;

// SD Card Module
File dataFile;
String filename = "tempLog.txt";

void setup() {
  Serial.begin(9600);

  // Start temperature sensor
  sensors.begin();

  // Initialize RTC module
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);  // Stop if RTC not found
  }
  
  if (!rtc.isrunning()) {
    Serial.println("RTC is not running, setting the time...");
    rtc.adjust(DateTime(2024, 10, 20, 12, 0, 0));  // Set RTC time manually
  }

  // Initialize SD card
  if (!SD.begin(10)) {
    Serial.println("SD Card initialization failed!");
    return;
  }
  Serial.println("SD Card initialized.");
}

void loop() {
  DateTime now = rtc.now();  // Get current time from RTC
  
  // Request temperature data from DS18B20 sensor
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);  // Get temperature in Celsius
  
  if (tempC != DEVICE_DISCONNECTED_C) {
    // Print temperature to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println(" C");

    // Store the temperature and timestamp in SD card
    dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
      // Write timestamp to file
      dataFile.print("Timestamp: ");
      dataFile.print(now.year(), DEC);
      dataFile.print('/');
      dataFile.print(now.month(), DEC);
      dataFile.print('/');
      dataFile.print(now.day(), DEC);
      dataFile.print(' ');
      dataFile.print(now.hour(), DEC);
      dataFile.print(':');
      dataFile.print(now.minute(), DEC);
      dataFile.print(':');
      dataFile.print(now.second(), DEC);

      // Write temperature to file
      dataFile.print(" | Temperature: ");
      dataFile.print(tempC);
      dataFile.println(" C");
      
      dataFile.close();  // Close the file
      Serial.println("Data written to SD card.");
    } else {
      Serial.println("Error opening file for writing.");
    }
  } else {
    Serial.println("Error reading temperature.");
  }

  delay(5000);  // Wait 5 seconds before next reading
}
