/* LedTable
 *
 * Written by: Greg Beckman
 * 
 * Audio system for the LED table
 */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
//#include <SD.h>
#include <SD_t3.h>
#include "AudioSampleT_stgclr.h"//1
#include "AudioSampleT_blkrot.h"//2
#include "AudioSampleT_hitsl.h"//3
#include "AudioSampleT_lndrop.h"//4
#include "AudioSampleT_lnrm.h"//5
#include "AudioSampleT_lnrm4.h"//6

// BCLK  = pin 9
// MCLK  = pin 11
// TX    = pin 22
// LRCLK = pin 23
// GUItool: begin automatically generated code
AudioPlayMemory          playMem1;       //xy=115,282
AudioPlaySdWav           playSdWav2;     //xy=117,368
AudioPlaySdWav           playSdWav1;     //xy=153,119
AudioMixer4              mixer1;         //xy=375,267
AudioOutputI2S           i2s1;           //xy=588,225
AudioConnection          patchCord1(playMem1, 0, mixer1, 1);
AudioConnection          patchCord2(playSdWav2, 0, mixer1, 2);
AudioConnection          patchCord3(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord4(mixer1, 0, i2s1, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 1);
// GUItool: end automatically generated code

float gain0 = 0.5;
float gain1, gain2;

char command;
char lastcommand;
char curMusicCommand;

uint16_t buttonSnapPin = 18;
uint16_t volumePin = 19;

uint16_t buttonSnap = 0;
uint16_t prevButtonSnap = 0;
uint16_t outputPin = 4;

//int mode = 0; //mode 0 == tetris, 1 == pong, 2 == high scores, 3 == god suit

int whichTetrisMusic = 0;
int sfxON = 1;
int musicON = 1;
bool musicFast = false;
bool lastSpeed = false;
int dontPlayOtherFX = 0;
int whichSnakeMusic = 0;
int snakeSfxON = 1;
int snakeMusicON = 1;
long interval = 1000;
long previousMillis = 0;
unsigned long currentMillis;
int delayTime = 10;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  11
#define SDCARD_MISO_PIN  12
#define SDCARD_SCK_PIN   13
//to use pin 14 teensy needs to be at 24 or 72 MHz -- i think we like 24 
//24 MHz doesnt work well with the neopixel strips


Sd2Card card;
SdVolume volume;


void setup() {
  Serial.begin(9600);
  Serial1.begin(4800);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  SPI.setMISO(SDCARD_MISO_PIN);

  randomSeed(analogRead(16));
  
  pinMode(buttonSnapPin,INPUT_PULLUP);
  pinMode(volumePin, INPUT);

  pinMode(outputPin,OUTPUT);
  digitalWrite(outputPin,LOW);

  boolean status;
  float size;

  delay(500);

   // First, detect the card
  status = card.init(SPI_FULL_SPEED, SDCARD_CS_PIN);
  if (!status) {
    return;
  }

  // Then look at the file system and print its capacity
  status = volume.init(card);
  if (!status) {
    return;
  }
  size = volume.blocksPerCluster() * volume.clusterCount();
  size = size * (512.0 / 1e6); // convert blocks to millions of bytes

  // Now open the SD card normally
  status = SD.begin(SDCARD_CS_PIN);
  if (!status) {
    return;
  }

  AudioMemory(30);

  delay(500);

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);
  mixer1.gain(2,0.5);
  //mixer1.gain(3,0.3);

  delay(1000);
}
// maybe each time serial1 is available check last command and if it differs stop playsd1 then end if
// then in new if do if is not playing and play according to command
//if it lags, barebones it to if not playing like how they do in tutorials.

