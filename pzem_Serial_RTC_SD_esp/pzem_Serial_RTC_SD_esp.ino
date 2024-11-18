#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>

// PZEM Communication Pins
#define PZEM_RX D3 // GPIO0
#define PZEM_TX D4 // GPIO2

// SD Card Pin
#define SD_CS_PIN D8

// PZEM Instance
SoftwareSerial pzemSerial(PZEM_RX, PZEM_TX);
PZEM004Tv30 pzem(pzemSerial);

// Variables
float cumulativeEnergy = 0.0; // Energy in kWh
unsigned long previousMillis = 0;
const unsigned long interval = 2000; // 2 seconds

void setup() {
    Serial.begin(115200); // Initialize Serial Monitor for RTC data
    pzemSerial.begin(9600); // Initialize PZEM communication

    // SD Card Initialization
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD Card initialization failed!");
        while (1);
    }
    Serial.println("SD Card initialized.");

    // Restore cumulative energy from SD card
    cumulativeEnergy = restoreEnergyFromSD();
    Serial.print("Restored Energy: ");
    Serial.println(cumulativeEnergy, 3);
}

void loop() {
    // Read RTC data from Arduino
    String rtcData = readRTCFromArduino();

    // Read energy data from PZEM
    float energy = pzem.energy();
    if (!isnan(energy)) {
        cumulativeEnergy += energy; // Update cumulative energy
    } else {
        Serial.println("Error reading PZEM energy data.");
    }

    // Prepare log entry
    String logEntry = rtcData + " " + String(cumulativeEnergy, 3);
    Serial.println(logEntry); // Debug output

    // Log data to SD card
    logEnergyToSD(logEntry);

    // Save cumulative energy periodically
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        saveCumulativeEnergyToSD(cumulativeEnergy);
    }

    delay(1000); // Delay for 1 second
}

// Function to read RTC data from Arduino
String readRTCFromArduino() {
    if (Serial.available()) {
        String rtcData = Serial.readStringUntil('\n');
        rtcData.trim(); // Remove any extra spaces or newline characters
        return rtcData;
    }
    return "No RTC Data";
}

// Function to log energy data to SD card
void logEnergyToSD(String logData) {
    File dataFile = SD.open("energy_log.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.println(logData);
        dataFile.close();
        Serial.println("Data logged successfully!");
    } else {
        Serial.println("Error writing to log file!");
    }
}

// Function to save cumulative energy to SD card
void saveCumulativeEnergyToSD(float energy) {
    File dataFile = SD.open("last_energy.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.seek(0); // Overwrite existing data
        dataFile.print(energy, 3);
        dataFile.close();
        Serial.println("Cumulative energy saved!");
    } else {
        Serial.println("Error writing to last energy file!");
    }
}

// Function to restore cumulative energy from SD card
float restoreEnergyFromSD() {
    if (!SD.exists("last_energy.txt")) {
        Serial.println("No previous energy log found. Starting fresh.");
        return 0.0;
    }

    File file = SD.open("last_energy.txt", FILE_READ);
    if (file) {
        String lastEnergyStr = file.readStringUntil('\n');
        file.close();
        return lastEnergyStr.toFloat(); // Convert string to float
    }

    Serial.println("Error reading last energy file!");
    return 0.0;
}

