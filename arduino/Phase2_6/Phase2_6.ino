#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>

#include <EEPROM.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

PZEM004Tv30 pzem(D1,D2);//Rx/Tx   Connect RX->D2 & TX->D1

#define UPDATE_INTERVAL 1000  // Update every 5 seconds

#define EEPROM_SIZE 4096  // Increase EEPROM size
#define TOTAL_ENERGY_ADDRESSES 3  // Number of EEPROM addresses for wear leveling
#define LAST_ADDRESS_MARKER 1000  // EEPROM address for storing the last used address
#define BALANCE_MARKER 2000
#define TEMP_BALANCE_MARKER 3000

float totalEnergy = 0.0;
int currentAddress = 0;
int rechargeUnits=0;
String status="";
unsigned long previousMillis = 0;
float balanceWH=0.0;

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Firebase Credentials
#define API_KEY "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
#define DATABASE_URL "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define USER_EMAIL "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
#define USER_PASSWORD "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define Relay1 14  //D5 pin
#define Relay2 12   //D6 pin
#define Relay3 13  //D7 pin
#define Relay4 15  //D8 pin

#define switchPin1 D0
#define switchPin2 D3
#define switchPin3 D4

String val1 = "";  // Variable to store incoming data from firebase for device 1
String val2 = "";  // Variable to store incoming data from firebase for device 2
String val3 = "";  // Variable to store incoming data from firebase for device 3
String val4 = "";  // Variable to store incoming data from firebase for device 4

String uid;
String ipAddress;
String path;
IPAddress ip;

float totalBalance=0.0;
int wifiFlag=1;
void setup() {
  Serial.begin(9600);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  pinMode(switchPin3, INPUT_PULLUP);

  if(wifiFlag==0){
    Serial.println("WiFi Connected: With Internet Mode");
    // withInternet();
    checkBtnStatus();
  }else{
    Serial.println("WiFi not connected: Without Internet Mode");
    withoutInternet();
  }

  connectToWiFi();

  if(WiFi.status()==WL_CONNECTED){
    if(wifiFlag==1)
      wifiFlag=0;
  }else{
    if(wifiFlag==0)
      wifiFlag=1;
  }

  Serial.println("Connected to WiFi");
  Serial.print(WiFi.localIP().toString());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;
  Firebase.begin(&config, &auth);

  Serial.println("Getting User UID");
  while ((auth.token.uid)=="") {
    Serial.print('.');
    delay(1000);
  }

  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.print(uid);

  ip = WiFi.localIP();
  // ipAddress = ipToString(ip);
  path = "/Users/" + uid + "/ip";

  // Retrieve the last used EEPROM address
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(LAST_ADDRESS_MARKER, currentAddress);
  EEPROM.end();

  // If the value is invalid, set it to the default starting address
  if (currentAddress < 0 || currentAddress >= TOTAL_ENERGY_ADDRESSES) {
    currentAddress = 0;
  }

  // Retrieve the latest total energy from EEPROM at the start
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(currentAddress * sizeof(float), totalEnergy);
  EEPROM.end();

  if (isnan(totalEnergy)) {
    totalEnergy = 0.0;  // Or any other default value
  }

  Serial.print("Total Energy: ");
  Serial.println(totalEnergy);

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(BALANCE_MARKER , totalBalance);
  EEPROM.end();

  if(isnan(totalBalance)){
    totalBalance=0.0;
  }
  Serial.print("Total Balance: ");
  Serial.println(totalBalance);

  Serial.println("Initialization complete");

}

void eraseUsedAddresses() {
  // Erase only the used addresses (continuous block starting from 0)
  for (int i = 0; i < TOTAL_ENERGY_ADDRESSES; ++i) {
    EEPROM.write(i * sizeof(float), 0);
  }
  EEPROM.commit();
}

