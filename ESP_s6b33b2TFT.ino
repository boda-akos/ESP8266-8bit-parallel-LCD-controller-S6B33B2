//Mr.Wang init
//CS D[0], RS D[10], WR D[9]
//TFT chip is s6b33b3 !! 0x10 register -and more- differences to s6b33b2
//HC595 clk 0, din 15, latch 2
byte x;
//byte  D[]={16,5,4,0,2,14,12,13,15,1,3};  //call TX pin D9, RX pin D10
  
#include "Adafruit_TFTLCD_8bit_ESP.h" 
Adafruit_TFTLCD_8bit_ESP tft;

int color[]={0xf100,0x07e0,0x001f,0xffff,YELLOW,MAGENTA};  //bl,gn,rd,wh

uint8_t conv2d(const char* p); 

uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time
byte i;
void setup() { 
  tft.begin();
  //write8(0); digitalWrite(TFT_RST,0);
  //while(1) {CD_COMMAND; CD_DATA; }//{i++; TFT_RS=i&0x01;TFT_RS=i&0x01;TFT_CS=i&0x01; }
	//Serial.begin(9600);
	int i=0; 
	//while (i<99) Serial.println(i++);		
  delay(100);
 // tft.setRotation(0);
 // digitalWrite(D[0],0);  digitalWrite(D[10],1); 
/*  for (int i=0; i<128*160; i++) {
  if (i<2048 ) lcdData(color[0]) ;
if (i<4096 && i>=2048) lcdData(color[1]); 
if (i<8192 && i>=4096) {if (i>(4096 + 10*128) && i<(4096 + 11*128) )  lcdData(color[4]); else lcdData(0);} 
if (i<10240 && i>=8192) lcdData(color[4]);
if (i<12288 && i>=10240) lcdData(color[3]);
if (i<16384 && i>=12288) lcdData(color[5]); //lcdData(random(255)*256+random(255)); 
if (i>=16384) lcdData(color[2]); 
}  
*/



tft.setRotation(1);
tft.setCursor(10,10); 
tft.setTextColor(WHITE,BLACK);tft.setTextWrap(1);
tft.setTextSize(2); 

tft.drawRoundRect(10,80,80,10,6,RED);
tft.drawLine(10,80,80,100,BLUE);
tft.drawCircle(10,80,8,BLUE);
tft.setCursor(80,60);tft.print("Tolna"); 

tft.drawFastHLine(5,5,150,CYAN);tft.drawFastHLine(5,6,150,CYAN);
tft.drawFastVLine(5,5,120,BLACK);
 delay(2000);
tft.fillScreen(BLACK);
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
  tft.setCursor(30,40); pr2(hh);tft.print(':');pr2(mm);tft.print(':');pr2(ss);
  }
}

 uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

void pr2(byte n)

{
  if (n<10) tft.print('0'); 
  tft.print(n);
  
  }
