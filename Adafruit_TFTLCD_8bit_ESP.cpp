// Graphics library by ladyada/adafruit with init code from Rossum
// MIT license

#include "Adafruit_TFTLCD_8bit_ESP.h"
//#include "s6b.h"

//======================================
// Constructor

Adafruit_TFTLCD_8bit_ESP :: Adafruit_TFTLCD_8bit_ESP(void)
: Adafruit_GFX(TFTWIDTH, TFTHEIGHT)
{
}
  
 void wrBus(byte c){  //send 8 bit data to LCD parallel port, move WR signal with delay
 /* digitalWrite(ltch,0);                                           //WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 8, 1<<ltch );  //latch
  for (byte i=0; i<8 ; i++) {
     digitalWrite(clk,0);                                           // WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 8, 1<<clk );  //clock low
     if (c & 0x80) digitalWrite(dta,1); else digitalWrite(dta,0);   //if (c & 0x80) WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 4, 1<<dta ); //data
                                                                    //else WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 8, 1<<dta );
    digitalWrite(clk,1);                                            //WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 4, 1<<clk );  //clock high
   c= c<<1;       
 }  //8 bits shifted in
   digitalWrite(ltch,1);                                            //WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 4, 1<<ltch );  //latch high
   */
   //74HC595 serial to 8 bit convert
   WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 8, 1<<ltch );  //latch
  for (byte i=0; i<8 ; i++) {
    WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 8, 1<<clk );  //clock low
     if (c & 0x80) WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 4, 1<<dta ); //data
              else WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 8, 1<<dta );
    WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 4, 1<<clk );  //clock high
    c = c<<1;       
 }  //8 bits shifted in, parallel output set
  WRITE_PERI_REG(PERIPHS_GPIO_BASEADDR + 4, 1<<ltch );  //latch high
  digitalWrite(TFT_WR,LOW);  // write to LCD
  delayMicroseconds(10);
  digitalWrite(TFT_WR,HIGH);  
}

void wrBus2(int d1, int d2) {
wrBus(d1);
wrBus(d2);
  }
  
void wrBus3(int d1, int d2, int d3) {
wrBus(d1);
wrBus(d2);
wrBus(d3);
  }