void updateTotalEnergy() {
  unsigned long currentMillis = millis();
  unsigned long deltaTime = currentMillis - previousMillis;
  previousMillis = currentMillis;

  float currentPower = pzem.power();  // Power in watts
  float currentEnergy = (currentPower * deltaTime / 1000.0) / 3600.0; // Energy in watt-hours

  if (!isnan(currentPower) && currentPower >= 0.0) {
    totalEnergy += currentEnergy;

    // Implement wear leveling by rotating EEPROM addresses
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(currentAddress * sizeof(float), totalEnergy);
    EEPROM.commit();
    EEPROM.end();

    currentAddress = (currentAddress + 1) % TOTAL_ENERGY_ADDRESSES;

    // Store the current EEPROM address
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(LAST_ADDRESS_MARKER, currentAddress);
    EEPROM.commit();
    EEPROM.end();

    Serial.println("Current Power: " + String(currentPower) + " W");
    Serial.println("Energy This Interval: " + String(currentEnergy) + " Wh");
    Serial.println("Total Energy: " + String(totalEnergy) + " Wh");

    balanceWH=totalBalance*1000.0;
    balanceWH-=currentEnergy;

    Serial.print("BalanceWH: ");
    Serial.println(balanceWH);

    if(balanceWH<=0.0){
      digitalWrite(Relay4, HIGH);
      val4="OFF";
      if(Firebase.ready()){
        Firebase.RTDB.setString(&fbdo,"/Users/"+uid+"/Devices/Device 4","OFF");
      }
    }

    totalBalance=balanceWH/1000.0;

    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(BALANCE_MARKER, totalBalance);
    EEPROM.commit();
    EEPROM.end();

    Serial.print("Total Balance: ");
    Serial.println(totalBalance);

    float totalUnitsConsumed = totalEnergy / 1000.0; // Convert watt-hours to kilowatt-hours
    Serial.println("Total Units Consumed: " + String(totalUnitsConsumed) + " kWh");

    if (Firebase.ready()) {
      // Assuming 'fbdo' is an instance of FirebaseData
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Readings/CurrentPower", String(currentPower));
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Readings/TotalEnergy", String(totalEnergy));
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Readings/TotalUnitsConsumed", String(totalUnitsConsumed));
      Firebase.RTDB.setFloat(&fbdo,"/Users/"+uid+"/Readings/Balance",totalBalance);
      // You can also store other values like currentEnergy, etc.
    }
  } else {
    Serial.println("Error reading current power. Skipping update.");
  }
}

void checkRecharge(){
  if(Firebase.ready()){
    if (Firebase.RTDB.getString(&fbdo, "/Users/" + uid + "/Readings/Status")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_string) {
        status = fbdo.to<String>();
      } else {
        Serial.println(fbdo.errorReason());
      }
    }

    if (Firebase.RTDB.getInt(&fbdo, "/Users/" + uid + "/Readings/rechargeUnits")) {
      if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_integer) {
        rechargeUnits = fbdo.to<int>();
      } else {
        Serial.println(fbdo.errorReason());
      }
    }

    Serial.print("Status: ");
    Serial.println(status);

    Serial.print("Recharged Units: ");
    Serial.println(rechargeUnits);
  }

  if(status.equals("true") && rechargeUnits>0.0){
    totalBalance=totalBalance+rechargeUnits;

    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(BALANCE_MARKER, totalBalance);
    EEPROM.commit();
    EEPROM.end();

    if(Firebase.ready()){
      Firebase.RTDB.setFloat(&fbdo, "/Users/" + uid + "/Readings/rechargeUnits", 0.0);
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Readings/Status", "false");
    }

    Serial.println("Recharged successfully");

  }else{
    if(Firebase.ready()){
      Firebase.RTDB.setFloat(&fbdo,"/Users/" + uid + "/Readings/Balance",totalBalance);
    }
    Serial.println(totalBalance);
  }
}

void connectToWiFi() {
  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("ESP8266")) {
    Serial.print("Failed to connect");
    delay(1000);
    ESP.reset();
    delay(5000);
  }
}

void updateDB(){
  if(Firebase.ready()){
    if(val1.equals("ON")){
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Devices/Device 1", "ON");
    }else{
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Devices/Device 1", "OFF");
    }

    if(val2.equals("ON")){
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Devices/Device 2", "ON");
    }else{
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Devices/Device 3", "OFF");
    }

    if(val3.equals("ON")){
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Devices/Device 3", "ON");
    }else{
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Devices/Device 3", "OFF");
    }

    if(val4.equals("ON")){
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Devices/Device 4", "ON");
    }else{
      Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Devices/Device 4", "OFF");
    }
    
  }
}



void withoutInternet(){
  if(digitalRead(switchPin1)==0){
    val1="ON";
  }else{
    val1="OFF";
  }

  if(digitalRead(switchPin2)==0){
    val2="ON";
  }else{
    val2="OFF";
  }

  if(digitalRead(switchPin3)==0){
    val3="ON";
  }else{
    val3="OFF";
  }


  updateDeviceState(Relay1, val1, "Device 1");
  updateDeviceState(Relay2, val2, "Device 2");
  updateDeviceState(Relay3, val3, "Device 3");
}

