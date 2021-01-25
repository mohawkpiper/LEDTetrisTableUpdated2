/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty - Modified by: Mark Quinn - Modified again by: Greg Beckman
 * 
 * Initialize and base stuff for the LED table
 */
 
// fast led includes
#include <avr/pgmspace.h>  // Needed to store stuff in Flash using PROGMEM
#include "FastLED.h"       // Fastled library to control the LEDs
#include "FastLED_RGBW.h"  // added for rgbW leds

#include <EEPROMex.h> // write all high score data to EEPROM
int inputLong1 = 1;
char* inputChar1 = (char*)"AAA";
//tetris high scores regular
int addressLong[10] = {
                        EEPROM.getAddress(sizeof(long)),
                        EEPROM.getAddress(sizeof(long)),
                        EEPROM.getAddress(sizeof(long)),
                        EEPROM.getAddress(sizeof(long)),
                        EEPROM.getAddress(sizeof(long)),
                        EEPROM.getAddress(sizeof(long)),
                        EEPROM.getAddress(sizeof(long)),
                        EEPROM.getAddress(sizeof(long)),
                        EEPROM.getAddress(sizeof(long)),
                        EEPROM.getAddress(sizeof(long))
                        };
//tetris high score names regular
int addressCharArray[10] = {
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1)
                            };

int inputInt11 = 1;

//snake high scores regular
int addressInt[10] = {
                      EEPROM.getAddress(sizeof(int)),
                      EEPROM.getAddress(sizeof(int)),
                      EEPROM.getAddress(sizeof(int)),
                      EEPROM.getAddress(sizeof(int)),
                      EEPROM.getAddress(sizeof(int)),
                      EEPROM.getAddress(sizeof(int)),
                      EEPROM.getAddress(sizeof(int)),
                      EEPROM.getAddress(sizeof(int)),
                      EEPROM.getAddress(sizeof(int)),
                      EEPROM.getAddress(sizeof(int))
                      };
//snake high score names regular
int addressCharArray2[10] = {
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1),
                            EEPROM.getAddress((sizeof(char)*3)+1)
                            };


#include <Arduino.h>
#include <SPI.h>

////////////////////////////////////////////
//LED field size and shit //////////////////
 #define  FIELD_WIDTH      28
 #define  FIELD_HEIGHT     26
 const int NUM_LEDS=FIELD_WIDTH*FIELD_HEIGHT;

#define FAST_LED_CHIPSET WS2812B
#define BRIGHTNESS 64
#define MAX_BRIGHTNESS 50        // Thats full on, watch the power!
#define MIN_BRIGHTNESS 10       // set to a minimum of 10%
const int brightnessInPin = 14;

const int resetHSpin = 20;
int resetHS = 1;
 
// Define the Data Pin
#define DATA_PIN 2  // Connected to the data pin of the first LED strip

#define  NUM_PIXELS    FIELD_WIDTH*FIELD_HEIGHT

// FastLED with RGBW
CRGBW leds[NUM_PIXELS];
CRGB *ledsRGB = (CRGB *) &leds[0];
 
 
/* *** LED color table *** */ 

