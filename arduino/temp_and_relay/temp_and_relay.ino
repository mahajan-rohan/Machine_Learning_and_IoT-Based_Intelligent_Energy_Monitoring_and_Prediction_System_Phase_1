#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire for temperature sensor on digital pin 2
#define ONE_WIRE_BUS 2
// Relay module connected to pin 7
int relayPin = 7;
int ledPin = LED_BUILTIN;  // Built-in LED on Arduino (pin 13)

// Temperature threshold value (in Celsius)
float temperatureThreshold = 40.0;  // Relay OFF when temperature >= 30°C, ON when < 30°C

// OneWire instance and DallasTemperature object
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void) {
  sensors.begin();              // Start temperature sensor library
  Serial.begin(9600);           // Start serial communication
  pinMode(relayPin, OUTPUT);    // Set relay pin as output
  pinMode(ledPin, OUTPUT);      // Set LED pin as output
  digitalWrite(relayPin, LOW);  // Initialize relay as ON
  digitalWrite(ledPin, LOW);    // Turn LED off initially
}

void loop(void) {
  sensors.requestTemperatures();                    // Get temperature from the sensor
  float temperatureC = sensors.getTempCByIndex(0);  // Read in Celsius

  // Print temperature in Celsius and Fahrenheit
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  //Serial.print((char)176);  // degree symbol
  Serial.println("C");

  // Relay control logic with a single threshold
  if (temperatureC >= temperatureThreshold) {
    // Turn relay OFF when temperature exceeds or equals the threshold
    digitalWrite(relayPin, LOW);  // Relay OFF
    digitalWrite(ledPin, LOW);    // LED ON to indicate relay OFF
    Serial.println("Relay OFF - High temperature");
  } else {
    // Turn relay ON when temperature drops below the threshold
    digitalWrite(relayPin, HIGH);  // Relay ON
    digitalWrite(ledPin, HIGH);    // LED OFF to indicate relay ON
    // Serial.println("Relay ON - Low temperature");
  }

  delay(500);  // Wait 500ms before next loop
}
