# Download LCD library
# wget https://github.com/dbrgn/RPLCD/archive/refs/heads/master.zip

# Unzip it and install
# unzip master.zip
# python RPLCD-master/setup.py install
# rm -rf master.zip
# rm -rf RPLCD-master

# Turn on I2C interface
# sudo raspi-config

# connect display
# SDA and SCL portsна on Raspberry Pi

from RPLCD.i2c import CharLCD

lcd = CharLCD('PCF8574', 0x27)  # Замените 0x27 на адрес вашего I2C, если он отличается
lcd.write_string('Hello World!')
lcd.crlf()
lcd.write_string('Hello World!')