#define  RED        0xFA000000 //(250   0   0   0) theirs/mine
#define  REDORANGE  0xF00C0000 //(240  12   0   0) mine
#define  YLLWORANGE 0x9A440000 //(154  68   0   0) mine
#define  CHOCOLATE  0xB45A1E00 //(180  90  30   0) theirs (its like a faded yellow with pink
#define  YLLWGREEN  0x55AA0000 //( 85 170   0   0) mine
#define  GREEN      0x00FA0000 //(  0 250   0   0) theirs/mine
#define  DKGREEN    0x00BE0F00 //(  0 190  15   0) mine
#define  SEAGREEN   0x00C82800 //(  0 200  40   0) mine
#define  AQUA       0x00787800 //{  0 120 120   0) theirs/we use
#define  BLUE       0x0000FA00 //(  0   0 250   0) theirs/mine
#define  CURULEAN   0x00647800 //(  0 100 120   0) mine
#define  DKCURULEAN 0x0500BE00 //(  5   0 190   0) mine
#define  DKPURPLE   0x3200C800 //( 50   0 200   0) mine
#define  FUSCHIA    0xC8002800 //(200   0  40   0) mine
#define  PLUM       0xAF001400 //(175   0  20   0) mine
#define  PINKPURPLE 0x78008200 //(120   0 130   0) mine
#define  WHITE      0X0000005A //(  0   0   0  90) theirs/mine
#define  WHITERED   0x46000050 //( 70   0   0  80) mine
#define  WHITEGREEN 0x0041003C //(  0  65   0  60) mine
#define  WHITEBLUE  0x000A5037 //(  0  10  80  55) mine
#define  WHITEPRPLE 0x32006423 //( 50   0 100  35) mine
#define  WHITEPINK  0x46000050 //( 70   0   0  80) mine
#define  GREY       0x141E1E00 //( 20  30  30   0) mine
#define  LTGREY     0x0C0C0C18 //( 12  12  12  24) mine
#define  FRAMECOL   0x30240018 //( 48  36  00  22) faded yellow 
#define  BLACK      0x00000000 //(  0   0   0   0)
//#define  BLACK      0x00000018 //(  0   0   0  12) for testing purposes only cuz i cant see the real black in invisible mode

unsigned int long appleColorLib[9] = {WHITEPINK,WHITERED,WHITEGREEN,DKGREEN,GREEN,YLLWGREEN,YLLWORANGE,REDORANGE,RED};
unsigned int long snakeColorLib[8] = {BLUE,CURULEAN,DKCURULEAN,DKPURPLE,FUSCHIA,GREY,PINKPURPLE,WHITEBLUE};
unsigned int long colorLibLvl[10][3] = {  
                                          {WHITEBLUE,CURULEAN,DKCURULEAN},  //lv0  
                                          {WHITEGREEN,YLLWGREEN,DKGREEN},   //lv1
                                          {WHITEPRPLE,PINKPURPLE,DKPURPLE}, //lv2
                                          {WHITE,GREEN,BLUE},               //lv3
                                          {WHITEPINK,FUSCHIA,SEAGREEN},     //lv4
                                          {WHITEGREEN,SEAGREEN,CURULEAN},   //lv5
                                          {WHITERED,GREY,REDORANGE},        //lv6
                                          {WHITEPRPLE,PLUM,DKPURPLE},       //lv7
                                          {WHITE,RED,BLUE},                 //lv8
                                          {WHITERED,YLLWORANGE,REDORANGE}   //lv9
                                        };
 

/* *** Game commonly used defines ** */ 
#define  DIR_UP    1 
#define  DIR_DOWN  2 
#define  DIR_LEFT  3 
#define  DIR_RIGHT 4 


/* *** Bluetooth controller button defines and input method *** */ 
#define  BTN_NONE  0 
#define  BTN_UP    1 
#define  BTN_DOWN  2 
#define  BTN_LEFT  3 
#define  BTN_RIGHT 4 
#define  BTN_START 5 
#define  BTN_SELECT  6 
#define  BTN_A     7 
#define  BTN_B     8
#define  BTN_A_AND_LEFT  9
#define  BTN_A_AND_DOWN  10
#define  BTN_A_AND_RIGHT  11
#define  BTN_B_AND_LEFT  12
#define  BTN_B_AND_DOWN  13
#define  BTN_B_AND_RIGHT  14

uint8_t curControl = BTN_NONE; 
int state = 20;
int buttons[8];

#define MAINTEXTHEIGHT 7

