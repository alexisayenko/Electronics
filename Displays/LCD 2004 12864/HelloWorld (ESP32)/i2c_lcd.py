from lcd_api import LcdApi
from machine import I2C
from time import sleep

class I2cLcd(LcdApi):
    def __init__(self, i2c, addr, num_lines, num_columns):
        self.i2c = i2c
        self.addr = addr
        self.num_lines = num_lines
        self.num_columns = num_columns
        self.backlight = 0x08
        sleep(0.02)
        self._write_command(0x03)
        sleep(0.005)
        self._write_command(0x03)
        sleep(0.005)
        self._write_command(0x03)
        self._write_command(0x02)
        self._write_command(0x28)
        self._write_command(0x08)
        self._write_command(0x01)
        sleep(0.002)
        self._write_command(0x06)
        self._write_command(0x0C)

    def _write_command(self, cmd):
        self._write_byte(cmd, 0)

    def _write_data(self, data):
        self._write_byte(data, 1)

    def _write_byte(self, data, mode):
        byte = (data & 0xF0) | self.backlight | mode
        self.i2c.writeto(self.addr, byte.to_bytes(1, 'big'))
        self._pulse_enable(byte)
        byte = ((data << 4) & 0xF0) | self.backlight | mode
        self.i2c.writeto(self.addr, byte.to_bytes(1, 'big'))
        self._pulse_enable(byte)

    def _pulse_enable(self, byte):
        self.i2c.writeto(self.addr, (byte | 0x04).to_bytes(1, 'big'))
        sleep(0.0001)
        self.i2c.writeto(self.addr, (byte & ~0x04).to_bytes(1, 'big'))
        sleep(0.0001)

    def backlight_on(self):
        self.backlight = 0x08
        self._write_command(0)

    def backlight_off(self):
        self.backlight = 0x00
        self._write_command(0)