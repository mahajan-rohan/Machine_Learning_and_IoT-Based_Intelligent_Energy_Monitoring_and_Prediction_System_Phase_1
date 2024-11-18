// Temp pin : 4
//GSM middle right -> 3
//GSM middle left -> 2

// #include <OneWire.h>
// #include <DallasTemperature.h>
// #include <SoftwareSerial.h>

// // Data wire is plugged into digital pin 2 on the Arduino
// #define ONE_WIRE_BUS 4
// #define THRESHOLD_TEMP 31.0  // Set your threshold temperature here

// // Setup a oneWire instance to communicate with any OneWire device
// OneWire oneWire(ONE_WIRE_BUS);	
// DallasTemperature sensors(&oneWire);

// // GSM module connected to pins 2 (RX) and 3 (TX)
// SoftwareSerial SIM900A(2, 3);

// bool alertSent = false;

// void setup(void)
// {
//   sensors.begin();	// Start up the library
//   Serial.begin(9600);
//   SIM900A.begin(9600);  // GSM Module Baud rate
//   Serial.println("System Ready");
// }

// void loop(void)
// { 
//   sensors.requestTemperatures(); 

//   Serial.print("Temperature: ");
//   Serial.print(sensors.getTempCByIndex(0));
//   Serial.print((char)176); // shows degrees character
//   Serial.println("C");

//   if ((sensors.getTempCByIndex(0)) > THRESHOLD_TEMP && !alertSent) {
//     sendAlertMessage();
//     alertSent = true;  // Ensure the message is sent only once
//   }

//   delay(500);
// }

// void sendAlertMessage() {
//   Serial.println("Sending Alert Message...");
//   SIM900A.println("AT+CMGF=1");  // Text Mode initialization
//   delay(1000);
//   SIM900A.println("AT+CMGS=\"+919028762236\"\r");  // Receiver's Mobile Number
//   delay(1000);
//   SIM900A.println("Alert! Temperature has exceeded the threshold.");  // Message content
//   delay(100);
//   SIM900A.println((char)26);  // End AT command with a ^Z, ASCII code 26
//   delay(1000);
//   Serial.println("Alert Message sent successfully");
// }


//------------------------------------------------------------------------------------------------------------

// #include <OneWire.h>
// #include <DallasTemperature.h>
// #include <SoftwareSerial.h>

// // Data wire is plugged into digital pin 2 on the Arduino
// #define ONE_WIRE_BUS 4
// #define THRESHOLD_TEMP 30.0  // Set your threshold temperature here

// // Setup a oneWire instance to communicate with any OneWire device
// OneWire oneWire(ONE_WIRE_BUS);	
// DallasTemperature sensors(&oneWire);

// // GSM module connected to pins 2 (RX) and 3 (TX)
// SoftwareSerial SIM900A(2, 3);

// bool alertSent = false;
// unsigned long alertTime = 0;
// bool waitingForConfirmation = false;

// void setup(void)
// {
//   sensors.begin();	// Start up the library
//   Serial.begin(9600);
//   SIM900A.begin(9600);  // GSM Module Baud rate
//   Serial.println("System Ready");
// }

// void loop(void)
// { 
//   sensors.requestTemperatures(); 
//   float temperatureC = sensors.getTempCByIndex(0);

//   Serial.print("Temperature: ");
//   Serial.print(temperatureC);
//   Serial.print((char)176); // shows degrees character
//   Serial.println("C");

//   if (temperatureC > THRESHOLD_TEMP && !alertSent) {
//     sendAlertMessage();
//     alertSent = true;  // Ensure the message is sent only once
//     alertTime = millis();
//     waitingForConfirmation = true;
//   }

//   if (waitingForConfirmation) {
//     if (millis() - alertTime > 30000) {  // 30 seconds timeout
//       sendSOSMessage();
//       waitingForConfirmation = false;
//     } else if (SIM900A.available() > 0) {
//       String response = SIM900A.readString();
//       if (response.indexOf("SOS") != -1) {
//         Serial.println("Confirmation received.");
//         waitingForConfirmation = false;
//       }
//     }
//   }

//   delay(500);
// }

// void sendAlertMessage() {
//   Serial.println("Sending Alert Message...");
//   SIM900A.println("AT+CMGF=1");  // Text Mode initialization
//   delay(1000);
//   SIM900A.println("AT+CMGS=\"+919028762236\"\r");  // Receiver's Mobile Number
//   delay(1000);
//   SIM900A.println("Alert! Temperature has exceeded the threshold. Reply with 'SOS' to confirm.");  // Message content
//   delay(100);
//   SIM900A.println((char)26);  // End AT command with a ^Z, ASCII code 26
//   delay(1000);
//   Serial.println("Alert Message sent successfully");
// }

