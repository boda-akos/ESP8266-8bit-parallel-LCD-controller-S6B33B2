// Graphics library by ladyada/adafruit with init code from Rossum
// MIT license

#ifndef _ADAFRUIT_TFTLCD_8BIT_ESP_H_
#define _ADAFRUIT_TFTLCD_8BIT_ESP_H_

#include <Adafruit_GFX.h>

//============================================================
//Define pins and Output Data Registers
//ESP8266 mini D1 pin names reference e.g. D6=12
//uint8_t  D[]={16,5,4,0,2,14,12,13,15,1,3};  //call TX pin D9, RX pin D10

//========== HC595 pins ===================
#define ltch    2        //Data port 74HC595
#define clk     0
#define dta     15
//=========== LCD pins ====================
#define TFT_WR		1
#define TFT_RS		3
#define TFT_CS		16
#define TFT_RST   4 
//Parallel LCD Data 0-7 --> 74hc595 pins 15,1,2,3,4,5,6,7 
//=======================================
#define TFTWIDTH   128
#define TFTHEIGHT  160

// Initialization command tables for different LCD controllers
#define TFTLCD_DELAY 0xFF

// For compatibility with sketches written for older versions of library.
// Color function name was changed to 'color565' for parity with 2.2" LCD
// library.
#define Color565 color565

//=======================================
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

	#define WR_ACTIVE    digitalWrite(TFT_WR, LOW)
	#define WR_IDLE      digitalWrite(TFT_WR, HIGH)
	#define CD_COMMAND   digitalWrite(TFT_RS, LOW)
	#define CD_DATA      digitalWrite(TFT_RS, HIGH)
	#define CS_ACTIVE    digitalWrite(TFT_CS, LOW)
	#define CS_IDLE      digitalWrite(TFT_CS, HIGH)
	#define CS_ACTIVE_CD_COMMAND	{ CS_ACTIVE; CD_COMMAND; }

#define WR_STROBE { WR_ACTIVE; WR_IDLE; }

//Fast port write ESP8266

inline void write8(byte c){             //shift 8 bits in HC595 latch MSB first
  WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 8, 1<<ltch );  //latch low
  for (byte i=0; i<8 ; i++) {
     WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 8, 1<<clk );  //clock low
     if (c & 0x80) WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 4, 1<<dta ); //data 1
              else WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 8, 1<<dta ); //data 0
    WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 4, 1<<clk );  //clock high
  c = c*2;       
 }
  WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 4, 1<<ltch );  //latch high
  WR_STROBE;  //write to LCD
}

//=======================================

#define swap(a, b) { int16_t t = a; a = b; b = t; }

//=======================================
class Adafruit_TFTLCD_8bit_ESP : public Adafruit_GFX {

 public:

  //Adafruit_TFTLCD_8bit_ESP(uint8_t cs, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t rst);
  Adafruit_TFTLCD_8bit_ESP(void);

  void     begin(void);
  void     drawPixel(int16_t x, int16_t y, uint16_t color);
  void     drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
  void     drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
  void     fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
  void     fillScreen(uint16_t color);
  void     reset(void);
  void     setRegisters8(uint8_t *ptr, uint8_t n);
  void     setRegisters16(uint16_t *ptr, uint8_t n);
  void     setRotation(uint8_t x);
       // These methods are public in order for BMP examples to work:
  void     setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
  void     invertDisplay(boolean i),
			     pushColors(uint16_t *data, int16_t len, boolean first),
           drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t * bitmap);

  //uint16_t readPixel(int16_t x, int16_t y),
      //     readID(void);
//=======================================
// Pass 8-bit (each) R,G,B, get back 16-bit packed color
// color coding on bits:
// high byte sill be sent first
// bit nr: 		15	14	13	12	11	 10	09	08		07	06	05	 04	03	02	01	00
// color/bit:	R5	R4	R3	R2	R1 | G5	G4	G3		G2	G1	G0 | B5	B4	B3	B2	B1
// 								R0=R5											B0=B5
//=======================================
  uint16_t inline color565(uint8_t r, uint8_t g, uint8_t b) { return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); }

 private:

  void     init();
  void     flood(uint16_t color, uint32_t len);
  uint8_t  driver;
};

extern void writeCommand(uint16_t c);
extern void writeRegister8(uint16_t a, uint8_t d);
extern void writeRegister16(uint16_t a, uint16_t d);
extern void writeRegister24(uint16_t a, uint32_t d);
extern void writeRegister32(uint16_t a, uint32_t d);
extern void writeRegisterPair(uint16_t aH, uint16_t aL, uint16_t d);

extern Adafruit_TFTLCD_8bit_ESP tft;

#endif
