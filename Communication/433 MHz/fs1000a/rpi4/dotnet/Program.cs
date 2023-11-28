using System;
using System.Device.Gpio;
using System.Threading;

class Program
{
    static void TransmitChar(GpioController controller, char character)
    {
        // Convert character to ASCII and then to binary
        string binary = Convert.ToString(character, 2).PadLeft(8, '0');

        foreach (char bit in binary)
        {
            if (bit == '1')
                controller.Write(TransmitterPin, PinValue.High);
            else
                controller.Write(TransmitterPin, PinValue.Low);

            Thread.Sleep(1); // Duration of each bit
        }

        // Short pause after each character
        controller.Write(TransmitterPin, PinValue.Low);
        Thread.Sleep(1);
    }

    const int TransmitterPin = 17; // Adjust to your GPIO pin

    static void Main()
    {
        using var controller = new GpioController();
        controller.OpenPin(TransmitterPin, PinMode.Output);

        Console.WriteLine("Press Ctrl+C to exit.");
        try
        {
            while (true)
            {
                //// Example: Toggle the transmitter
                //controller.Write(TransmitterPin, PinValue.High);
                //Thread.Sleep(1000); // On for 1 second

                //controller.Write(TransmitterPin, PinValue.Low);
                //Thread.Sleep(1000); // Off for 1 second
                string message = "Hello World";
       		foreach (char c in message)
        	{
		   TransmitChar(controller, c);
            	   Thread.Sleep(100); // Delay between characters
        	}
		Thread.Sleep(2000);
	    }
        }
        finally
        {
            controller.ClosePin(TransmitterPin);
        }
    }
}