// void sendSOSMessage() {
//   Serial.println("Sending SOS Message...");
//   SIM900A.println("AT+CMGF=1");  // Text Mode initialization
//   delay(1000);
//   SIM900A.println("AT+CMGS=\"+919028762236\"\r");  // Receiver's Mobile Number
//   delay(1000);
//   SIM900A.println("SOS! No response received. Immediate action required.");  // Message content
//   delay(100);
//   SIM900A.println((char)26);  // End AT command with a ^Z, ASCII code 26
//   delay(1000);
//   Serial.println("SOS Message sent successfully");
// }


//----------------------------------------------------------------------------------------------------------------------------------

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 4
#define THRESHOLD_TEMP 30.0  // Set your threshold temperature here

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);	
DallasTemperature sensors(&oneWire);

// GSM module connected to pins 2 (RX) and 3 (TX)
SoftwareSerial SIM900A(2, 3);

bool alertSent = false;
unsigned long alertTime = 0;
bool waitingForConfirmation = false;

void setup(void)
{
  sensors.begin();	// Start up the library
  Serial.begin(9600);
  SIM900A.begin(9600);  // GSM Module Baud rate
  Serial.println("System Ready");
}

void loop(void)
{ 
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print((char)176); // shows degrees character
  Serial.println("C");

  if (temperatureC > THRESHOLD_TEMP && !alertSent) {
    sendAlertMessage();
    alertSent = true;  // Ensure the message is sent only once
    alertTime = millis();
    waitingForConfirmation = true;
  }

  if (waitingForConfirmation) {
    if (millis() - alertTime > 30000) {  // 30 seconds timeout
      sendSOSMessage();
      waitingForConfirmation = false;
    } else if (SIM900A.available() > 0) {
      String response = SIM900A.readString();
      if (response.indexOf("SOS") != -1) {
        Serial.println("Confirmation received.");
        sendEmergencyMessages();
        waitingForConfirmation = false;
      }
    }
  }

  delay(500);
}

void sendAlertMessage() {
  Serial.println("Sending Alert Message...");
  SIM900A.println("AT+CMGF=1");  // Text Mode initialization
  delay(1000);
  SIM900A.println("AT+CMGS=\"+919028762236\"\r");  // Receiver's Mobile Number
  delay(1000);
  SIM900A.println("Alert! Temperature has exceeded the threshold. Reply with 'SOS' to confirm.");  // Message content
  delay(100);
  SIM900A.println((char)26);  // End AT command with a ^Z, ASCII code 26
  delay(1000);
  Serial.println("Alert Message sent successfully");
}

void sendSOSMessage() {
  Serial.println("Sending SOS Message...");
  SIM900A.println("AT+CMGF=1");  // Text Mode initialization
  delay(1000);
  SIM900A.println("AT+CMGS=\"+919028762236\"\r");  // Receiver's Mobile Number
  delay(1000);
  SIM900A.println("SOS! No response received. Immediate action required.");  // Message content
  delay(100);
  SIM900A.println((char)26);  // End AT command with a ^Z, ASCII code 26
  delay(1000);
  Serial.println("SOS Message sent successfully");
}

void sendEmergencyMessages() {
  // Message to the nearest fire department
  Serial.println("Sending message to the nearest fire department...");
  SIM900A.println("AT+CMGF=1");  // Text Mode initialization
  delay(1000);
  SIM900A.println("AT+CMGS=\"+919420777040\"\r");  // Fire department's Mobile Number
  delay(1000);
  SIM900A.println("Emergency! Short circuit detected at YamgekarColony. Contact: +919028762236");  // Message content
  delay(100);
  SIM900A.println((char)26);  // End AT command with a ^Z, ASCII code 26
  delay(1000);
  Serial.println("Message to fire department sent successfully");

  // Message to an emergency contact
  Serial.println("Sending message to emergency contact...");
  SIM900A.println("AT+CMGF=1");  // Text Mode initialization
  delay(1000);
  SIM900A.println("AT+CMGS=\"+918668443862\"\r");  // Emergency contact's Mobile Number
  delay(1000);
  SIM900A.println("Alert! Temperature has exceeded the threshold. Immediate action required. Contact: +919028762236");  // Message content
  delay(100);
  SIM900A.println((char)26);  // End AT command with a ^Z, ASCII code 26
  delay(1000);
  Serial.println("Message to emergency contact sent successfully");
}
