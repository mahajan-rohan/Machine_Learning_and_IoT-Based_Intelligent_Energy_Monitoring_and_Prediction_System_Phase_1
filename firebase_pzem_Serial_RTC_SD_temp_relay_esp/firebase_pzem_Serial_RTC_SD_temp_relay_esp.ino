#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// WiFi and Firebase Configurations
#define API_KEY "AIzaSyBaGCN2sjd9JSyosAj0EJFqbKPb8Y-13lY"
#define DATABASE_URL "http://relay-cf260-default-rtdb.firebaseio.com/"
#define WIFI_SSID "Mahajan"
#define WIFI_PASSWORD "#Rohan@25&"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signup = false;

// PZEM Communication Pins
#define PZEM_RX D3 // GPIO0
#define PZEM_TX D4 // GPIO2

// SD Card Pin
// #define SD_CS_PIN D8
#undef SD_CS_PIN
#define SD_CS_PIN D8


// Temperature Sensor
#define TEMP_SENSOR_PIN D0 // Temperature sensor on D0
#define TEMP_THRESHOLD 30.0 // Temperature threshold in °C

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

// Relay
#define RELAY_PIN 10 // GPIO10 for the relay

// PZEM Instance
SoftwareSerial pzemSerial(PZEM_RX, PZEM_TX);
PZEM004Tv30 pzem(pzemSerial);

// Variables
float cumulativeEnergy = 0.0; // Energy in kWh
unsigned long previousMillis = 0;
const unsigned long interval = 2000; // 2 seconds
unsigned long firebaseUpdateInterval = 15000; // 15 seconds
unsigned long lastFirebaseUpdate = 0;

void setup() {
    Serial.begin(115200); // Initialize Serial Monitor
    pzemSerial.begin(9600); // Initialize PZEM communication

    // WiFi Connection
    WiFi.disconnect();
    delay(2000);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print("..");
    }
    Serial.println("\nWiFi Connected!");
    Serial.print("Device IP: ");
    Serial.println(WiFi.localIP());

    // Firebase Initialization
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    if (Firebase.signUp(&config, &auth, "", "")) {
        Serial.println("Firebase Connected");
        signup = true;
    } else {
        Serial.println(config.signer.signupError.message.c_str());
    }

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

    // Initialize Temperature Sensor
    sensors.begin();
    Serial.println("Temperature Sensor Initialized.");

    // Initialize Relay
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Ensure relay is OFF initially
    Serial.println("Relay Initialized.");
}

void loop() {
    // Read RTC data from Arduino with delimiter
    String rtcData = readRTCData();

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

    // Temperature Control
    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(0); // Get temperature in Celsius

    // Print temperature readings
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    // Relay control based on temperature
    if (temperature > TEMP_THRESHOLD) {
        Serial.println("Temperature above threshold. Turning Relay ON.");
        digitalWrite(RELAY_PIN, HIGH); // Turn ON relay
    } else {
        Serial.println("Temperature below threshold. Turning Relay OFF.");
        digitalWrite(RELAY_PIN, LOW); // Turn OFF relay
    }

    // Push data to Firebase every 15 seconds
    if (currentMillis - lastFirebaseUpdate >= firebaseUpdateInterval) {
        lastFirebaseUpdate = currentMillis;
        pushDataToFirebase(rtcData, cumulativeEnergy);
    }

    delay(1000); // Delay for 1 second before the next reading
}

// Function to push data to Firebase
void pushDataToFirebase(String time, float energy) {
    if (Firebase.ready() && signup) {
        String path = "userid/" + String(millis() / firebaseUpdateInterval) + "/";
        if (Firebase.RTDB.setString(&fbdo, (path + "time").c_str(), time) &&
            Firebase.RTDB.setFloat(&fbdo, (path + "unit").c_str(), energy)){
            Serial.println("Data pushed to Firebase!");
        } else {
            Serial.print("Firebase error: ");
            Serial.println(fbdo.errorReason());
        }
    }
}

// Function to read RTC data from Arduino with delimiter
String readRTCData() {
    static String inputString = ""; // Buffer to store incoming data
    while (Serial.available()) {
        char c = Serial.read();       // Read one character at a time
        if (c == '<') {               // Start of the message
            inputString = "";         // Clear the buffer
        } else if (c == '>') {        // End of the message
            return inputString;       // Return the complete message
        } else {
            inputString += c;         // Append character to the buffer
        }
    }
    return "";                        // Return empty if no complete message
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