char alphabet[41][2] = {{"_"},{"A"},{"B"},{"C"},{"D"},{"E"},{"F"},{"G"},{"H"},{"I"},{"J"},{"K"},{"L"},{"M"},{"N"},{"O"},{"P"},{"Q"},{"R"},{"S"},{"T"},{"U"},{"V"},{"W"},{"X"},{"Y"},{"Z"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"},{"0"},{"."},{","},{"!"},{" "}};

void readBlankInput(){
  curControl = BTN_NONE;

  if(Serial3.available() > 0){
    state = Serial3.read();
    int numInputs = 0;
    for (byte e = 0; e < 8; e++){
      buttons[e] = bitRead(state,7-e)? 1 : 0;
      if (buttons[e] == 0){
        numInputs++;
      }
    }
  }
}
void readInput(){
  curControl = BTN_NONE;

  if(Serial3.available() > 0){
    state = Serial3.read();
    int numInputs = 0;
    for (byte e = 0; e < 8; e++){
      buttons[e] = bitRead(state,7-e)? 1 : 0;
      if (buttons[e] == 0){
        numInputs++;
      }
    }
    if(numInputs==1){
      if(buttons[0] == 0){//value coming in from bluetooth controller = 127
          curControl = BTN_B;//<=============================
      }
      if(buttons[1] == 0){//value coming in from bluetooth controller = 191
          curControl = BTN_A;//<=============================
      }
      if(buttons[2] == 0){//value coming in from bluetooth controller = 223
          curControl = BTN_SELECT;
      }
      if(buttons[3] == 0){//value coming in from bluetooth controller = 239
          curControl = BTN_START;
      }
      if(buttons[4] == 0){//value coming in from bluetooth controller = 247
          curControl = BTN_UP;
      }
      if(buttons[5] == 0){//value coming in from bluetooth controller = 251
          curControl = BTN_DOWN;
      }
      if(buttons[6] == 0){//value coming in from bluetooth controller = 253
          curControl = BTN_LEFT;
      }
      if(buttons[7] == 0){//value coming in from bluetooth controller = 254
          curControl = BTN_RIGHT;
      }
    }else{
      if(buttons[0] == 0 && buttons[6] == 0){//value coming in = 125
          curControl = BTN_B_AND_LEFT;//<=============================
      }
      if(buttons[0] == 0 && buttons[5] == 0){//value coming in = 123
          curControl = BTN_B_AND_DOWN;//<=============================
      }
      if(buttons[0] == 0 && buttons[7] == 0){//value coming in = 126
          curControl = BTN_B_AND_RIGHT;//<=============================
      }
      if(buttons[1] == 0 && buttons[6] == 0){//value coming in = 189
          curControl = BTN_A_AND_LEFT;//<=============================
      }
      if(buttons[1] == 0 && buttons[5] == 0){//value coming in = 187
          curControl = BTN_A_AND_DOWN;//<=============================
      }
      if(buttons[1] == 0 && buttons[7] == 0){//value coming in = 190
          curControl = BTN_A_AND_RIGHT;//<=============================
      }
    }
  }
}
 
void initPixels(){ 
  FastLED.addLeds<FAST_LED_CHIPSET, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
} 
 
void setPixel(int n, int long color){ 
  leds[n] = CRGB(color); 
} 

void setPixelRGBW(int n, int long color){ 
  leds[n] = CRGBW(color); 
} 

void setPixelRGBW(int n, int long r,int long g, int long b, int long w){ 
  leds[n] = CRGBW(r,g,b,w); 
} 
 
void setDelay(int duration) { 
  FastLED.delay(duration); 
} 
 
int long getPixel(int n){ 
  return (leds[n].r << 24) + (leds[n].g << 16) + (leds[n].b << 8) + leds[n].w;
} 
 
void showPixels(){ 
  int mappedValue = map(analogRead(brightnessInPin), 0, 1023, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
  FastLED.setBrightness(mappedValue);
  FastLED.show(); 
} 
 
void setTablePixel(int x, int y, int long color){ 
  setPixel(x*FIELD_HEIGHT + y,color); 
}

void setTablePixelRGBW(int x, int y, int long color){ 
  setPixelRGBW(x*FIELD_HEIGHT + y,color); 
}

void setTablePixelRGBW(int x, int y, int long r,int long g, int long b, int long w){ 
  setPixelRGBW(x*FIELD_HEIGHT + y,r,g,b,w); 
} 

void clearTablePixels(){ 
  for (int n=0; n<FIELD_WIDTH*FIELD_HEIGHT; n++){ 
    setPixel(n,0); 
  } 
} 

void scrollText(char* text, int textLength, int yoffset, int long color, unsigned long textSpeed) {  
  unsigned long prevUpdateTime = 0;

  for (int x = FIELD_WIDTH; x > -(textLength * 8); x--) {
    printText(text, textLength, x, yoffset, color);
    //Read buttons
    unsigned long curTime;
    do {
      readInput();
      if (curControl != BTN_NONE){
        if (curControl == BTN_B || curControl == BTN_A){
          x=-(textLength * 8);
        }
      }
      curTime = millis();
    } while (((curTime - prevUpdateTime) < textSpeed));// && (curControl == BTN_NONE));//Once enough time  has passed, proceed

    prevUpdateTime = curTime;
  }
}

void scrollPixelText(char* text, int textLength, int yoffset, int long color, int textSpeed) {  
  unsigned long prevUpdateTime = 0;

  for (int x = FIELD_WIDTH; x > -(textLength * 6); x--) {
    printPixelText(text, textLength, x, yoffset, color);
    //Read buttons
    unsigned long curTime;
    do {
      readInput();
      if (curControl != BTN_NONE){
        if (curControl == BTN_B || curControl == BTN_A){
          x=-(textLength * 6);
        }
      }
      curTime = millis();
    } while (((curTime - prevUpdateTime) < textSpeed));// && (curControl == BTN_NONE));//Once enough time  has passed, proceed

    prevUpdateTime = curTime;
  }
}

void scrollSmallText(char* text, int textLength, int yoffset, int long color, int textSpeed) {  
  unsigned long prevUpdateTime = 0;

  for (int x = FIELD_WIDTH; x > -(textLength * 4); x--) {
    printSmallText(text, textLength, x, yoffset, color);
    //Read buttons
    unsigned long curTime;
    do {
      readInput();
      curTime = millis();
    } while (((curTime - prevUpdateTime) < (unsigned)textSpeed) && (curControl == BTN_NONE));//Once enough time  has passed, proceed

    prevUpdateTime = curTime;
  }
}

/* *** original text helper methods *** */ 
#include "font.h" 
uint8_t charBuffer[8][8]; 
 
void printText(char* text, unsigned int textLength, int xoffset, int yoffset, int long color){ 
  uint8_t curLetterWidth = 0; 
  int curX = xoffset; 
  clearTablePixels(); 
   
  //Loop over all the letters in the string 
  for (int i=0; i<textLength; i++){ 
    //Determine width of current letter and load its pixels in a buffer 
    curLetterWidth = loadCharInBuffer(text[i]); 
    //Loop until width of letter is reached 
    for (int lx=0; lx<curLetterWidth; lx++){ 
      //Now copy column per column to field (as long as within the field 
      if (curX>=FIELD_WIDTH){//If we are to far to the right, stop loop entirely 
         break; 
      } else if (curX>=0){//Draw pixels as soon as we are "inside" the drawing area 
        for (int ly=0; ly<8; ly++){//Finally copy column 
          setTablePixelRGBW(curX,yoffset+ly,charBuffer[lx][ly]*color); 
        } 
      } 
      curX++; 
    } 
  }   
  showPixels(); 
} 

//Load char in buffer and return width in pixels 
uint8_t loadCharInBuffer(char letter){ 
  uint8_t tmpCharWidth; 
   
  int letterIdx = (letter-32)*8; 
    
  int y=0; 
  for (int idx=letterIdx; idx<letterIdx+8; idx++){ 
    for (int x=0; x<8; x++){ 
      charBuffer[x][y] = ((font[idx]) & (1<<(7-x)))>0; 
    } 
    y++; 
  }  
  tmpCharWidth = 8; 
  return tmpCharWidth; 
} 

/* *** original text helper methods *** */ 
#include "fontSmallWide.h" 
uint8_t charSmallWideBuffer[8][6]; 
 
void printSmallWideText(char* text, unsigned int textLength, int xoffset, int yoffset, int long color){ 
  uint8_t curLetterWidth = 0; 
  int curX = xoffset; 
  clearTablePixels(); 
   
  //Loop over all the letters in the string 
  for (int i=0; i<textLength; i++){ 
    //Determine width of current letter and load its pixels in a buffer 
    curLetterWidth = loadSmallWideCharInBuffer(text[i]); 
    //Loop until width of letter is reached 
    for (int lx=0; lx<curLetterWidth; lx++){ 
      //Now copy column per column to field (as long as within the field 
      if (curX>=FIELD_WIDTH){//If we are to far to the right, stop loop entirely 
         break; 
      } else if (curX>=0){//Draw pixels as soon as we are "inside" the drawing area 
        for (int ly=0; ly<6; ly++){//Finally copy column 
          setTablePixelRGBW(curX,yoffset+ly,charSmallWideBuffer[lx][ly]*color); 
        } 
      } 
      curX++; 
    } 
  }   
  showPixels(); 
} 

void setSmallWideText(char* text, unsigned int textLength, int xoffset, int yoffset, int long color){ 
  uint8_t curLetterWidth = 0; 
  int curX = xoffset; 
   
  //Loop over all the letters in the string 
  for (int i=0; i<textLength; i++){ 
    //Determine width of current letter and load its pixels in a buffer 
    curLetterWidth = loadSmallWideCharInBuffer(text[i]); 
    //Loop until width of letter is reached 
    for (int lx=0; lx<curLetterWidth; lx++){ 
      //Now copy column per column to field (as long as within the field 
      if (curX>=FIELD_WIDTH){//If we are to far to the right, stop loop entirely 
         break; 
      } else if (curX>=0){//Draw pixels as soon as we are "inside" the drawing area 
        for (int ly=0; ly<6; ly++){//Finally copy column 
          setTablePixelRGBW(curX,yoffset+ly,charSmallWideBuffer[lx][ly]*color); 
        } 
      } 
      curX++; 
    } 
  }    
}

//Load char in buffer and return width in pixels 
uint8_t loadSmallWideCharInBuffer(char letter){ 
  uint8_t tmpCharWidth; 
   
  int letterIdx = (letter-32)*6; 
    
  int y=0; 
  for (int idx=letterIdx; idx<letterIdx+6; idx++){ 
    for (int x=0; x<6; x++){ 
      charSmallWideBuffer[x][y] = ((fontSmallWide[idx]) & (1<<(7-x)))>0; 
    } 
    y++; 
  }  
  tmpCharWidth = 6; 
  return tmpCharWidth; 
} 

/* *** text helper methods *** */ 
#include "fontPixelMix.h" 
uint8_t charPixelBuffer[6][8]; 

void printPixelText(char* text, unsigned int textLength, int xoffset, int yoffset, int long color){ 
  uint8_t curLetterWidth = 0; 
  int curX = xoffset; 
  clearTablePixels(); 
   
  //Loop over all the letters in the string 
  for (int i=0; i<textLength; i++){ 
    //Determine width of current letter and load its pixels in a buffer 
    curLetterWidth = loadPixelCharInBuffer(text[i]); 
    //Loop until width of letter is reached 
    for (int lx=0; lx<curLetterWidth; lx++){ 
      //Now copy column per column to field (as long as within the field 
      if (curX>=FIELD_WIDTH){
        //If we are to far to the right, stop loop entirely 
        break; 
      } else if (curX>=0){
        //Draw pixels as soon as we are "inside" the drawing area 
        for (int ly=0; ly<8; ly++){//Finally copy column 
          setTablePixelRGBW(curX,yoffset+ly,charPixelBuffer[lx][ly]*color); 
        } 
      } 
      curX++; 
    } 
  }    
  showPixels(); 
} 

void setPixelText(char* text, unsigned int textLength, int xoffset, int yoffset, int long color){ 
  uint8_t curLetterWidth = 0; 
  int curX = xoffset; 
  //Loop over all the letters in the string 
  for (int i=0; i<textLength; i++){ 
    //Determine width of current letter and load its pixels in a buffer 
    curLetterWidth = loadPixelCharInBuffer(text[i]); 
    //Loop until width of letter is reached 
    for (int lx=0; lx<curLetterWidth; lx++){ 
      //Now copy column per column to field (as long as within the field 
      if (curX>=FIELD_WIDTH){
        //If we are to far to the right, stop loop entirely 
        break; 
      } else if (curX>=0){
        //Draw pixels as soon as we are "inside" the drawing area 
        for (int ly=0; ly<8; ly++){//Finally copy column 
          setTablePixelRGBW(curX,yoffset+ly,charPixelBuffer[lx][ly]*color); 
        } 
      } 
      curX++; 
    } 
  }    
}
 
//Load char in buffer and return width in pixels 
uint8_t loadPixelCharInBuffer(char letter){  
  uint8_t tmpCharWidth; 
  int letterIdy = (letter-32)*6; 
  int x=0;
  for (int idy=letterIdy; idy<letterIdy+6; idy++){ 
    for (int y=0; y<8; y++){ 
      charPixelBuffer[x][y] = abs( 1-(  ((fontPixelMix[idy]) & (1<<(7-y)))>0)   );
    } 
    x++; 
  }   
  tmpCharWidth = 6; 
  return tmpCharWidth; 
} 

/* *** original text helper methods *** */ 
#include "fontSmall.h" 
uint8_t charSmallBuffer[4][6]; 
 
void printSmallText(char* text, unsigned int textLength, int xoffset, int yoffset, int long color){ 
  uint8_t curLetterWidth = 0; 
  int curX = xoffset; 
  clearTablePixels(); 
   
  //Loop over all the letters in the string 
  for (int i=0; i<textLength; i++){ 
    //Determine width of current letter and load its pixels in a buffer 
    curLetterWidth = loadSmallCharInBuffer(text[i]); 
    //Loop until width of letter is reached 
    for (int lx=0; lx<curLetterWidth; lx++){ 
      //Now copy column per column to field (as long as within the field 
      if (curX>=FIELD_WIDTH){//If we are to far to the right, stop loop entirely 
         break; 
      } else if (curX>=0){//Draw pixels as soon as we are "inside" the drawing area 
        for (int ly=0; ly<6; ly++){//Finally copy column 
          setTablePixelRGBW(curX,yoffset+ly,charSmallBuffer[lx][ly]*color); 
        } 
      } 
      curX++; 
    } 
  }   
  showPixels(); 
} 

void setSmallText(char* text, unsigned int textLength, int xoffset, int yoffset, int long color){ 
  uint8_t curLetterWidth = 0; 
  int curX = xoffset;  
   
  //Loop over all the letters in the string 
  for (int i=0; i<textLength; i++){ 
    //Determine width of current letter and load its pixels in a buffer 
    curLetterWidth = loadSmallCharInBuffer(text[i]); 
    //Loop until width of letter is reached 
    for (int lx=0; lx<curLetterWidth; lx++){ 
      //Now copy column per column to field (as long as within the field 
      if (curX>=FIELD_WIDTH){//If we are to far to the right, stop loop entirely 
         break; 
      } else if (curX>=0){//Draw pixels as soon as we are "inside" the drawing area 
        for (int ly=0; ly<6; ly++){//Finally copy column 
          setTablePixelRGBW(curX,yoffset+ly,charSmallBuffer[lx][ly]*color); 
        } 
      } 
      curX++; 
    } 
  }
} 

//Load char in buffer and return width in pixels 
uint8_t loadSmallCharInBuffer(char letter){ 
  uint8_t tmpCharWidth; 
   
  int letterIdx = (letter-32)*3;
    
  int y=0; 
  for (int idx=letterIdx; idx<letterIdx+3; idx++){ 
    for (int x=0; x<8; x++){ 
      y = int((idx-letterIdx)+(idx-letterIdx)+ int(floor(x/4)));
      int newx = x%4;
      charSmallBuffer[newx][y] = ((fontSmall[idx]) & (1<<(7-x)))>0; 
    } 
  }  
  tmpCharWidth = 4; 
  return tmpCharWidth; 
} 

/* *********************************** */ 

long previousFadeMillis = 0; 
long fadeInterval = 2;
unsigned long currentFadeMillis;
void fadeOut(){ 
  int tmpx = 0;
  
  while(tmpx<70){
    currentFadeMillis = millis();
    if(currentFadeMillis - previousFadeMillis > fadeInterval) {
      dimLeds(0.95); 
      showPixels(); 
      tmpx++;
      previousFadeMillis = currentFadeMillis;   
    }
  }
} 
 
void dimLeds(float factor){ 
  //Reduce brightness of all LEDs, typical factor is 0.97 
  for (int n=0; n<(FIELD_WIDTH*FIELD_HEIGHT); n++){ 
    int curColor = getPixel(n); 
    //Derive the tree colors 
    int long r = ((curColor & 0xFF000000)>>24); 
    int long g = ((curColor & 0x00FF0000)>>16); 
    int b = ((curColor & 0x0000FF00)>>8);
    int w = (curColor & 0x000000FF); 
    //Reduce brightness 
    r = r*factor; 
    g = g*factor; 
    b = b*factor;
    w = w*factor; 
    //Pack into single variable again 
    //curColor = (r<<16) + (g<<8) + b;
    curColor = (r<<24) + (g<<16) + (b<<8) + w; 
    //Set led again 
    setPixelRGBW(n,curColor); 
  } 
} 

void setup(){ 
  Serial.begin(9600);
  Serial2.begin(4800);
  Serial3.begin(38400);
  delay(1000);
  pinMode(brightnessInPin, INPUT);
  pinMode(resetHSpin, INPUT_PULLUP);
  delay(100);
  resetHS = digitalRead(resetHSpin);
  randomSeed(analogRead(16));
  EEPROM.setMaxAllowedWrites(600);
  if(resetHS == 0) {
    for(int i = 0; i<10; i++){
      while (!EEPROM.isReady()) { delay(1); }
      EEPROM.writeLong(addressLong[i],inputLong1+9-i);
    }
    for(int i = 0; i<10; i++){
      while (!EEPROM.isReady()) { delay(1); }
      EEPROM.writeBlock<char>(addressCharArray[i], inputChar1,3);
    }
    for(int i = 0; i<10; i++){
      while (!EEPROM.isReady()) { delay(1); }
      EEPROM.writeInt(addressInt[i],inputInt11+9-i);
    }
    for(int i = 0; i<10; i++){ 
      while (!EEPROM.isReady()) { delay(1); }
      EEPROM.writeBlock<char>(addressCharArray2[i], inputChar1,3);
    }
    
  }
/*  ///// uncomment to set the high scores when uploading for the first time. then recomment and upload again.
  for(int i = 0; i<10; i++){
    while (!EEPROM.isReady()) { delay(1); }
    EEPROM.writeLong(addressLong[i],inputLong1+9-i);
  }
  for(int i = 0; i<10; i++){
    while (!EEPROM.isReady()) { delay(1); }
    EEPROM.writeBlock<char>(addressCharArray[i], inputChar1,3);
  }
  for(int i = 0; i<10; i++){
    while (!EEPROM.isReady()) { delay(1); }
    EEPROM.writeInt(addressInt[i],inputInt11+9-i);
  }
  for(int i = 0; i<10; i++){ 
    while (!EEPROM.isReady()) { delay(1); }
    EEPROM.writeBlock<char>(addressCharArray2[i], inputChar1,3);
  }
*/
  //Initialise display 
  initPixels(); 
  showPixels(); 
 
 
  mainLoop();
} 
 
 
void loop(){ 
} 
