#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

// Define PZEM communication pins
#define PZEM_RX D3 // GPIO0
#define PZEM_TX D4 // GPIO2

// Create SoftwareSerial instance for communication
SoftwareSerial pzemSerial(PZEM_RX, PZEM_TX);

// Create PZEM object
PZEM004Tv30 pzem(pzemSerial);

void setup() {
    // Initialize Serial Monitor
    Serial.begin(9600);
    delay(1000); // Allow some time for initialization

    // Initialize PZEM-004T
    Serial.println("Initializing PZEM-004T...");
}

void loop() {
    // Read voltage, current, power, and energy from PZEM
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();

    // Check if readings are valid
    if (isnan(voltage) || isnan(current) || isnan(power) || isnan(energy)) {
        Serial.println("Error reading PZEM data");
    } else {
        // Display readings on Serial Monitor
        Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
        Serial.print("Current: "); Serial.print(current); Serial.println(" A");
        Serial.print("Power: "); Serial.print(power); Serial.println(" W");
        Serial.print("Energy: "); Serial.print(energy); Serial.println(" kWh");
        Serial.println("---------------------------------");
    }

    delay(1000); // Wait 1 second before the next reading
}
