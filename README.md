# ESP8266-8bit-parallel-LCD-controller-S6B33B2
ESP8266 mini D1 has only a limited number of GPiO pins. To control a 8-bit wide LCD the options are : PCF8574 i2c controller, or a simple shift register. 
To control the 74HC595 chip we need 3 pins, and for the LCD additional 3 + Reset (not sure if reset is really needed). 
The simple library links to Adafruit GFX graphics.
The library has fixed hardware pins, however my basic idea was to save dedicated pins for SPI or i2c. Reconfigured TX and RX pins as general I/O.
