from RPLCD.i2c import CharLCD
import time

lcd = CharLCD('PCF8574', 0x27)  # Замените 0x27 на адрес вашего I2C, если он отличается

while True:
    current_time = time.strftime('%H:%M:%S')  # Получаем текущее время
    lcd.clear()  # Очищаем дисплей
    lcd.write_string(current_time)  # Выводим время на дисплей
    time.sleep(1)  # Ожидаем одну секунду перед следующим обновлением