//=======================================
void Adafruit_TFTLCD_8bit_ESP::begin()
{
	 reset();
  digitalWrite(TFT_CS,0); //CS
  digitalWrite(TFT_RS,0); //RS
  digitalWrite(TFT_WR,1); //WR  
  
  wrBus(0x2c); delay(10); // LCDWriteCommand( 0x2C);delay_1ms(20);------set OTP mode off---------
  wrBus(0xea); delay(10);
  wrBus2(0x02,0x01);delay(10);//------set internal osc on---------LCDWriteCommand( 0x02);LCDWriteCommand( 0x01); 
  wrBus2(0x26,0x01);delay(20);//------booster1 on---------------LCDWriteCommand( 0x26);LCDWriteCommand( 0x01);
  wrBus2(0x26,0x09);delay(20);//------booster1 on and amp on---------LCDWriteCommand( 0x26);LCDWriteCommand( 0x09);
  wrBus2(0x26,0x0B);delay(20);//------booster2 on-------------LCDWriteCommand( 0x26); LCDWriteCommand( 0x0b);
  wrBus2(0x26,0x0F);delay(20);//------booster3 on-------------LCDWriteCommand( 0x26);LCDWriteCommand( 0x0f);
   //rotate 0x10 ,  adjust 0x42 together +3 
   //Register 0x10 DB0 also matters - see s6b33b3 pdf . For rotate, 0x40 bit 0x02 is in effect as well
  wrBus2(0x10,0x22);delay(10);//------LCDWriteCommand( 0x10);LCDWriteCommand( 0x21);//FL side 0x22 L,0x25 R
  wrBus2(0x20,0x01); delay(10);//------booster1 on---------------LCDWriteCommand( 0x20);LCDWriteCommand( 0x0A);  
  wrBus2(0x22,0x11); delay(10); //------bias set to 1/5 --------LCDWriteCommand( 0x22);LCDWriteCommand(  0x11);
  wrBus2(0x24,0x11);delay(10);//------set clock fpck=fose/32(Normal)/fpck=fose/16(partial1)-------LCDWriteCommand( 0x24);LCDWriteCommand( 0x11);
  wrBus2(0x28,0x01);delay(10);//------temp comp ratio -0.05%------ LCDWriteCommand( 0x28);LCDWriteCommand( 0x01);
  wrBus2(0x2A,0xd0);delay(10);//------contrast1 set v1 to 3.757v  max=4v----0x2A,CTRL1 LCDWriteCommand( 0x2a);LCDWriteCommand( 0xBB);   //partial display mode 0
  //wrBus2(0x2B,0x10);delay(10);//------contrast2 set v1 to 3.757v  max=4v--------LCDWriteCommand( 0x2b);LCDWriteCommand( 0x20);   //partial display mode 1
  wrBus2(0x30,0x02);delay(10);//------GSM=00:65K color,DSG=0,SGF=0,SGP=01,SGM=0----- LCDWriteCommand( 0x30);LCDWriteCommand( 0x02);//09
  wrBus2(0x32,0x0e);delay(10);//------row vector type=Diagonal ,INC=111-----LCDWriteCommand( 0x32);LCDWriteCommand( 0x0E);//0e
  wrBus2(0x34,0x89);delay(10);//------frame set FIM=ON,FIP=1FRAME,N-BLOCK=9----- LCDWriteCommand( 0x34);LCDWriteCommand( 0x89);//cd
  wrBus2(0x36,0x00);delay(10);//------Low frequency set off-------LCDWriteCommand( 0x36);LCDWriteCommand( 0x00);
  wrBus2(0x45,0x00);delay(10);//------ram skip area set no skip------LCDWriteCommand( 0x45);LCDWriteCommand(  0x00);
  wrBus2(0x40,0x00);delay(10);//------entry mode set : x addr incr, read modify write off--LCDWriteCommand( 0x40);LCDWriteCommand( 0x00);
  wrBus3(0x42,0x02,161);delay(10);//------x address set from 00 to 159--------LCDWriteCommand( 0x42);LCDWriteCommand( 0x00);LCDWriteCommand( 0x9F);  
  wrBus3(0x43,0x00,127);delay(10);//------y address set from 00 to 127--------LCDWriteCommand( 0x43);LCDWriteCommand( 0x00);LCDWriteCommand( 0x7F);  
  wrBus2(0x55,0x00);delay(10);//------partial display mode off-------LCDWriteCommand( 0x55);LCDWriteCommand( 0x00);
  wrBus2(0x53,0x00);delay(10);//------normal display---------LCDWriteCommand( 0x53);LCDWriteCommand( 0x00);
  wrBus2(0x5A,0x00);delay(10);//-------Scroll start line Set------- LCDWriteCommand(0x5A);LCDWriteCommand( 0x00);
  wrBus(0x51);delay(300); //------display on set--------delay_1ms(20);LCDWriteCommand( 0x51);
  digitalWrite(TFT_CS,1); //CS
}

//=======================================
void Adafruit_TFTLCD_8bit_ESP::reset(void)
{
  pinMode(TFT_WR, FUNCTION_3); //TX RX as normal GPIO
  pinMode(TFT_RS, FUNCTION_3);
  pinMode(dta,OUTPUT); pinMode(clk, OUTPUT);pinMode(ltch, OUTPUT);

	pinMode(TFT_WR, OUTPUT);
	pinMode(TFT_RS, OUTPUT);
	pinMode(TFT_CS, OUTPUT);
	CS_IDLE; // Set all control bits to HIGH (idle)
	CD_DATA; // Signals are ACTIVE LOW
	WR_IDLE;
	write8(0); //clear shift register
  	pinMode(TFT_RST, OUTPUT);
		digitalWrite(TFT_RST, HIGH);
		delay(10);
		digitalWrite(TFT_RST, LOW);
		delay(10);
		digitalWrite(TFT_RST, HIGH);
		delay(10);
}

