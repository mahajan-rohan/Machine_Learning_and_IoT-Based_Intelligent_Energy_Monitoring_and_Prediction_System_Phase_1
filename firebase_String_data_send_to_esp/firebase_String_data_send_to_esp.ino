#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyBaGCN2sjd9JSyosAj0EJFqbKPb8Y-13lY"
#define DATABASE_URL "http://relay-cf260-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signup=false;

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

  config.api_key=API_KEY;
  config.database_url=DATABASE_URL;
  if(Firebase.signUp(&config,&auth,"","")){
    Serial.println("Firebase Connected");
    signup=true;
  } else {
    Serial.println(config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);

}
void loop() {
  // put your main code here, to run repeatedly:
  if(Firebase.ready() && signup){
    float x = 0;
    Firebase.RTDB.getString(&fbdo,"/random/data");
    String relayState = fbdo.stringData();
    Serial.println(relayState);
  }
  delay(1000);
}











