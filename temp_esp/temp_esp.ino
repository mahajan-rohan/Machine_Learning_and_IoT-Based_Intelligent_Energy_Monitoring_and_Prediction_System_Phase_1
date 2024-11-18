#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO4 (D2 on NodeMCU)
#define ONE_WIRE_BUS D4

// Setup a oneWire instance to communicate with the DS18B20
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup(void)
{
  // Start the Serial communication
  Serial.begin(9600);
  Serial.println("Initializing Temperature Sensor...");

  // Start up the DallasTemperature library
  sensors.begin();
}

void loop(void)
{ 
  // Send the command to get temperatures
  sensors.requestTemperatures(); 

  // Print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0)); // Index 0 assumes a single sensor
  Serial.println(" °C");

  // Print the temperature in Fahrenheit
  Serial.print("Temperature: ");
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.println(" °F");

  delay(500);
}
