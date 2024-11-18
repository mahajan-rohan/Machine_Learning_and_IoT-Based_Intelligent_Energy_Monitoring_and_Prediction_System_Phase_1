#include <SPI.h>
#include <SD.h>

File myFile;

// void setup() 
// {
//   Serial.begin(9600);
//   SD.begin(10);
//   // open the file. note that only one file can be open at a time,
//   // so you have to close this one before opening another.
//   myFile = SD.open("test.txt", FILE_WRITE);
//   // write to it:
//   Serial.print("Writing to test.txt...");
//   myFile.println("Rohan testing a new adapter");
//   myFile.close();
//   Serial.println("done.");

//   // re-open the file for reading:
//   myFile = SD.open("test.txt");
//   Serial.println("test.txt:");
//   // read from the file until the line ends
//   String data = myFile.readString();
//   Serial.println(data);
//   // close the file:
//   myFile.close();
// }

// void loop() 
// {
//   // nothing happens after setup
// }

//------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);
  Serial.println("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  Serial.println("SD card initialized successfully!");

  File testFile = SD.open("test.txt", FILE_WRITE);
  if (testFile) {
    testFile.println("SD card write test successful!");
    testFile.close();
    Serial.println("Write test successful. Check test.txt on the SD card.");
  } else {
    Serial.println("Error: Unable to write to SD card!");
  }
}
void loop() {}

