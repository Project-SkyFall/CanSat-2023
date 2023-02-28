/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-microsd-card-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include "SPI.h"

SPIClass spi = SPIClass(VSPI);

#define SCK  18
#define MISO  19
#define MOSI  23
#define CS  17

String lines[200];
int n = 0;
char dataread[200];
int l = 0;
int j = 0;
bool openable;





void setup() {
  Serial.begin(115200);

  spi.begin(SCK, MISO, MOSI, CS);

  initSDCard();

  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/data.txt");
  if (!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Test File --- Lets GO \r\n");
  }
  else {
    Serial.println("File already exists");
  }
  file.close();

}

void loop() {
  delay(500);
  readFile(SD, "/test.txt");


  //Serial.print(dataread);
  if (openable == true) {
    while (dataread[l] != (42)) {
      while (dataread[l] != (13)) {
        lines[j] += dataread[l];
        l++;
      }
      lines[j] += ("\r\n");
      j++;
      l++;
    }
    if (dataread[l] == (42)) {
      l = 0;
      j = 0;
    }
  }
  Serial.print(lines[0]);
  Serial.print(lines[1]);
  Serial.print(lines[2]);
  Serial.print(lines[3]);
  Serial.print(lines[4]);



  Serial.print("\r\n");




}