//=======================================
// Sets the LCD address window.
// Relevant to rect/screen fills and H/V lines.  Input coordinates are
// assumed pre-sorted (e.g. x2 >= x1).
//=======================================
void Adafruit_TFTLCD_8bit_ESP::setAddrWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{ 
  CS_ACTIVE_CD_COMMAND;
  write8(0x42);write8(x1);write8(x2);
  write8(0x43);write8(y1);write8(y2);
  CS_IDLE;
}

//=======================================
// Fast block fill operation for fillScreen, fillRect, H/V line, etc.
// Requires setAddrWindow() has previously been called to set the fill
// bounds.  'len' is inclusive, MUST be >= 1.
//=======================================
void Adafruit_TFTLCD_8bit_ESP::flood(uint16_t color, uint32_t len)
{
  int i;
  CS_ACTIVE;
  CD_DATA;
  for ( i=0; i<len; i++) { 
    write8(color >> 8);
    write8(color);
    } //0x00ff masking done in write8
  CS_IDLE;
}

//=======================================
void Adafruit_TFTLCD_8bit_ESP::drawFastHLine(int16_t x, int16_t y, int16_t length, uint16_t color)
{
  int16_t x2;

  // Initial off-screen clipping
  if((length <= 0     ) ||
     (y      <  0     ) || ( y                  >= _height) ||
     (x      >= _width) || ((x2 = (x+length-1)) <  0      )) return;

  if(x < 0) {        // Clip left
    length += x;
    x       = 0;
  }
  if(x2 >= _width) { // Clip right
    x2      = _width - 1;
    length  = x2 - x + 1;
  }

  setAddrWindow(x, y, x2, y);
  flood(color, length);
   setAddrWindow(0, 0, _width - 1, _height - 1);
  
}

//=======================================
void Adafruit_TFTLCD_8bit_ESP::drawFastVLine(int16_t x, int16_t y, int16_t length, uint16_t color)
{
  int16_t y2;

  // Initial off-screen clipping
  if((length <= 0      ) ||
     (x      <  0      ) || ( x                  >= _width) ||
     (y      >= _height) || ((y2 = (y+length-1)) <  0     )) return;
  if(y < 0) {         // Clip top
    length += y;
    y       = 0;
  }
  if(y2 >= _height) { // Clip bottom
    y2      = _height - 1;
    length  = y2 - y + 1;
  }

  setAddrWindow(x, y, x, y2);
  flood( color, length);
 setAddrWindow(0, 0, _width - 1, _height - 1);
  
}

void Adafruit_TFTLCD_8bit_ESP::fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, uint16_t fillcolor)
{
	//Serial.println("\n::fillRect...");
  int16_t  x2, y2;

  // Initial off-screen clipping
  if( (w            <= 0     ) ||  (h             <= 0      ) ||
      (x1           >= _width) ||  (y1            >= _height) ||
     ((x2 = x1+w-1) <  0     ) || ((y2  = y1+h-1) <  0      )) return;
  if(x1 < 0) { // Clip left
    w += x1;
    x1 = 0;
  }
  if(y1 < 0) { // Clip top
    h += y1;
    y1 = 0;
  }
  if(x2 >= _width) { // Clip right
    x2 = _width - 1;
    w  = x2 - x1 + 1;
  }
  if(y2 >= _height) { // Clip bottom
    y2 = _height - 1;
    h  = y2 - y1 + 1;
  }

  setAddrWindow(x1, y1, x2, y2);
  flood(fillcolor, (uint32_t)w * (uint32_t)h);
  setAddrWindow(0, 0, _width - 1, _height - 1);
}

