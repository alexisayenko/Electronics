package main

import (
    "fmt"
    "log"
    "time"

    "periph.io/x/conn/v3/gpio"
    "periph.io/x/conn/v3/gpio/gpioreg"
    "periph.io/x/host/v3"
)

const transmitterPinName = "GPIO17" // Change as per your GPIO setup

func transmitCode(pin gpio.PinIO, code string) {
    for _, bit := range code {
        if bit == '1' {
            pin.Out(gpio.High)
        } else {
            pin.Out(gpio.Low)
        }
        time.Sleep(1 * time.Millisecond) // Duration of each bit; adjust as needed
    }
    pin.Out(gpio.Low)
    time.Sleep(1 * time.Millisecond) // Gap between characters
}

func encodePhrase(phrase string) []string {
    var encoded []string
    for _, c := range phrase {
        encoded = append(encoded, fmt.Sprintf("%08b", c))
    }
    return encoded
}

func main() {
    // Initialize periph.io library
    if _, err := host.Init(); err != nil {
        log.Fatal(err)
    }

    pin := gpioreg.ByName(transmitterPinName)
    if pin == nil {
        log.Fatalf("Failed to find %s", transmitterPinName)
    }

    phrase := "Hello World"
    encodedPhrase := encodePhrase(phrase)

    for {
        for _, code := range encodedPhrase {
            transmitCode(pin, code)
        }
        time.Sleep(2 * time.Second) // Wait before repeating the phrase
    }
}
