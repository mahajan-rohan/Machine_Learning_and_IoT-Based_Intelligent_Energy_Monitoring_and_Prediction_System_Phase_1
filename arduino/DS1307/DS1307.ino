// // #include <Bonezegei_DS1307.h> //Library used

// // CLK -> 4 , DAT -> 3, Reset -> 2

// // Bonezegei_DS1307 rtc(0x68);

// #include <Wire.h>
// #include <RTClib.h>

// RTC_DS1307 rtc;


// void setup() {
//     Serial.begin(9600);
//     if (!rtc.begin()) {
//         Serial.println("Couldn't find RTC");
//         while (1);
//     }
//     rtc.adjust(DateTime(__DATE__, __TIME__));
// }

// // void setup() {
// //  Serial.begin(9600);
// //   rtc.begin();
// //   rtc.setFormat(12);        //Set 12 Hours Format
// //   rtc.setAMPM(1);           //Set AM or PM    1 = AM  0 = PM
// //   rtc.setTime("10:33:43");  //Set Time    Hour:Minute:Seconds
// //   rtc.setDate("09/10/24");   //Set Date    Month/Date/Year
// // }

// void setup() {
//     Serial.begin(9600);
//     if (!rtc.begin()) {
//         Serial.println("Couldn't find RTC");
//         while (1);
//     }
//     Serial.println("Setting time and date...");
//     rtc.setFormat(12);        // Set 12 Hours Format
//     rtc.setAMPM(1);           // Set AM or PM    1 = AM  0 = PM
//     rtc.setTime("10:33:43");  // Set Time    Hour:Minute:Seconds
//     rtc.setDate("09/10/24");  // Set Date    Month/Date/Year
// }


// void loop() {

//   if (rtc.getTime()) {
//       Serial.print("Date: ");
//       Serial.print(twoDigit(rtc.getDate()));
//       Serial.print("/");
//       Serial.print(twoDigit(rtc.getMonth()));
//       Serial.print("/");
//       Serial.print("20" + String(rtc.getYear()));

//       Serial.print("    ::    Time: ");
//       Serial.print(twoDigit(rtc.getHour()));
//       Serial.print(":");
//       Serial.print(twoDigit(rtc.getMinute()));
//       Serial.print(":");
//       Serial.print(twoDigit(rtc.getSeconds()));


//     if (rtc.getFormat() == 12) {  // returns 12 or 24 hour format

//       if (rtc.getAMPM()) {  //return 1 = AM  0 = PM
//         Serial.println("PM");
//       } else {
//         Serial.println("AM");
//       }
//     }
//   }

//   delay(1000);
// }

// String twoDigit(int number) {
//   if (number < 10) {  // If it's a single-digit number
//     return "0" + String(number);  // Add a leading zero
//   } else {
//     return String(number);  // Otherwise, just return the number as is
//   }
// }

//////////////////////////////////--------------------------------------------------------------------

// #include <Bonezegei_DS1307.h>
// Bonezegei_DS1307 rtc(0x68);

// void setup() {
//   Serial.begin(9600);
//   rtc.begin();
//   rtc.setFormat(12);        // Set 12 Hours Format
//   rtc.setAMPM(1);           // Set AM or PM 0 = AM 1 = PM
//   rtc.setTime("01:53:53");  // Set Time Hour:Minute:Seconds
//   rtc.setDate("09/10/24");   // Set Date Month/Date/Year
// }

// void loop() {
//   if (rtc.getTime()) {
//     Serial.print("Time ");
//     Serial.print(twoDigit(rtc.getHour()));
//     Serial.print(":");
//     Serial.print(twoDigit(rtc.getMinute()));
//     Serial.print(":");
//     Serial.print(twoDigit(rtc.getSeconds()));
//     Serial.print(" ");

//     if (rtc.getFormat() == 12) {  // returns 12 or 24 hour format
//       if (rtc.getAMPM()) {        // return 0 = AM 1 = PM
//         Serial.print("PM ");
//       } else {
//         Serial.print("AM ");
//       }
//     }

//     Serial.print("Date ");
//     Serial.print(twoDigit(rtc.getMonth()));
//     Serial.print("-");
//     Serial.print(twoDigit(rtc.getDate()));
//     Serial.print("-");
//     Serial.print(rtc.getYear());
//     Serial.println();
//   }
//   delay(1000);
// }

