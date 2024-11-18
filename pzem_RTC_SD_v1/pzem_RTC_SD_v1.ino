#include <PZEM004Tv30.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

// PZEM004T Initialization (TX: Pin 6, RX: Pin 7)
PZEM004Tv30 pzem(6, 7); // No need for 'begin()' function, just initialize like this

// RTC Initialization
RTC_DS1307 rtc;

File dataFile;

// Variables for logging and energy tracking
unsigned long lastLogTime = 0;       // Tracks the last time data was logged
int logInterval = 15000;              // Log interval: 15 seconds (15000 ms)
float cumulativeEnergy = 0.0;        // Tracks total energy consumed

// Timing variables
unsigned long previousMillis = 0;    // For non-blocking delay
const long interval = 2000;           // 2 seconds for displaying energy

void setup() {
  Serial.begin(9600);

  // Initialize PZEM - No need for a 'begin' method
  Serial.println("Initializing PZEM004T...");
  
  // Initialize RTC
  Serial.println("Initializing RTC...");
  unsigned long rtcStartTime = millis();
  while (!rtc.begin() && (millis() - rtcStartTime) < 5000) {
    Serial.println("RTC not found, retrying...");
    delay(100);  // Short delay to avoid rapid retries
  }
  if (millis() - rtcStartTime >= 5000) {
    Serial.println("RTC initialization failed!");
    while (1);  // Halt the program if RTC fails to initialize
  }

  // Initialize SD card
  Serial.println("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("SD card initialization failed!");
    while (1);  // Halt the program if SD card fails to initialize
  }
  Serial.println("SD card initialized.");

  // Read last saved energy value from SD card
  if (SD.exists("last_energy.txt")) {
    dataFile = SD.open("last_energy.txt", FILE_READ);
    if (dataFile) {
      String lastEnergy = dataFile.readStringUntil('\n');
      cumulativeEnergy = lastEnergy.toFloat();
      Serial.print("Resuming from last energy value: ");
      Serial.println(cumulativeEnergy, 3);
      dataFile.close();
    } else {
      Serial.println("Error reading last energy file!");
    }
  } else {
    Serial.println("No previous energy data found. Starting from 0.");
    cumulativeEnergy = 0.0;
  }
}

void loop() {
  // Feed the watchdog to prevent resets
  ESP.wdtFeed();  

  // Get current time
  DateTime now = rtc.now();

  // Read PZEM sensor data
  float energy = pzem.energy();  // Use this method to get energy reading from PZEM
  if (isnan(energy)) {
    Serial.println("Error reading energy data!");
    return;
  }

  // Update cumulative energy
  cumulativeEnergy += energy;

  // Log data to SD card at regular intervals
  unsigned long currentMillis = millis();
  if (currentMillis - lastLogTime >= logInterval) {
    lastLogTime = currentMillis; // Update log time

    // Format date and time for logging
    char timestamp[20];
    sprintf(timestamp, "%02d-%02d-%04d %02d:%02d:%02d",
            now.day(), now.month(), now.year(),
            now.hour(), now.minute(), now.second());

    // Prepare data string
    String logData = String(timestamp) + ", " + String(cumulativeEnergy, 3) + " kWh";

    // Log data to SD card
    dataFile = SD.open("energy_log.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(logData);
      dataFile.close();
      Serial.println("Data logged successfully!");
    } else {
      Serial.println("Error writing to log file!");
    }

    // Save cumulative energy to last_energy.txt
    dataFile = SD.open("last_energy.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print(cumulativeEnergy, 3);
      dataFile.close();
      Serial.println("Cumulative energy saved!");
    } else {
      Serial.println("Error writing to last energy file!");
    }
  }

  // Display data on Serial Monitor for debugging every 2 seconds
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.print("Energy Consumed: ");
    Serial.print(cumulativeEnergy, 3);
    Serial.println(" kWh");
  }

  // Feed the watchdog after each loop iteration
  ESP.wdtFeed();
}
