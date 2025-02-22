#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27 // Адрес твоего дисплея

// The following are predefined for the Arduino Uno
// 	SDA → A4
//	SCL → A5

LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4); // 20 символов, 4 строки

void setup() {
    lcd.init();           // Инициализация дисплея
    lcd.backlight();      // Включаем подсветку
    lcd.setCursor(0, 0);  // Перемещаем курсор в начало
    lcd.print("Hello, world!"); // Выводим текст
}

void loop() {}