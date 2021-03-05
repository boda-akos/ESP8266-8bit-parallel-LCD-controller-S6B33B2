# ESP8266-8bit-parallel-LCD-controller-S6B33B2
ESP8266 D1 mini has only a limited number of GPiO pins. To control a 8-bit wide LCD data bus the options are : PCF8574 i2c controller, or a simple shift register. 
To control the 74HC595 chip 3 pins are needed, for the LCD additional 3 + Reset (is it really necessary to pulse Reset?  seems to work well with Reset tied to 3.3V). 
The simple library links to Adafruit GFX graphics. The hardware uses fast direct port write. It is a lot faster than i2c.

The library has fixed hardware pins, some dedicated pins for SPI and i2c are still free to use. Reset D2 should be moved to some other pin if i2c is used. 
I disconnected Reset anyway from D2 and tied to 3.3V, seems to work well.
 
Pin configuration : 

Reconfigured TX and RX pins as general I/O pins.
74HC595 control : D8 data, D4 latch, D3 clock.
S6B33B2 control : D0 CS, RX GPIO3 RS, TX GPIO0 WR, D2 Reset.

The library should work with any 8-bit TFT if the LCD initial setup in the cpp file was modified for that particular TFT controller. 
