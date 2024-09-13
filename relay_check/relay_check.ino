// Relay module connected to pin 7
int relayPin = 7;
int ledPin = LED_BUILTIN;  // Built-in LED on Arduino (usually pin 13)
void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}
  void loop() {
      digitalWrite(relayPin,LOW);
      digitalWrite(ledPin, HIGH);
      delay(2000);
      digitalWrite(relayPin, HIGH);
      digitalWrite(ledPin, LOW);
      delay(2000);
}
