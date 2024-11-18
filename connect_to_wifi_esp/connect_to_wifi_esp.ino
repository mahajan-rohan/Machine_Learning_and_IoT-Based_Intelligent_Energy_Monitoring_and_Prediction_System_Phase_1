#include <ESP8266WiFi.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.disconnect();
  delay(2000);
  Serial.println("Start Connection");
  WiFi.begin("Mahajan","#Rohan@25&");
  while((!(WiFi.status()==WL_CONNECTED))){
    delay(200);
    Serial.print("..");
  }
  Serial.println("Connected");
  Serial.print("Device IP : ");
  Serial.println((WiFi.localIP().toString()));

}

void loop() {
  // put your main code here, to run repeatedly:

}
