#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>

/* 
 * Connect lcd to pins: SCL - 7, SDA - 6, Vcc, GND
 * Connect potentiometer: left pin - GND, right pin - Vcc, central pin - ESP32 pin 1 (ADC)
 */

const int pinPotentiometer = 1; 
const int pinSda = 6;
const int pinScl = 7;
const int maxAdcValue = 8191;
const int barsNumberMax = 10;
int oneBarValue = maxAdcValue / barsNumberMax;

// Define custom character █
byte barChar[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

// 0x27 is a standard I2C address for LCDs
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Wire.begin(pinSda, pinScl);
    lcd.init();
    lcd.backlight();

    // store bar char in memory at index  0
    lcd.createChar(0, barChar);
}

void loop() {

  int value = analogRead(pinPotentiometer);
  int barsNumber = value / oneBarValue;

   lcd.setCursor(0, 0);
   lcd.print(value);

   lcd.setCursor(0, 1);
   lcd.print(barsNumber);
   
   // cleanning
   if (barsNumber < 10)
    lcd.print(" "); 
  
   lcd.setCursor(3, 1);

  // print the progress bar of barChars (char from memory at index 0)
   for (int i = 0; i < barsNumber; i++)
     lcd.write(byte(0));
   
   // cleanning the rest of the line
   for (int i = barsNumber; i < 20; i++)
      lcd.print(" ");

   delay(10);
}