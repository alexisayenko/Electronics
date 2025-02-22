#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> 

hd44780_I2Cexp lcd; // Create an instance for I2C LCD

void setup() {
    Wire.begin(6, 7); // Set custom SDA and SCL pins (GPIO6, GPIO7)

    lcd.begin(20, 4); // Initialize LCD with 16x2 size
    lcd.print("Hello, World!"); // Print message on the first line

    lcd.setCursor(0, 1); // Move to the second line
    lcd.print("ESP32-S2 Ready!");
}

void loop() {
    // Nothing needed here, static message
}