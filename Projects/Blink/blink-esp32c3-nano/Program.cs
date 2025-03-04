using System;
using System.Threading;
using nanoFramework.Hardware.Esp32;

class Program
{
    static void Main()
    {
        int ledPin = 10;  // Adjust based on your board
        GpioPin led = new GpioController().OpenPin(ledPin, PinMode.Output);

        while (true)
        {
            led.Write(PinValue.High);
            Thread.Sleep(500);
            led.Write(PinValue.Low);
            Thread.Sleep(500);
        }
    }
}