#include <PZEM004Tv30.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

// PZEM004T Initialization (TX: Pin 6, RX: Pin 7)
PZEM004Tv30 pzem(6, 7);

// RTC Initialization
RTC_DS1307 rtc;

File dataFile;

// Variables for logging and energy tracking
int lastLogTime = 0;       // Tracks the last time data was logged
// const unsigned long logInterval = 3600000; // Log interval: 1 hour (3600000 ms)
int logInterval = 15000; // Log interval: 15 seconds (15000 ms)
float cumulativeEnergy = 0.0;        // Tracks total energy consumed

void setup() {
  Serial.begin(9600);

  // Initialize PZEM
  Serial.println("Initializing PZEM004T...");
  Serial.println("Hello");

  // Initialize RTC
  Serial.println("Initializing RTC...");
  if (!rtc.begin()) {
    Serial.println("RTC not found!");
    // while (1);
  }
  // if (!rtc.isrunning()) {
    Serial.println("RTC is not running, setting time...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile time
  // }

  // Initialize SD card
  Serial.println("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("SD card initialization failed!");
    // while (1);
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
  // Get current time
  DateTime now = rtc.now();

  // Read PZEM sensor data
  float energy = pzem.energy();
  if (isnan(energy)) {
    Serial.println("Error reading energy data!");
    return;
  }

  // Update cumulative energy
  cumulativeEnergy += energy;

  // Log data to SD card hourly
  if (millis() - lastLogTime >= logInterval) {
    lastLogTime = millis(); // Update log time

    // Format date and time for logging
    char timestamp[20];
    sprintf(timestamp, "%02d-%02d-%04d %02d:%02d:%02d",
            now.day(), now.month(), now.year(),
            now.hour(), now.minute(), now.second());

    // Prepare data string
    String logData = String(timestamp) + ", " + String(cumulativeEnergy, 3) + " kWh";

    // Print log data to Serial Monitor
    Serial.println("Logging data:");
    Serial.println(logData);

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

  // Display data on Serial Monitor for debugging
  Serial.print("Energy Consumed: ");
  Serial.print(cumulativeEnergy, 3);
  Serial.println(" kWh");
  delay(2000); // Wait 2 seconds before next reading
}
//-------------------------------------------------------------------------------------------------------
// reduced size but not working
// #include <PZEM004Tv30.h>
// #include <Wire.h>
// #include "RTClib.h"
// #include <SPI.h>
// #include <SD.h>

// // PZEM004T Initialization (TX: Pin 6, RX: Pin 7)
// PZEM004Tv30 pzem(6, 7);

// // RTC Initialization
// RTC_DS1307 rtc;

// // SD card initialization
// File dataFile;

// // Variables for logging and energy tracking
// unsigned long lastLogTime = 0;
// float cumulativeEnergy = 0.0;

// // Optimized buffer sizes
// char timestamp[20];  // Reducing the buffer size for timestamp
// char logData[50];    // Reducing the buffer size for log data

// void setup() {
//   Serial.begin(9600);

//   // Debugging log
//   Serial.println("Initializing PZEM004T...");
  
//   // Initialize RTC
//   if (!rtc.begin()) {
//     Serial.println("RTC not found! Check connections.");
//     while (1); // Halt execution if RTC is not found
//   }

//   if (!rtc.isrunning()) {
//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile time
//   }

//   // Initialize SD card
//   if (!SD.begin(10)) {
//     Serial.println("SD card initialization failed! Check connections or formatting.");
//     while (1); // Halt execution if SD card fails
//   }

//   // Check and create energy log file if not present
//   if (!SD.exists("energy_log.txt")) {
//     dataFile = SD.open("energy_log.txt", FILE_WRITE);
//     if (dataFile) {
//       dataFile.println("DateTime, Energy Consumed (kWh)"); // Write headers
//       dataFile.close();
//     }
//   }

//   // Check and create last_energy.txt file if not present
//   if (!SD.exists("last_energy.txt")) {
//     dataFile = SD.open("last_energy.txt", FILE_WRITE);
//     if (dataFile) {
//       dataFile.print(cumulativeEnergy, 3);
//       dataFile.close();
//     }
//   }
// }

// void loop() {
//   DateTime now = rtc.now();

//   // Read PZEM sensor data
//   float energy = pzem.energy();
//   if (isnan(energy)) {
//     Serial.println("Error reading energy data! Ensure PZEM is connected.");
//     return;
//   }

//   // Update cumulative energy
//   cumulativeEnergy += energy;

//   // Log data to SD card every 15 seconds
//   if (millis() - lastLogTime >= 15000) { // 15 seconds interval
//     lastLogTime = millis(); // Update log time

//     // Prepare timestamp string
//     snprintf(timestamp, sizeof(timestamp), "%02d-%02d-%04d %02d:%02d:%02d",
//              now.day(), now.month(), now.year(),
//              now.hour(), now.minute(), now.second());

//     // Prepare log data string
//     snprintf(logData, sizeof(logData), "%s, %.3f kWh", timestamp, cumulativeEnergy);

//     // Log data to SD card
//     dataFile = SD.open("energy_log.txt", FILE_WRITE);
//     if (dataFile) {
//       dataFile.println(logData);
//       dataFile.close();
//     }

//     // Save cumulative energy to last_energy.txt
//     dataFile = SD.open("last_energy.txt", FILE_WRITE);
//     if (dataFile) {
//       dataFile.print(cumulativeEnergy, 3);
//       dataFile.close();
//     }
//   }

//   // Display data on Serial Monitor for debugging
//   Serial.print("Energy Consumed: ");
//   Serial.print(cumulativeEnergy, 3);
//   Serial.println(" kWh");
//   delay(2000); // Wait 2 seconds before next reading
// }

//------------------------------------------------------------------------------------------------------
#include <PZEM004Tv30.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

// PZEM004T Initialization (TX: Pin 6, RX: Pin 7)
PZEM004Tv30 pzem(6, 7);

// RTC Initialization
RTC_DS1307 rtc;

// SD card initialization
File dataFile;

// Variables for logging and energy tracking
unsigned long lastLogTime = 0;
float cumulativeEnergy = 0.0;

// Optimized buffer sizes
char timestamp[20];  // Reducing the buffer size for timestamp
char logData[50];    // Reducing the buffer size for log data

void setup() {
  Serial.begin(9600);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("RTC not found! Check connections.");
    while (1); // Halt execution if RTC is not found
  }

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile time
  }

  // Initialize SD card
  if (!SD.begin(10)) {
    Serial.println("SD card initialization failed! Check connections or formatting.");
    while (1); // Halt execution if SD card fails
  }

  // Check and create energy log file if not present
  if (!SD.exists("energy_log.txt")) {
    dataFile = SD.open("energy_log.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println("DateTime, Energy Consumed (kWh)"); // Write headers
      dataFile.close();
      Serial.println("Energy log file created.");
    } else {
      Serial.println("Error creating energy_log.txt file!");
    }
  }

  // Check and create last_energy.txt file if not present
  if (!SD.exists("last_energy.txt")) {
    dataFile = SD.open("last_energy.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print(cumulativeEnergy, 3);
      dataFile.close();
      Serial.println("Last energy file created.");
    } else {
      Serial.println("Error creating last_energy.txt file!");
    }
  }
}

void loop() {
  DateTime now = rtc.now();

  // Read PZEM sensor data
  float energy = pzem.energy();
  if (isnan(energy)) {
    Serial.println("Error reading energy data! Ensure PZEM is connected.");
    return;
  }

  // Update cumulative energy
  cumulativeEnergy += energy;

  // Log data to SD card every 15 seconds
  if (millis() - lastLogTime >= 15000) { // 15 seconds interval
    lastLogTime = millis(); // Update log time

    // Prepare timestamp string (optimized buffer size)
    snprintf(timestamp, sizeof(timestamp), "%02d-%02d-%04d %02d:%02d:%02d",
             now.day(), now.month(), now.year(),
             now.hour(), now.minute(), now.second());

    // Prepare log data string (optimized buffer size)
    snprintf(logData, sizeof(logData), "%s, %.3f kWh", timestamp, cumulativeEnergy);

    // Log data to SD card
    dataFile = SD.open("energy_log.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(logData);
      dataFile.close();
      Serial.println("Data logged to energy_log.txt");
    } else {
      Serial.println("Error writing to energy_log.txt!");
    }

    // Save cumulative energy to last_energy.txt
    dataFile = SD.open("last_energy.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print(cumulativeEnergy, 3);
      dataFile.close();
      Serial.println("Data saved to last_energy.txt");
    } else {
      Serial.println("Error writing to last_energy.txt!");
    }
  }

  // Display data on Serial Monitor for debugging
  Serial.print("Energy Consumed: ");
  Serial.print(cumulativeEnergy, 3);
  Serial.println(" kWh");
  delay(2000); // Wait 2 seconds before next reading
}


