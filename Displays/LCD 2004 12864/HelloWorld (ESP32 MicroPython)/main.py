from machine import I2C, Pin
from i2c_lcd import I2cLcd
import time

# Initialize I2C with GPIO7 (SCL) and GPIO6 (SDA)
i2c = I2C(0, scl=Pin(7), sda=Pin(6), freq=400000)

# Scan for LCD address
devices = i2c.scan()
if not devices:
    print("No I2C devices found!")
else:
    lcd_address = devices[0]  # Assume first detected device is LCD
    lcd = I2cLcd(i2c, lcd_address, 2, 16)  # 16x2 LCD

    lcd.clear()
    lcd.putstr("Hello, MicroPython!")

    time.sleep(2)
    lcd.clear()
    lcd.putstr("LCD 1602A Works!")