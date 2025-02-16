from RPLCD.i2c import CharLCD

lcd = CharLCD('PCF8574', 0x27)  # Замените 0x27 на адрес вашего I2C, если он отличается
lcd.write_string('Hello World!')
lcd.crlf()
lcd.write_string('Hello World!')
