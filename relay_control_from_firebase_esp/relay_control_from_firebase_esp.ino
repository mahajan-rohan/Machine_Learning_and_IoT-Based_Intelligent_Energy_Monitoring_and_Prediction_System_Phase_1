#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Firebase Configuration
#define API_KEY "XXXXX"
#define DATABASE_URL "XXXXX"

// Pins
#define RELAY_PIN 10 // GPIO10
#define LED_PIN 2    // Onboard LED is GPIO2 (built-in)

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signup = false;

void setup() {
  // Serial Communication
  Serial.begin(115200);

  // WiFi Connection
  WiFi.disconnect();
  delay(2000);
  Serial.println("Start Connection");
  WiFi.begin("JAI GANESH_SWAMI", "Atharva@123");
  while (!(WiFi.status() == WL_CONNECTED)) {
    delay(200);
    Serial.print("..");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("Device IP: ");
  Serial.println(WiFi.localIP().toString());

  // Firebase Initialization
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase Connected");
    signup = true;
  } else {
    Serial.println(config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Pin Configuration
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Initialize Relay and LED as OFF
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  if (Firebase.ready() && signup) {
    // Fetch relayState from Firebase
    if (Firebase.RTDB.getString(&fbdo, "/random/data")) {
      String relayState = fbdo.stringData();
      Serial.print("Relay State from Firebase: ");
      Serial.println(relayState);

      // Control Relay and LED based on relayState
      if (relayState == "true") {
        digitalWrite(RELAY_PIN, LOW); // Relay OFF
        digitalWrite(LED_PIN, LOW);  // LED OFF
        Serial.println("Relay OFF, LED OFF");
      } else if (relayState == "false") {
        digitalWrite(RELAY_PIN, HIGH); // Relay ON
        digitalWrite(LED_PIN, HIGH);  // LED ON
        Serial.println("Relay ON, LED ON");
      }
    } else {
      Serial.print("Firebase Error: ");
      Serial.println(fbdo.errorReason());
    }
  }
  delay(1000); 
}
