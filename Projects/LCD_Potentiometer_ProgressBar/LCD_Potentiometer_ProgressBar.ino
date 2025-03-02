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
const int barsNumberMax = 20;
const int lcdRows = 4;
const int lcdCols = 20;
const int progressBarStartCol = 0;
const int progressBarStartRow = 2;

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
    0b11111};

std::map<int, std::array<byte, 8>> barsFractionMap = {
    {1, barChar20},
    {2, barChar40},
    {3, barChar60},
    {4, barChar80},
    {5, barChar100}};

// 0x27 is a standard I2C address for LCDs
LiquidCrystal_I2C lcd(0x27, lcdCols, lcdRows);

void displayProgressBar()
{
  int value = analogRead(pinPotentiometer);
  int oneBarValue = maxAdcValue / barsNumberMax;

  int barsNumber = value / oneBarValue;

  lcd.setCursor(0, 0);
  lcd.print(value);

  lcd.setCursor(0, 1);
  lcd.print(barsNumber);

  // cleanning
  if (barsNumber < barsNumberMax)
    lcd.print(" ");

  lcd.setCursor(progressBarStartCol, progressBarStartRow);

  // print the progress bar of barChars (char from memory at index 0)
  for (int i = 0; i < barsNumber; i++)
    lcd.write(byte(0));

  // cleanning the rest of the line (3 chars - for bottom label)
  for (int i = barsNumber; i < 17; i++)
    lcd.print(" ");
}

void printTopLabel(int value1, int value2 = INT_MIN)
{
  lcd.setCursor(0, 0);
  lcd.print(value1);
  lcd.print(" ");
  if (value2 > INT_MIN)
    lcd.print(value2);

  // Cleanning old digits
  lcd.print("   ");
}

void printBottomLabel(int value)
{
  lcd.setCursor(0, 1);
  lcd.print(value);

  // Cleanning old digits
  if (value < 100)
    lcd.print(" ");
  if (value < 10)
    lcd.print("  ");
}

void printBars(int percentage)
{
  if (barsNumberMax != 10 && barsNumberMax != 20)
  {
    lcd.print("Error: barsNumberMax must be 10 or 20");
    return;
  }

  int barsNumber = percentage * barsNumberMax / 100;
  int fractionBarsIndex = (percentage * 5 / barsNumberMax) % 5;

  lcd.setCursor(progressBarStartCol, progressBarStartRow);

  // Print full bars
  for (int i = 0; i < barsNumber; i++)
    lcd.write(byte(5));

  // Print fractional bars
  if (fractionBarsIndex > 0)
    lcd.write(byte(fractionBarsIndex));

  // Cleanning rest of the line
  for (int i = barsNumber + (fractionBarsIndex > 0 ? 1 : 0); i < lcdCols - 1; i++)
    lcd.print(" ");
}

void displaySmoothProgressBar()
{
  int value = analogRead(pinPotentiometer);

  int percentage = value * 100 / maxAdcValue;
  int barsNumber = percentage * barsNumberMax / 100;

  printTopLabel(value, barsNumber);

  printBottomLabel(percentage);

  printBars(percentage);
}

void setup()
{
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

void loop()
{
  displaySmoothProgressBar();
  delay(10);
}