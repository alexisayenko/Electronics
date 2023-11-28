import RPi.GPIO as GPIO
import time

TRANSMITTER_PIN = 17  # GPIO pin number of your transmitter

GPIO.setmode(GPIO.BCM)
GPIO.setup(TRANSMITTER_PIN, GPIO.OUT)

def transmit_code(code):
    '''Transmits a given code via the 433MHz transmitter'''
    GPIO.output(TRANSMITTER_PIN, 1)
    time.sleep(code)
    GPIO.output(TRANSMITTER_PIN, 0)

try:
    while True:
        # Example: Transmit a signal every 2 seconds
        transmit_code(0.001)  # The code to transmit (duration of the signal in seconds)
        time.sleep(2)
except KeyboardInterrupt:
    GPIO.cleanup()
