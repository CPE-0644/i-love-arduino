/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11 | D7
 ** MISO - pin 12 | D6
 ** CLK - pin 13 | D5
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN) | D8

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("ABC.TXT", FILE_WRITE);
  int i;
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to FUCK.txt...");
    myFile.println("kuy save");
    for (i = 0; i < 100; i++) {
      myFile.println(i);
    }
    // close the file:
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening FUCK.txt");
  }
  myFile.close();
}

void loop() {
  // nothing happens after setup
}