// String twoDigit(int number) {
//   if (number < 10) {  // If it's a single-digit number
//     return "0" + String(number);  // Add a leading zero
//   } else {
//     return String(number);  // Otherwise, just return the number as is
//   }
// }

//---------------------------------------------------------------------------------------
////////////////////////////// Working but 24 hr format ////////////////////////
// #include <Wire.h>
// #include "RTClib.h"
// RTC_DS1307 rtc;

// void setup () {
//   Serial.begin(9600);

//   if (! rtc.begin()) {
//     Serial.println("Couldn't find RTC");
//     Serial.flush();
//     abort();
//   }
//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
// }

// void loop () {
//     DateTime now = rtc.now();

//     Serial.print(now.year(), DEC);
//     Serial.print('/');
//     Serial.print(now.month(), DEC);
//     Serial.print('/');
//     Serial.print(now.day(), DEC);
//     Serial.println();
//     Serial.print(now.hour(), DEC);
//     Serial.print(':');
//     Serial.print(now.minute(), DEC);
//     Serial.print(':');
//     Serial.print(now.second(), DEC);
//     Serial.println();
//     Serial.println();
//     delay(1000);
// }

//---------------------------------------------------------------------------------

// #include <Wire.h>
// #include "RTClib.h"

// RTC_DS1307 rtc;

// void setup () {
//   Serial.begin(9600);

//   if (! rtc.begin()) {
//     Serial.println("Couldn't find RTC");
//     Serial.flush();
//     abort();
//   }

//   // Uncomment the following lines to set the RTC to a known time for testing
//   // Serial.println("RTC is NOT running, let's set the time!");
//   // rtc.adjust(DateTime(2024, 9, 10, 3, 18, 20)); // Set to 2:20 PM for testing
// }

// void loop () {
//     DateTime now = rtc.now();

//     Serial.print("Current Date and Time: ");
//     Serial.print(now.day(), DEC);
//     Serial.print('/');
//     Serial.print(now.month(), DEC);
//     Serial.print('/');
//     Serial.print(now.year(), DEC);
//     Serial.print(' ');

//     // Convert to 12-hour format
//     int hour = now.hour();
//     String period = "PM";
//     if (hour == 0) {
//         hour = 12; // Midnight case
//     } else if (hour == 12) {
//         period = "PM"; // Noon case
//     } else if (hour > 12) {
//         hour -= 12;
//         period = "PM";
//     }

//     Serial.print(hour, DEC);
//     Serial.print(':');
//     Serial.print(now.minute(), DEC);
//     Serial.print(':');
//     Serial.print(now.second(), DEC);
//     Serial.print(' ');
//     Serial.print(period);
//     Serial.println();
//     Serial.println();
//     delay(1000);
// }

//----------------------------------------------------------------------------------------------

// final sender arduino not synced

// #include <Wire.h>
// #include "RTClib.h"

// RTC_DS1307 rtc;

// void setup() {
//   Serial.begin(115200);

//   if (!rtc.begin()) {
//     Serial.println("Couldn't find RTC");
//     Serial.flush();
//     abort();
//   }

//   // Uncomment the following line to set the RTC to the current date & time
//   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
// }

// void loop() {
//   DateTime now = rtc.now();

//   // Format: YYYY-MM-DD HH:MM:SS
//   char dateTimeString[20]; // Buffer to hold the formatted string
//   snprintf(dateTimeString, sizeof(dateTimeString), "%04d-%02d-%02d %02d:%02d:%02d",
//            now.year(), now.month(), now.day(),
//            now.hour(), now.minute(), now.second());

//   // Print the formatted date-time string
//   Serial.println(dateTimeString);

//   delay(1000);
// }

//---------------------------------------------------------------------------------

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup() {
  Serial.begin(115200);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  // Uncomment the following line to set the RTC to the current date & time
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();

  // Format: YYYY-MM-DD HH:MM:SS
  char dateTimeString[20];
  snprintf(dateTimeString, sizeof(dateTimeString), "%04d-%02d-%02d %02d:%02d:%02d",
           now.year(), now.month(), now.day(),
           now.hour(), now.minute(), now.second());

  // Send data with delimiters
  Serial.print('<');          // Start delimiter
  Serial.print(dateTimeString);
  Serial.println('>');        // End delimiter

  delay(1000); // Send data every second
}