void checkBtnStatus(){
  if (Firebase.ready()) {
    readFirebaseData("/Users/" + uid + "/Devices/Device 1", val1);
    readFirebaseData("/Users/" + uid + "/Devices/Device 2", val2);
    readFirebaseData("/Users/" + uid + "/Devices/Device 3", val3);
    readFirebaseData("/Users/" + uid + "/Devices/Device 4", val4);
  }

  updateDeviceState(Relay1, val1, "Device 1");
  updateDeviceState(Relay2, val2, "Device 2");
  updateDeviceState(Relay3, val3, "Device 3");
  updateDeviceState(Relay4, val4, "Device 4");
}

void readFirebaseData(const String& path, String& value) {
  if (Firebase.RTDB.getString(&fbdo, path)) {
    if (fbdo.dataTypeEnum() == firebase_rtdb_data_type_string) {
      value = fbdo.to<String>();
    } else {
      Serial.println(fbdo.errorReason());
    }
  }
}

void updateDeviceState(int relayPin, const String& value, const String& deviceName) {
  if (value.equals("ON")) {
    digitalWrite(relayPin, LOW);
    Serial.println(deviceName + " ON");
  } else if (value.equals("OFF")) {
    digitalWrite(relayPin, HIGH);
    Serial.println(deviceName + " OFF");
  }
}

void loop() {
  checkRecharge();
  Serial.print("Flag Value: ");
  Serial.println(wifiFlag);
  if(WiFi.status()==WL_CONNECTED){
    if(wifiFlag==1){
      Serial.println("Now Flag value set to 0 and manual button state updated to DB");
      wifiFlag=0;
      updateDB();
    }
  }else{
    if(wifiFlag==0)
      wifiFlag=1;
  }

  if(wifiFlag==0){
    Serial.println("WiFi Connected: With Internet Mode");
    // updateDB();
    checkBtnStatus();
  }else{
    Serial.println("WiFi not connected: Without Internet Mode");
    withoutInternet();
  }

  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();

  if(Firebase.ready()){
    Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Readings/Voltage", voltage);
    Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Readings/Current", current);
    Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Readings/Power", power);
    Firebase.RTDB.setString(&fbdo, "/Users/" + uid + "/Readings/Energy", energy);
  }

  Serial.println("Voltage: " + String(voltage) + " V");
  Serial.println("Current: " + String(current) + " A");
  Serial.println("Power: " + String(power) + " W");
  Serial.println("Energy: " + String(energy) + " Wh");

  

  updateTotalEnergy();

  // Erase only used addresses every 24 hours (adjust as needed)
  static unsigned long lastEraseTime = 0;
  if (millis() - lastEraseTime >= 86400000) {  // 24 hours in milliseconds
    // Here total energy is stored at end of EEPROM size
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(EEPROM_SIZE - sizeof(float), totalEnergy);  // Store at the end of EEPROM
    EEPROM.commit();
    EEPROM.end();

    // Here total balance is stored at TEMP_BALANCE_MARKER memory location
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(TEMP_BALANCE_MARKER, totalBalance);  // Store at the end of EEPROM
    EEPROM.commit();
    EEPROM.end();

    eraseUsedAddresses(); //Here three memory locations used for storing totalEnergy are erased

    // Here BALANCE_MARKER location where totalBalance is stored is erazed
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.write(BALANCE_MARKER, 0);
    EEPROM.commit();
    EEPROM.end();

    // Here totalEnergy is retrieved from last memory location of EEPROM and stored to variable
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(EEPROM_SIZE - sizeof(float), totalEnergy);
    EEPROM.end();
    
    // Here similarly totalBalanace retrieved from TEMP_BALANCE_MARKER location and stored to variable totalBalance
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(TEMP_BALANCE_MARKER, totalBalance);
    EEPROM.end();

    updateTotalEnergy(); //Here this method is caaled to again store total energy to that three memory locations

    // Now here last memory location where totalEnergy is temporarly stored are erased
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.write(EEPROM_SIZE - sizeof(float), 0);
    EEPROM.commit();
    EEPROM.end();

    // Here TEMP_BALANCE_MARKER location erased which is used to store totalBalance temporary
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.write(TEMP_BALANCE_MARKER, 0);
    EEPROM.commit();
    EEPROM.end();
    lastEraseTime = millis();
  }

  delay(UPDATE_INTERVAL);

}
