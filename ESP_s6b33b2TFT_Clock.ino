//Mr.Wang init
//CS D[0], RS D[10], WR D[9]
//TFT chip is s6b33b3 !! 0x10 register -and more- differences to s6b33b2
//HC595 clk 0, din 15, latch 2
byte x;
//byte  D[]={16,5,4,0,2,14,12,13,15,1,3};  //call TX pin D9, RX pin D10
  
#include "Adafruit_TFTLCD_8bit_ESP.h" 
Adafruit_TFTLCD_8bit_ESP tft;

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg=0, mdeg=0, hdeg=0;
uint16_t osx=62, osy=62, omx=62, omy=62, ohx=62, ohy=62;  // Saved H, M, S x & y coords
int xx0=0, xx1=0, yy0=0, yy1=0;
uint32_t targetTime = 0;                    // for next 1 second timeout

boolean initial = 1;

uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time

void setup() { 
tft.begin();	
delay(100);
//Serial.begin(9600); int i=0; while (i<99) Serial.println(i++);   
tft.setRotation(1);
tft.fillScreen(BLACK);
  
  tft.setTextColor(WHITE, BLACK);  // Adding a background colour erases previous text automatically
  
  // Draw clock face
  tft.fillCircle(62, 62, 59, BLUE);
  tft.fillCircle(62, 62, 55, BLACK);

  // Draw 12 lines
  for(int i = 0; i<360; i+= 30) {
    sx = cos((i-90)*0.0174532925);
    sy = sin((i-90)*0.0174532925);
    xx0 = sx*57+62;
    yy0 = sy*57+62;
    xx1 = sx*50+62;
    yy1 = sy*50+62;

    tft.drawLine(xx0, yy0, xx1, yy1, YELLOW);
  }

  // Draw 60 dots
  for(int i = 0; i<360; i+= 6) {
    sx = cos((i-90)*0.0174532925);
    sy = sin((i-90)*0.0174532925);
    xx0 = sx*51+62;
    yy0 = sy*51+62;
    // Draw minute markers
    tft.drawPixel(xx0, yy0, WHITE);
    
    // Draw main quadrant dots
    if(i==0 || i==180) tft.fillCircle(xx0, yy0, 2, WHITE);
    if(i==90 || i==270) tft.fillCircle(xx0, yy0, 2, WHITE);
  }

  //tft.fillCircle(60, 62, 3, WHITE);

  targetTime = millis() + 1000; 
}


void loop(void){
static int i; 
static int targetTime;

  if (targetTime < millis()) {
    targetTime = millis()+1000;
    ss++;              // Advance second
    if (ss==60) {
      ss=0;
      mm++;            // Advance minute
      if(mm>59) {
        mm=0;
        hh++;          // Advance hour
        if (hh>23) {
          hh=0;
        }
      }
    }

    // Pre-compute hand degrees, x & y coords for a fast screen update
    sdeg = ss*6;                  // 0-59 -> 0-354
    mdeg = mm*6+sdeg*0.01666667;  // 0-59 -> 0-360 - includes seconds
    hdeg = hh*30+mdeg*0.0833333;  // 0-11 -> 0-360 - includes minutes and seconds
    hx = cos((hdeg-90)*0.0174532925);    
    hy = sin((hdeg-90)*0.0174532925);
    mx = cos((mdeg-90)*0.0174532925);    
    my = sin((mdeg-90)*0.0174532925);
    sx = cos((sdeg-90)*0.0174532925);    
    sy = sin((sdeg-90)*0.0174532925);

    if (ss==0 || initial) {
      initial = 0;
      // Erase hour and minute hand positions every minute
      tft.drawLine(ohx, ohy, 62, 62, BLACK);
      ohx = hx*31+62;    
      ohy = hy*31+62;
      tft.drawLine(omx, omy, 62, 62, BLACK);
      omx = mx*42+62;    
      omy = my*42+62;
    }

      // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
      tft.drawLine(osx, osy, 62, 62, BLACK);
      osx = sx*45+62;    
      osy = sy*45+62;
      tft.drawLine(osx, osy, 62, 62, RED);
      tft.drawLine(ohx, ohy, 62, 62, WHITE);
      tft.drawLine(omx, omy, 62, 62, WHITE);
      tft.drawLine(osx, osy, 62, 62, RED);
      tft.fillCircle(62, 62, 3, RED);
     
      tft.setCursor(129,20); tft.print("TIME");
      tft.setCursor(126,30); tft.print("FLIES");
      tft.setCursor(132,50); tft.print(hh);tft.print(":h");
      tft.setCursor(132,60); if (mm<10) tft.print("0"); tft.print(mm);tft.print(":m");
      tft.setCursor(132,70); if (ss<10) tft.print("0"); tft.print(ss);tft.print(" s");
      
      tft.fillRect(130,90,20,20,random(255<<8) | random(255)); 
      tft.setAddrWindow(133,93,147,107); CD_DATA;  CS_ACTIVE;
      for (byte i=0; i<14*14; i++) {write8(random(255)); write8(random(255));}
  }
}



void pr2(byte n)

{
  if (n<10) tft.print('0'); 
  tft.print(n);
   }
   
