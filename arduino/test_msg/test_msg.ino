// #include <SoftwareSerial.h>

// SoftwareSerial SIM900A(2, 3);

// void setup() {
//   SIM900A.begin(9600);  // GSM Module Baud rate - communication speed
//   Serial.begin(9600);   // Baud rate of Serial Monitor in the IDE app
//   Serial.println("Text Messege Module Ready & Verified");
//   delay(100);
//   Serial.println("Type s to send message or r to receive message");
// }


// void loop() {
//   if (Serial.available() > 0)
//     switch (Serial.read()) {
//       case 's':
//         SendMessage();
//         break;
//       case 'r':
//         RecieveMessage();
//         break;
//     }

//   if (SIM900A.available() > 0)
//     Serial.write(SIM900A.read());
// }


// void SendMessage() {
//   Serial.println("Sending Message please wait....");
//   SIM900A.println("AT+CMGF=1");  //Text Mode initialisation
//   delay(1000);
//   Serial.println("Set SMS Number");
//   SIM900A.println("AT+CMGS=\"+919028762236\"\r");  // Receiver's Mobile Number
//   delay(1000);
//   Serial.println("Set SMS Content");
//   SIM900A.println("Hie GSM here, Sim : Airtel");  // Messsage content
//   delay(100);
//   Serial.println("Done");
//   SIM900A.println((char)26);  //   delay(1000);
//   Serial.println("Message sent succesfully");
// }


// void RecieveMessage() {
//   Serial.println("Receiving Messeges");
//   delay(1000);
//   SIM900A.println("AT+CNMI=2,2,0,0,0");  // Eeceiving Mode Enabled
//   delay(1000);
//   Serial.write("Messege Received Sucessfully");
// }


//---------------------------------------------------------------------
// #include <SoftwareSerial.h>

// SoftwareSerial SIM900A(2, 3);

// // void setup(){
// //     SIM900A.begin(9600);  // GSM Module Baud rate - communication speed
// //     Serial.begin(9600);
// //     SIM900A.println("ATD+9028762236");
// //     delay(1000);
// // }

// // void loop(){

// // }

// void setup() {
//   SIM900A.begin(9600);  // GSM Module Baud rate - communication speed
//   Serial.begin(9600);   // Baud rate of Serial Monitor in the IDE app
//   Serial.println("Text Message Module Ready & Verified");
//   delay(100);
//   Serial.println("Type 's' to send message or 'r' to receive message");
// }

// void loop() {
//   if (Serial.available() > 0) {
//     switch (Serial.read()) {
//       case 's':
//         SendMessage();
//         break;
//       case 'r':
//         ReceiveMessage();
//         break;
//     }
//   }

//   if (SIM900A.available() > 0) {
//     Serial.write(SIM900A.read());  // Display the received data from SIM900A on Serial Monitor
//   }
// }

// void SendMessage() {
//   Serial.println("Sending Message, please wait...");
//   SIM900A.println("AT+CMGF=1");  // Text Mode initialization
//   delay(1000);
//   Serial.println("Set SMS Number");
//   SIM900A.println("AT+CMGS=\"+919028762236\"\r");  // Receiver's Mobile Number
//   delay(1000);
//   Serial.println("Set SMS Content");
//   SIM900A.println("Hi, GSM here, Sim: Airtel");  // Message content
//   delay(1000);
//   SIM900A.write(26);  // Send Ctrl+Z to indicate the end of the message
//   delay(1000);
//   Serial.println("Message sent successfully");
// }

// void ReceiveMessage() {
//   Serial.println("Receiving Messages, please wait...");
//   delay(1000);
//   SIM900A.println("AT+CMGF=1");  // Set the GSM module to text mode
//   delay(1000);
//   SIM900A.println("AT+CNMI=1,2,0,0,0");  // Set the module to display received messages immediately
//   delay(1000);
//   Serial.println("Ready to receive messages");
// }

//----------------------------------------------------------------------------------------------------------------------

//  #include <SoftwareSerial.h>

// SoftwareSerial mySerial(9, 10);

// void setup()
// {
//   mySerial.begin(9600);   // Setting the baud rate of GSM Module  
//   Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
//   delay(100);
// }


// void loop()
// {
//   if (Serial.available()>0)
//    switch(Serial.read())
//   {
//     case 's':
//       mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
//      delay(1000);  // Delay of 1 second
//      mySerial.println("AT+CMGS=\"+919028762236\"\r"); // Replace x with mobile number
//      delay(1000);
//      mySerial.println("Technolab creation");// The SMS text you want to send
//      delay(100);
//      mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
//       delay(1000);
//       break;



      
//     case 'r':
//       mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
//       delay(1000);
//       break;
//   }

