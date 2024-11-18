#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

// Define PZEM communication pins
#define PZEM_RX D3 // GPIO0
#define PZEM_TX D4 // GPIO2

// Create SoftwareSerial instance for PZEM communication
SoftwareSerial pzemSerial(PZEM_RX, PZEM_TX);

// Create PZEM object
PZEM004Tv30 pzem(pzemSerial);

String readRTCData() {
  static String inputString = ""; // Buffer to store incoming data
  while (Serial.available()) {
    char c = Serial.read();       // Read one character at a time
    if (c == '<') {               // Start of the message
      inputString = "";           // Clear the buffer
    } else if (c == '>') {        // End of the message
      return inputString;         // Return the complete message
    } else {
      inputString += c;           // Append character to the buffer
    }
  }
  return "";                      // Return empty if no complete message
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  delay(1000); // Allow some time for initialization

  // Initialize PZEM-004T
  Serial.println("Initializing PZEM-004T...");
}

void loop() {
  // Read RTC data from Arduino
  String rtcData = readRTCData();
  if (rtcData != "") {
    Serial.print("RTC Data: ");
    Serial.println(rtcData); // Print RTC data received
  }

  // Read PZEM-004T sensor data
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();

  // Check if PZEM readings are valid
  if (isnan(voltage) || isnan(current) || isnan(power) || isnan(energy)) {
    Serial.println("Error reading PZEM data");
  } else {
    // Display PZEM readings on Serial Monitor
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
    Serial.print("Current: "); Serial.print(current); Serial.println(" A");
    Serial.print("Power: "); Serial.print(power); Serial.println(" W");
    Serial.print("Energy: "); Serial.print(energy); Serial.println(" kWh");
    Serial.println("---------------------------------");
  }

  delay(1000); // Wait 1 second before the next loop
}
