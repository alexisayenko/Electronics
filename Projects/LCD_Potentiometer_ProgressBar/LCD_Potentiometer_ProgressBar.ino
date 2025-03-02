#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Arduino.h>
#include <map>

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


std::array<byte, 8> barChar20 = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000};

  std::array<byte, 8> barChar40 = {
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000,
  0b11000};

  std::array<byte, 8> barChar60 = {
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100,
  0b11100};
  
  std::array<byte, 8> barChar80 = {
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110,
  0b11110};

// Define custom character █
std::array<byte, 8> barChar100 = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};


std::map<int, std::array<byte, 8>> barsFractionMap = {
  {1, barChar20},
  {2, barChar40},
  {3, barChar60},
  {4, barChar80},
  {5, barChar100}
};

// 0x27 is a standard I2C address for LCDs
LiquidCrystal_I2C lcd(0x27, 16, 2);

void displayProgressBar(){
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
}

void displaySmoothProgressBar(){
  int value = analogRead(pinPotentiometer);
  int barsNumber = value / oneBarValue;
 
  int percentage  = value * 100 / maxAdcValue;

  if (percentage == 100)
    barsNumber++;;

   lcd.setCursor(0, 0);
   lcd.print(value);
   lcd.print("   "); 

   lcd.setCursor(0, 1);
   lcd.print(percentage);
   
   // Cleanning old bars
  if (percentage < 100)
    lcd.print(" "); 
  if (percentage < 10)
    lcd.print(" ");

   lcd.setCursor(3, 1); 

  // Print the progress bar of barChars (char from memory at index 0)
  if (barsNumber > 0)
    for (int i = 0; i < barsNumber - 1; i++)
      lcd.write(byte(5));

  // Calculate the amount of fraction bars
  int a = percentage % 10;
  int b = a % 2 == 0 ? a : a + 1;
  int fractionBarsIndex = b / 2;

  if (b > 0)
    lcd.write(byte(fractionBars));
   
   // Cleanning the rest of the line
   for (int i = barsNumber; i < 20; i++)
      lcd.print(" ");
}

void setup() {
    Wire.begin(pinSda, pinScl);
    lcd.init();
    lcd.backlight();

    // Store bar char in memory at index  0
    lcd.createChar(1, barChar20.data());
    lcd.createChar(2, barChar40.data());
    lcd.createChar(3, barChar60.data());
    lcd.createChar(4, barChar80.data());
    lcd.createChar(5, barChar100.data());
}

void loop() {

   displaySmoothProgressBar();
   delay(10);
}