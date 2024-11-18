// #include <SPI.h>
// #include <SD.h>

// #define SD_CS_PIN D0  // Set SD card chip select to D2 (NodeMCU)

// void setup() {
//   Serial.begin(115200);

//   // Initialize SD card
//   if (!SD.begin(SD_CS_PIN)) {
//     Serial.println("SD card initialization failed! Check connections.");
//     while (1);  // Halt execution if SD card fails
//   }
//   Serial.println("SD card initialized.");

//   // Create and open a file for writing
//   File dataFile = SD.open("test.txt", FILE_WRITE);
//   if (dataFile) {
//     dataFile.println("Hello, SD Card!");
//     dataFile.println("This is a test file.");
//     dataFile.close();
//     Serial.println("Data written to test.txt");
//   } else {
//     Serial.println("Error opening test.txt for writing.");
//   }
// }

// void loop() {
//   // Nothing to do in loop
// }

//--------------------------------------------------------------------------
#include <SPI.h>
#include <SD.h>

#define SD_CS_PIN 15  // Chip Select Pin connected to GPIO15 (SDCMD)

// Pin mappings for custom SPI pins
#define SD_CLK_PIN   14  // SCK (SDCLK) connected to GPIO14
#define SD_MISO_PIN  12  // MISO (SDD1) connected to GPIO12
#define SD_MOSI_PIN  13  // MOSI (SDD0) connected to GPIO13

void setup() {
  Serial.begin(115200);

  // Manually set SPI pins
  pinMode(SD_CLK_PIN, OUTPUT);  // SCK (SDCLK)
  pinMode(SD_MISO_PIN, INPUT);  // MISO (SDD1)
  pinMode(SD_MOSI_PIN, OUTPUT); // MOSI (SDD0)
  pinMode(SD_CS_PIN, OUTPUT);   // CS (SDCMD)

  // Initialize SPI
  SPI.begin();  // Use default SPI pins
  
  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed! Check connections.");
    while (1);  // Halt execution if SD card fails
  }
  Serial.println("SD card initialized.");

  // Create and open a file for writing
  File dataFile = SD.open("testone.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Hello, SD Card!");
    dataFile.println("This is a test file.");
    dataFile.close();
    Serial.println("Data written to test.txt");
  } else {
    Serial.println("Error opening test.txt for writing.");
  }
}

void loop() {
  // Nothing to do in loop
}
