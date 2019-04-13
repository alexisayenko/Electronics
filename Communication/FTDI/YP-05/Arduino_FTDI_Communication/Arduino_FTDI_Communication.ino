#include <SoftwareSerial.h>

const byte rxPin = 2; // Wire this to Tx Pin of ESP8266
const byte txPin = 3; // Wire this to Rx Pin of ESP8266

SoftwareSerial ftdi (rxPin, txPin);

void setup() {

  // SoftwareSerial does not work correctly with 115200. Hence, use 9600.
  ftdi.begin(9600);
  Serial.begin(9600);

  ftdi.println("Commnunication Arduino with FTDI (software serial)");
  Serial.println("Communication Arduino with PC (hardware serial)");
}

void loop() {
  
  if (Serial.available() > 0) 
    while(Serial.available() > 0)
    {
      String message = Serial.readStringUntil('\n');
      ftdi.println(message);
    }

  if (ftdi.available() > 0)
    while(ftdi.available() > 0)
    {
      String message = ftdi.readStringUntil('\n');
      Serial.println(message);
    }
}