//  if (mySerial.available()>0)
//    Serial.write(mySerial.read());
// }


// #include<SoftwareSerial.h>
// SoftwareSerial gsm(2,3);

// void setup()
// {
//   delay(10000);
//   Serial.begin(9600);
//   Serial.println("Sending Message, please wait...");
//   gsm.begin(9600);
//   gsm.println("AT+CMGF=1");
//   delay(1000);
//   gsm.println("AT+CMGS=\"+919028762236\"\r");
//   delay(1000);
//   gsm.println("hello");
//   delay(100);
//   gsm.println((char)26);
//   delay(1000);
//   Serial.println("end");
// }
// void loop()
// {
  
// }

//--------------------------------------------------------------------------------------------------------------------------
// #include <SoftwareSerial.h>    //Create software serial object to communicate with SIM800L
// SoftwareSerial GSM(2, 3); //SIM800L Tx & Rx is connected to Arduino #8 & #9

// char phone_no[] = "+919028762236"; //change with phone number to sms


// void setup() { 
//   Serial.begin(9600);//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
//   GSM.begin(9600);   //Begin serial communication with Arduino and SIM800L
//   delay(5000);
//   Serial.println("Initializing....");
//   initModule("AT", "OK", 1000);              //Once the handshake test is successful, it will back to OK
//   delay(2000);
//   Serial.println("Calling now...");
//   callUp(phone_no);
//   Serial.println("end...");
//   // GSM.println("AT+CMGF=1");
//   // delay(1000);
//   // GSM.println("AT+CMGS=\"+919028762236\"\r");
//   // delay(1000);
//   // GSM.println("hello");
//   // delay(100);
//   // GSM.println((char)26);
//   // delay(1000);
//   // Serial.println("end");

// }

// void loop() {

// }


// void callUp(char *number) {
//   GSM.print("ATD + "); GSM.print(number); GSM.println(";"); //Call to the specific number, ends with semi-colon,replace X with mobile number
//   delay(20000);       // wait for 20 seconds...
//   GSM.println("ATH"); //hang up
//   delay(100);
// }


// void initModule(String cmd, char *res, int t) {
//   while (1) {
//     Serial.println(cmd);
//     GSM.println(cmd);
//     delay(100);
//     while (GSM.available() > 0) {
//       if (GSM.find(res)) {
//         Serial.println(res);
//         delay(t);
//         return;
//       } else {
//         Serial.println("Error");
//       }
//     }
//     delay(t);
//   }
// }

//---------------------------------------------------------------------------------------------

#include <SoftwareSerial.h>    //Create software serial object to communicate with SIM800L
SoftwareSerial GSM(2, 3); //SIM800L Tx & Rx is connected to Arduino #8 & #9

char phone_no[] = "+919028762236"; //change with phone number to sms

void setup() { 

  Serial.begin(9600);//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  GSM.begin(9600);   //Begin serial communication with Arduino and SIM800L


  Serial.println("Initializing....");
  initModule("AT", "OK", 1000);              //Once the handshake test is successful, it will back to OK
callUp(phone_no);
delay(5000);
SendMessage();
}

void loop() {

}

void SendMessage() {
  Serial.println("Sending Message please wait....");
  GSM.println("AT+CMGF=1");  //Text Mode initialisation
  delay(1000);
  Serial.println("Set SMS Number");
  GSM.println("AT+CMGS=\"+919028762236\"\r");  // Receiver's Mobile Number
  delay(1000);
  Serial.println("Set SMS Content");
  GSM.println("Hie GSM here, Sim : Airtel");  // Messsage content
  delay(100);
  Serial.println("Done");
  GSM.println((char)26);  //   delay(1000);
  Serial.println("Message sent succesfully");
}

void callUp(char *number) {
  GSM.print("ATD + "); GSM.print(number); GSM.println(";"); //Call to the specific number, ends with semi-colon,replace X with mobile number
  delay(20000);       // wait for 20 seconds...
  GSM.println("ATH"); //hang up
  delay(100);
}


void initModule(String cmd, char *res, int t) {
  while (1) {
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while (GSM.available() > 0) {
      if (GSM.find(res)) {
        Serial.println(res);
        delay(t);
        return;
      } else {
        Serial.println("Error");
      }
    }
    delay(t);
  }
}




