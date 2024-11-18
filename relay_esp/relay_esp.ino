#define RELAY_PIN 10 // GPIO10

void setup() {
    pinMode(RELAY_PIN, OUTPUT); // Set GPIO10 as output
    digitalWrite(RELAY_PIN, LOW); // Ensure relay is OFF initially
    Serial.begin(9600);
}

void loop() {
    Serial.println("Relay ON");
    digitalWrite(RELAY_PIN, HIGH); // Turn ON relay
    delay(1000); // Wait for 1 second

    Serial.println("Relay OFF");
    digitalWrite(RELAY_PIN, LOW); // Turn OFF relay
    delay(1000); // Wait for 1 second
}
