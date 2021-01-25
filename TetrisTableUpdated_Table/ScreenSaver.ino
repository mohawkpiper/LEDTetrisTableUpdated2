const uint8_t kMatrixWidth = FIELD_HEIGHT;
const uint8_t kMatrixHeight = FIELD_WIDTH;

int PWMcurr = 6;
int PWMinc = 0;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = false;//true;

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  
  return i;
}

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
}

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;      
    for( byte x = 0; x < kMatrixWidth; x++) {
      int nXandY = x+(y*FIELD_HEIGHT);
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
//      if(nXandY%PWMcurr!=PWMinc){
         //       leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255); <--------------------------------------this should be commented out right? cuz it wasnt
//      }
//      else
//      {
//        leds[XY(x,y)] = CRGBW(0,0,0,0); 
//      }
    }
  }
  FastLED.setBrightness(BRIGHTNESS);
  showPixels();
  PWMinc++;
  if(PWMinc>=PWMcurr){PWMinc=0;}
}

void GridPattern()
{
  uint32_t ms = millis();
  //int32_t yHueDelta32 = ((int32_t)cos16( ms * (27/1) ) * (350 / kMatrixWidth));
  //int32_t xHueDelta32 = ((int32_t)cos16( ms * (39/1) ) * (310 / kMatrixHeight));
  int32_t yHueDelta32 = ((int32_t)cos16( ms * (20/1) ) * (350 / kMatrixWidth));
  int32_t xHueDelta32 = ((int32_t)cos16( ms * (29/1) ) * (310 / kMatrixHeight));
  DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
}

void DisplayPicture(){
  for(int i = 0; i < NUM_LEDS; i++) {
    setPixel(i, Image1[i]);
  }
  //adjust_gamma();
  //adjust_gamma_more();
  showPixels();
}



long previousScreenSaverMillis = 0; 
long screenSaverInterval = 10000;//30000;
unsigned long currentScreenSaverMillis;
int whichScreenSaver = 0;
int totNumPics = 10;

long unsigned int* picList[]  {Image1,Image2,Image3,Image4,Image5,Image6,Image7,Image8,Image9,Image10};
int gammaList[]               {     1,     1,     1,     2,     1,     2,     1,     2,     1,     1};


void runScreenSaver(void){
  clearTablePixels();

  previousScreenSaverMillis = currentScreenSaverMillis;
  long unsigned int *thisPic = picList[whichScreenSaver];

  for(int i = 0; i < NUM_LEDS; i++) {
    setPixel(i, thisPic[i]);
  }
  int thisGamma = gammaList[whichScreenSaver];
  if(thisGamma==1){
    adjust_gamma();
  }else if(thisGamma==2){
    adjust_gamma_more();
  }else{
    
  }
  showPixels();
 
  while(true){
    
    currentScreenSaverMillis = millis();

    if(currentScreenSaverMillis - previousScreenSaverMillis > screenSaverInterval) {
      previousScreenSaverMillis = currentScreenSaverMillis;
      whichScreenSaver++;
      if(whichScreenSaver>=totNumPics){
        whichScreenSaver=0;
      }
    }
    
    //Show menu system and wait for input
    boolean levelSelectionChanged = false;
    thisPic = picList[whichScreenSaver];
    for(int i = 0; i < NUM_LEDS; i++) {
      setPixel(i, thisPic[i]);
    }
    thisGamma = gammaList[whichScreenSaver];
    if(thisGamma==1){
      adjust_gamma();
    }else if(thisGamma==2){
      adjust_gamma_more();
    }else{
      
    }
    showPixels();
    

    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        levelSelectionChanged = true;
      } else if (curControl == BTN_UP){
        levelSelectionChanged = true;
      } else if (curControl == BTN_START){
        levelSelectionChanged = true;
      } else if (curControl == BTN_SELECT){
        levelSelectionChanged = true;
      } else if (curControl == BTN_LEFT){
        levelSelectionChanged = true;
      } else if (curControl == BTN_RIGHT){
        levelSelectionChanged = true;
      } else if (curControl == BTN_B){
        levelSelectionChanged = true;
      } else if (curControl == BTN_A){
        levelSelectionChanged = true;
      }
    }

    if (levelSelectionChanged){
      mainLoop();
      break;
    }
  }

}

////////rainbow animation////////////
#define UPDATES_PER_SECOND 150
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

boolean colorPaletteRunning;

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}



void runRainbowPalette(){
  static uint8_t startIndex = 0;
  unsigned long prevUpdateTime = 0;
  unsigned long curTime = 0;
  colorPaletteRunning = true;
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  clearTablePixels();
  showPixels();
  
  
  while(colorPaletteRunning){
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    
    FillLEDsFromPaletteColors( startIndex);
    showPixels();
    setDelay(1000/UPDATES_PER_SECOND);
    startIndex = startIndex + 1;
    //Check input keys
    do{
      readInput();
      if (curControl == BTN_SELECT){
        colorPaletteRunning = false;
        mainLoop();
        break;
      }
      curTime = millis();
      //delay(10);
    } 
    while ((curTime - prevUpdateTime) <10);//Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
  }
}


