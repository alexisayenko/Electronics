===Specification===
Operating voltage: 3.3V
Supply voltage: 2.2V-4.2V
Bluetooth: V2.0+EDR
Frequency: 2.4 - 2.48 GHz
Power consumption: upto 20 mA in sniff mode


===Schematics===
Connect "Bluetooth BTM-5" to USB porft of PC with "FTDI YP-05"
FTDI	BT
-----------
RX	TX
TX	RX
VCC 5V	VCC
GND	GND
//DTR	MODE // use onboard button or set MODE pin in high state

pin MODE: 1=AT cmd, 0=Normal

=== Serial communication ===

- Open RealTerm
- Connection on rate 38400. this cannot be changed by user
- Every AT command must be fol lowed with <CR> and<LF> so set EOL: +CR +LF or alternatevely add to each command \r\n


Red led flashes rapidly about 5 times per second. It does not accept AT commands.

To switch into AT mode you need to restart the module with button pressed
 - Remove the 5V connection to BT VCC
 – Press and hold the button switch on the BT module
 – Re-connect BT VCC to 5V (while still pressing the button switch), the LED should come on.
 – Release the button switch and the LED should be blinking slowly on/off once every couple of seconds. This indicates AT mode.

In windows add bluetooth device named HC-05. Default password: 1234. The mode is changed - red led blinks shortly once a second
