# sudo apt-get install rpi.gpio

import RPi.GPIO as GPIO
import time

TRANSMITTER_PIN = 17  # GPIO pin number of your transmitter

GPIO.setmode(GPIO.BCM)
GPIO.setup(TRANSMITTER_PIN, GPIO.OUT)

def transmit_code(code):
    '''Transmits a given code via the 433MHz transmitter'''
    for bit in code:
        GPIO.output(TRANSMITTER_PIN, int(bit))
        time.sleep(0.001)  # Duration of each bit; adjust as needed
    GPIO.output(TRANSMITTER_PIN, 0)
    time.sleep(0.001)  # Gap between characters

def encode_phrase(phrase):
    '''Encodes a phrase into a list of binary codes'''
    return [f'{ord(c):08b}' for c in phrase]

try:
    phrase = "Hello World"
    encoded_phrase = encode_phrase(phrase)
    while True:
        for code in encoded_phrase:
            transmit_code(code)
        time.sleep(2)  # Wait before repeating the phrase
except KeyboardInterrupt:
    GPIO.cleanup()