///////////////////////////////-----------------------------------------///////////////////////
///////////////////////////////-----------------------------------------///////////////////

// matrix size
uint8_t CentreX =  (FIELD_HEIGHT / 2) -1;
uint8_t CentreY = (FIELD_WIDTH / 2) - 1;

#define BRIGHTNESS    128


DEFINE_GRADIENT_PALETTE( pit ) {
  0,     3,   3,   3,
  64,   13,   13, 255,  //blue
  128,   3,   3,   3,
  192, 255, 130,   3 ,  //orange
  255,   3,   3,   3
};

// parameters and buffer for the noise array
#define NUM_LAYERS 1
uint32_t nx[NUM_LAYERS];
uint32_t ny[NUM_LAYERS];
uint32_t nz[NUM_LAYERS];
uint32_t scale_nx[NUM_LAYERS];
uint32_t scale_ny[NUM_LAYERS];
uint8_t  noise[1][FIELD_WIDTH][FIELD_WIDTH];




// cheap correction with gamma 2.0
void adjust_gamma()
{
  for (uint16_t i = 0; i < NUM_PIXELS; i++)
  {
    leds[i].r = (leds[i].r*0.3)+(dim8_video(leds[i].r)*0.7);
    leds[i].g = (leds[i].g*0.3)+(dim8_video(leds[i].g)*0.7);
    leds[i].b = (leds[i].b*0.3)+(dim8_video(leds[i].b)*0.7);
  }
}

void adjust_gamma_more()
{
  for (uint16_t i = 0; i < NUM_PIXELS; i++)
  {
    leds[i].r = dim8_video(leds[i].r);
    leds[i].g = dim8_video(leds[i].g);
    leds[i].b = dim8_video(leds[i].b);
  }
}

//as shown on youtube
//a noise controlled & modulated by itself
void noise_noise1() {

  CRGBPalette16 Pal( pit );
  int noiseSpeed = 10;
  int noiseZoom = 16;
  //modulate the position so that it increases/decreases x
  //(here based on the top left pixel - it could be any position else)
  //the factor "2" defines the max speed of the x movement
  //the "-255" defines the median moving direction
  nx[0] = nx[0] + (noiseSpeed * noise[0][0][0]) - 255;
  //modulate the position so that it increases/decreases y
  //(here based on the top right pixel - it could be any position else)
  ny[0] = ny[0] + (noiseSpeed * noise[0][FIELD_HEIGHT][0]) - 255;
  //z just in one direction but with the additional "1" to make sure to never get stuck
  //in case the movement is stopped by a crazy parameter (noise data) combination
  //(here based on the down left pixel - it could be any position else)
  nz[0] += ((noise[0][0][FIELD_WIDTH]));// 1+ //     / 4);
  //set the scaling based on left and right pixel of the middle line
  //here you can set the range of the zoom in both dimensions
  scale_nx[0] = 7000 + (noise[0][0][CentreX] * noiseZoom);
  scale_ny[0] = 7000 + (noise[0][FIELD_HEIGHT][CentreY] * noiseZoom);

  //calculate the noise data
  uint8_t layer = 0;
  for (uint8_t i = 0; i < FIELD_WIDTH; i++) {
    uint32_t ioffset = scale_nx[layer] * (i - CentreX);
    for (uint8_t j = 0; j < FIELD_WIDTH; j++) {
      uint32_t joffset = scale_ny[layer] * (j - CentreY);
      uint16_t data = inoise16(nx[layer] + ioffset, ny[layer] + joffset, nz[layer]);
      // limit the 16 bit results to the interesting range
      if (data < 11000) data = 11000;
      if (data > 51000) data = 51000;
      // normalize
      data = data - 11000;
      // scale down that the result fits into a byte
      data = data / 161;
      // store the result in the array
      noise[layer][i][j] = data;
    }
  }

  //map the colors
  for (uint8_t nyy = 0; nyy < FIELD_WIDTH; nyy++) {
    for (uint8_t nxx = 0; nxx < FIELD_HEIGHT; nxx++) {
      int nXandY = nxx+(nyy*FIELD_HEIGHT);
      //I will add this overlay CRGB later for more colors
      //it´s basically a rainbow mapping with an inverted brightness mask
      CRGB overlay = CHSV(noise[0][nyy][nxx], 255, noise[0][nxx][nyy]);
      //here the actual colormapping happens - note the additional colorshift caused by the down right pixel noise[0][15][15]
//      if(nXandY%PWMcurr!=PWMinc){
      leds[XY(nxx, nyy)] = ColorFromPalette( Pal, noise[0][FIELD_WIDTH][FIELD_WIDTH] + noise[0][nxx][nyy]) + overlay;
//      }
//      else
//      {
//        leds[XY(nxx,nyy)] = CRGBW(0,0,0,0); 
//      }
    }
  }

  //make it looking nice
  adjust_gamma();

  showPixels();
  PWMinc++;
  if(PWMinc>=PWMcurr){PWMinc=0;}
  //and show it!
  //FastLED.show();
}