void loop() {
  currentMillis = millis();
  gain0 = float(analogRead(volumePin))/1023;
  if(Serial1.available()){
    if(currentMillis - previousMillis > interval) {
      dontPlayOtherFX = 0;
    }
    command = Serial1.read();

    //if(command==
    //lastSpeed = musicFast;
    
    if(true){
      if(command == '+'){
        lastSpeed = musicFast;
        musicFast = true;
      }
      if(command == '-'){
        lastSpeed = musicFast;
        musicFast = false;
      }
      if(command == '0'){
        sfxON=0;
      }
      if(command == '1'){
        sfxON=1;
      }
      if(command == '@'){
        musicON=0;
      }
      if(command == '!'){
        musicON=1;
      }
      if(command == '2'){
        snakeSfxON=0;
      }
      if(command == '3'){
        snakeSfxON=1;
      }
      if(command == '#'){
        snakeMusicON=0;
      }
      if(command == '$'){
        snakeMusicON=1;
      }
      if(command == 'A'){
        whichTetrisMusic = 0;
      }
      if(command == 'B'){
        whichTetrisMusic = 1;
      }
      if(command == 'C'){
        whichTetrisMusic = 2;
      }
      if(command == 'D'){
        whichTetrisMusic = 3;
      }
      if(command == 'E'){
        whichTetrisMusic = 4;
      }
      if(command == 'F'){
        whichTetrisMusic = 5;
      }
      if(command == 'I'){
        whichTetrisMusic = 6;
      }
      if(command == 'J'){
        whichTetrisMusic = 7;
      }
      if(command == 'K'){
        whichTetrisMusic = 8;
      }
      if(command == 'L'){
        whichTetrisMusic = 9;
      }
      if(command == 'M'){
        whichTetrisMusic = 10;
      }
      if(command == 'Y'){
        whichSnakeMusic = 0;
      }
      if(command == 'X'){
        whichSnakeMusic = 1;
      }
      if(command == 'W'){
        whichSnakeMusic = 2;
      }
    }
    
    if(command == 'T'||command=='G'||command=='P'||command=='H'||command=='S' || command=='Z'||command=='V'||command=='U'){
      curMusicCommand = command;
    }
    
    if(musicON==1){
      if(command == 'T'){
        if(lastcommand!='T'||playSdWav1.isPlaying() == false){
          //playSdWav1.play("T_TITLE.WAV");
          playSdWav1.play("T_TITL2.WAV");
          delay(delayTime);
        }
      }
      if(command == 'G'){
        playSdWav1.play("T_GMOVR.WAV");
        delay(delayTime);
      }
      if(command == 'P' || command == '+' || command == '-'){
        if(lastcommand!='P'||playSdWav1.isPlaying() == false || lastSpeed!=musicFast){
          if(whichTetrisMusic==0){//&&musicFast==false){
            if(musicFast==false){
              playSdWav1.play("T_TYPEA.WAV");
            }else{
              playSdWav1.play("T_FTYPA.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==1){
            if(musicFast==false){
              playSdWav1.play("T_TYPEB.WAV");
            }else{
              playSdWav1.play("T_FTYPB.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==2){
            if(musicFast==false){
              playSdWav1.play("T_TYPEC.WAV");
            }else{
              playSdWav1.play("T_FTYPC.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==3){
            if(musicFast==false){
              playSdWav1.play("T_NES1.WAV");
            }else{
              playSdWav1.play("T_FNES1.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==4){
            if(musicFast==false){
              playSdWav1.play("T_NES2.WAV");
            }else{
              playSdWav1.play("T_FNES2.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==5){
            if(musicFast==false){
              playSdWav1.play("T_NES3.WAV");
            }else{
              playSdWav1.play("T_FNES3.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==6){
            if(musicFast==false){
              playSdWav1.play("T_REMIX1.WAV");
            }else{
              playSdWav1.play("T_FRMX1.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==7){
            if(musicFast==false){
              playSdWav1.play("T_REMIX2.WAV");
            }else{
              playSdWav1.play("T_FRMX2.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==8){
            if(musicFast==false){
              playSdWav1.play("T_REMIX3.WAV");
            }else{
              playSdWav1.play("T_FRMX3.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==9){
            if(musicFast==false){
              playSdWav1.play("T_REMIX4.WAV");
            }else{
              playSdWav1.play("T_FRMX4.WAV");
            }
            delay(delayTime);
          }else if(whichTetrisMusic==10){
            if(musicFast==false){
              playSdWav1.play("T_REMIX5.WAV");
            }else{
              playSdWav1.play("T_FRMX5.WAV");
            }
            delay(delayTime);
          }
        }
      }
      if(command == 'H'){
        if(lastcommand!='H'||playSdWav1.isPlaying() == false){
          playSdWav1.play("T_HS.WAV");
          delay(delayTime);
        }
      }
    }
    if(snakeMusicON==1){
      if(command == 'Z'){
        if(lastcommand!='Z'||playSdWav1.isPlaying() == false){
          playSdWav1.play("S_TITLE.WAV");
          delay(delayTime);
        }
      }
      if(command == 'U'){
        playSdWav1.play("S_GMOVR.WAV");
        delay(delayTime);
      }
      if(command == 'V'){
        if(lastcommand!='V'||playSdWav1.isPlaying() == false){
          if(whichSnakeMusic==0){
            playSdWav1.play("S_SLOW.WAV");
            delay(delayTime);
          }else if(whichSnakeMusic==1){
            playSdWav1.play("S_MED.WAV");
            delay(delayTime);
          }else if(whichSnakeMusic==2){
            playSdWav1.play("S_FAST.WAV");
            delay(delayTime);
          }
        }
      }
      if(command == 'Q'){
        if(lastcommand!='Q'||playSdWav1.isPlaying() == false){
          playSdWav1.play("T_HS.WAV");
          delay(delayTime);
        }
      }
    }
    if(command == 'S'){
      playSdWav1.stop();
    }

    if(sfxON==1){

      if(dontPlayOtherFX==0){
        
        if(command == 'r'){
          playMem1.play(AudioSampleT_blkrot);
        }
        if(command == 'h'){
          playMem1.play(AudioSampleT_hitsl);
        }
        if(command == 'd'){
          playMem1.play(AudioSampleT_lndrop);
        }
        if(command == 'l'){
          playMem1.play(AudioSampleT_lnrm);
        }
        if(command == 't'){
          playMem1.play(AudioSampleT_lnrm4);
          previousMillis = currentMillis;
          interval = 950;
          dontPlayOtherFX = 1;
        }
        if(command == 'c'){
          playMem1.play(AudioSampleT_stgclr);
          previousMillis = currentMillis;
          interval = 960;
          dontPlayOtherFX = 1;
        }
        if(command == 'p'){
          playSdWav2.play("T_PAUSE.WAV");
          delay(delayTime);
        }
  
      }
    }

    if(snakeSfxON==1){
      if(dontPlayOtherFX==0){
        if(command == 'a'){
          playMem1.play(AudioSampleT_blkrot);
        }
        if(command == 'b'){
          playMem1.play(AudioSampleT_hitsl);
        }
        if(command == 'e'){
          playSdWav2.play("S_PAUSE.WAV");
          delay(delayTime);
        }
      }
    }
    
    
    
    if(command == 'T'||command=='G'||command=='P'||command=='H'||command=='S' || command=='Z'||command=='V'||command=='U'){
      lastcommand = command;
    }
    //Z = snake title Menu
          //Y = snake slow music
          //X = snake med music
          //W = snake fast music
    //V = play selected snake music
    //U = stop snake music
    //2 = snake sfx off
    //3 = snake sfx on

  }

  if(musicON==1){
    if(playSdWav1.isPlaying()==false){
      //playSdWav1.stop();
      AudioProcessorUsageMaxReset();
      //AudioMemoryUsageMaxReset();
      if(curMusicCommand == 'T'){
        //playSdWav1.play("T_TITLE.WAV");
        playSdWav1.play("T_TITL2.WAV");
        delay(delayTime);
      } else if(curMusicCommand == 'P'){
        if(whichTetrisMusic==0){
          if(musicFast==false){
            playSdWav1.play("T_TYPEA.WAV");
          }else{
            playSdWav1.play("T_FTYPA.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==1){
          if(musicFast==false){
            playSdWav1.play("T_TYPEB.WAV");
          }else{
            playSdWav1.play("T_FTYPB.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==2){
          if(musicFast==false){
            playSdWav1.play("T_TYPEC.WAV");
          }else{
            playSdWav1.play("T_FTYPC.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==3){
          if(musicFast==false){
            playSdWav1.play("T_NES1.WAV");
          }else{
            playSdWav1.play("T_FNES1.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==4){
          if(musicFast==false){
            playSdWav1.play("T_NES2.WAV");
          }else{
            playSdWav1.play("T_FNES2.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==5){
          if(musicFast==false){
            playSdWav1.play("T_NES3.WAV");
          }else{
            playSdWav1.play("T_FNES3.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==6){
          if(musicFast==false){
            playSdWav1.play("T_REMIX1.WAV");
          }else{
            playSdWav1.play("T_FRMX1.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==7){
          if(musicFast==false){
            playSdWav1.play("T_REMIX2.WAV");
          }else{
            playSdWav1.play("T_FRMX2.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==8){
          if(musicFast==false){
            playSdWav1.play("T_REMIX3.WAV");
          }else{
            playSdWav1.play("T_FRMX3.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==9){
          if(musicFast==false){
            playSdWav1.play("T_REMIX4.WAV");
          }else{
            playSdWav1.play("T_FRMX4.WAV");
          }
          delay(delayTime);
        }else if(whichTetrisMusic==10){
          if(musicFast==false){
            playSdWav1.play("T_REMIX5.WAV");
          }else{
            playSdWav1.play("T_FRMX5.WAV");
          }
          delay(delayTime);
        }
      }
    }
  }
    if(snakeMusicON==1){
    if(playSdWav1.isPlaying()==false){
      //playSdWav1.stop();
      AudioProcessorUsageMaxReset();
      //AudioMemoryUsageMaxReset();
      if(curMusicCommand == 'Z'){
        playSdWav1.play("S_TITLE.WAV");
        delay(delayTime);
      } else if(curMusicCommand == 'V'){
        if(whichSnakeMusic==0){
          playSdWav1.play("S_SLOW.WAV");
          delay(delayTime);
        }else if(whichSnakeMusic==1){
          playSdWav1.play("S_MED.WAV");
          delay(delayTime);
        }else if(whichSnakeMusic==2){
          playSdWav1.play("S_FAST.WAV");
          delay(delayTime);
        }
      }
    }
  }
  
  gain1 = gain0*0.45;
  gain2 = gain0*0.45;
  mixer1.gain(0, gain1);
  mixer1.gain(1, gain2);
  mixer1.gain(2,gain2);
}