//=======================================
void Adafruit_TFTLCD_8bit_ESP::fillScreen(uint16_t color)
{  
  
      CS_ACTIVE_CD_COMMAND;
    write8(0x40); write8(0x02); 
    write8(0x10); write8(0x22); 
    //default full-screen address window:
    setAddrWindow(2,0, _width + 1, _height -1);
    
    CS_ACTIVE; CD_DATA;
    flood(color,20480);   
    CD_COMMAND;
    write8(0x40); write8(0x00); 
  CS_IDLE;
  setAddrWindow(2,0, _width + 1, _height -1);
}

//=======================================
void Adafruit_TFTLCD_8bit_ESP::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  // Clip
    if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
    setAddrWindow(x, y, x, y);
    CS_ACTIVE; CD_DATA;
    write8 (color >>8);
    write8 (color & 0x00ff); 
    CS_IDLE;
}

//=======================================
// Draw an image bitmap (16bits per color) at the specified position from the provided buffer. //Untested !!!!!!!!!!
//=======================================
void Adafruit_TFTLCD_8bit_ESP::drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t * bitmap)
{
	if ( x>=0 && (x+w)<_width && y>=0 && (y+h)<=_height ) {
		// all pixel visible, do it in the fast way
		setAddrWindow(x,y,x+w-1,y+h-1);
		pushColors((uint16_t*)bitmap, w*h, true);
	} else {
		// some pixels outside visible area, do it in the classical way to disable off-screen points
		int16_t i, j;
		uint16_t * colorP = (uint16_t*)bitmap;
		for(j=0; j<h; j++) {
			for(i=0; i<w; i++ ) {
				drawPixel(x+i, y+j, *colorP++);
			}
		}
	}
}

//=======================================
// Issues 'raw' an array of 16-bit color values to the LCD; used  //Untested !!!!!!!!!!
// externally by BMP examples.  Assumes that setWindowAddr() has
// previously been set to define the bounds.  Max 255 pixels at
// a time (BMP examples read in small chunks due to limited RAM).
//=======================================
void Adafruit_TFTLCD_8bit_ESP::pushColors(uint16_t *data, int16_t len, boolean first)
{
  uint16_t color;
  uint8_t  hi, lo;
  CS_ACTIVE;
  if(first == true) { // Issue GRAM write command only on first call
    CD_COMMAND;
     write8(0x00);
    }
  CD_DATA;
  while(len--) {
    color = *data++;
    hi    = color >> 8; // Don't simplify or merge these
    lo    = color;      // lines, there's macro shenanigans
    write8(hi);         // going on.
    write8(lo);
  }
  CS_IDLE;
}

//=======================================
void Adafruit_TFTLCD_8bit_ESP::invertDisplay(boolean i)  //Does not work !!!!!!!!!!
{
		//writeCommand( i ? 0 : 1);
		CS_IDLE;
}
//=======================================
void Adafruit_TFTLCD_8bit_ESP::setRotation(uint8_t x){   //Limited operation !!!!!!
  // Call parent rotation func first -- sets up rotation flags, etc.
  Adafruit_GFX::setRotation(x);
  //Serial.print(x); Serial.print(' ');Serial.print(_width); Serial.print(' '); Serial.println(_height);
  // Then perform hardware-specific rotation operations...
    byte  t10,t40;
  switch (rotation) {
      case 1:  {t40=0x00; t10=0x22;break;}
      case 2:  {t40=0x02; t10=0x23; break;}//{t40=0x02; t10=0x22; break;}
      case 3:  {t40=0x02; t10=0x27; break;}//{t40=0x02; t10=0x25; break;}
      case 0:
      default: {t40=0x00; t10=0x22; break;}
    }
    CS_ACTIVE_CD_COMMAND;
    write8(0x40); write8(t40); 
    write8(0x10); write8(t10); 
    //default full-screen address window:
    setAddrWindow(0, 0, _width - 1, _height - 1); // CS_IDLE happens here
}


//Adafruit_TFTLCD_8bit_ESP tft;
