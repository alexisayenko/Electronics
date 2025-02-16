#include <Wire.h>
#include <hd44780.h>  
#include <hd44780ioClass/hd44780_I2Cexp.h>  // I2C interface

hd44780_I2Cexp lcd; // Automatically detects I2C address

void setup() {
    Wire.begin(6, 7);  // Use GPIO6 for SDA, GPIO7 for SCL (Lolin S2 Mini Pro)

    int status = lcd.begin(16, 2);  // Initialize 16x2 LCD
    if (status) { 
        Serial.print("LCD initialization failed with status: ");
        Serial.println(status);
        while (1);  // Stop execution if LCD fails
    }

    pinMode(2, INPUT_PULLDOWN);
    pinMode(3, INPUT_PULLDOWN);

    printMenu();
}

void loop() {

    if (moveSelector())
      printMenu();

    delay(200);
}

const char* menuItems[] = {
    "Start",
    "Settings",
    "Info",
    "Exit",
    "MenuItem 5"
};

const int menuItemsLength = sizeof(menuItems) / sizeof(menuItems[0]);

const int screenLinesCount = 2;
const int screenColumnsCount = 16;
const char selectorChar = (char)126;
int selectorPosition = 0;

bool moveSelector(){
   bool isDirty = false;

   if (digitalRead(2) == HIGH){
      selectorPosition--;
      isDirty = true;
    }

   if (digitalRead(3) == HIGH){
      selectorPosition++;
      isDirty = true;
    }

    if (isDirty == false)
      return false;

    if (selectorPosition < 0)
      selectorPosition = 0;
    if (selectorPosition == menuItemsLength)
      selectorPosition = menuItemsLength;

    return true;
}

void printMenu(){
  lcd.clear();

  int col = 0;
  int row = 0;
  uint maxLen = 0;
  
  for (int j = 0; j < menuItemsLength; j++) {

    maxLen = max(maxLen, strlen(menuItems[j]));

    if (col + maxLen >= screenColumnsCount - 1)
        return; 

    lcd.setCursor(col, row);
    if (selectorPosition == j)
      lcd.print(selectorChar);
    else
      lcd.print(" ");

    lcd.print(menuItems[j]);

    row++;

    if (row == screenLinesCount){
      col = maxLen + 1; // space/selector before the menu item
      row = 0;
      maxLen = 0;
    }
  }
}

void rotateText(char text[]){

    text[strlen(text) - 1] = text[0];

    for (int i = 0; i < strlen(text) - 1; i++)
    {
      text[i] = text [i+1];
    }
}