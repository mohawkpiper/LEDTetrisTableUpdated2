/* LedTable
 *
 * Written by: Greg Beckman
 * 
 * Bluetooth controller system for the LED table
 */

int DATA_CLOCK    = 6;
int DATA_LATCH    = 7;
int DATA_SERIAL  = 12;

int buttons[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

int incomingState = 5;

uint8_t buttonState;
uint8_t prevButtonState;

uint8_t tmpButtonState = 255;

bool rotateActive = false;

uint8_t isFirstPress = 1;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 10;

long previousMillis = 0;
long interval = 290;
unsigned long currentMillis;

long tmpPreviousMillis = 0;
long tmpInterval = 100-debounceDelay;
unsigned long tmpCurrentMillis;

void setup(){
  Serial.begin(38400);
  setupPins();
}

void loop(){
  getControllerData();
}

void setupPins(void){
  // Set DATA_CLOCK normally HIGH
  pinMode(DATA_CLOCK, OUTPUT);
  digitalWrite(DATA_CLOCK, HIGH);
  
  // Set DATA_LATCH normally LOW
  pinMode(DATA_LATCH, OUTPUT);
  digitalWrite(DATA_LATCH, LOW);

  // Set DATA_SERIAL normally HIGH
  pinMode(DATA_SERIAL, OUTPUT);
  digitalWrite(DATA_SERIAL, HIGH);
  pinMode(DATA_SERIAL, INPUT);  
}

void getControllerData(void){
  // Latch for 12us
  digitalWrite(DATA_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(DATA_LATCH, LOW);
  delayMicroseconds(6);
  
  // Retrieve button presses from shift register by pulling the clock high for 6us
  for(int i = 0; i < 16; i++){
    digitalWrite(DATA_CLOCK, LOW);
    delayMicroseconds(6);
    if(i < 8){
        buttons[i] = digitalRead(DATA_SERIAL);
        
    }
    if(i==8){
      buttons[1] = digitalRead(DATA_SERIAL);
    }
    //Serial.write(digitalRead(DATA_SERIAL));
    digitalWrite(DATA_CLOCK, HIGH);
    delayMicroseconds(6);
  }
  buttonState = 0;
  for(int x = 0; x < 8; x++){
    buttonState *=2;
    buttonState +=buttons[x];
    
  }

  if(buttonState!=prevButtonState){
    lastDebounceTime = millis();
    if(buttonState==255){
      isFirstPress=1;
    }
  }

  if(Serial.available() > 0){
    incomingState = Serial.read();
    
    //5 = for menu... set tetris DAS left right to also up and down
    //10 = set tetris DAS left/ right only and down interval for buttonState==251 to something like 35 or 40
    //15 = brick landed. dont allow any movement presses until unpressed (rotate is allowed)
    //20 = we are in snake game. don't set any DAS... just the regular 100 frame interval for all directions
    //
    
  }
  
  currentMillis = millis();
  tmpCurrentMillis = millis();
          

  //first frame pushing it goes right away
  //then while holding next moves waits 0.27 seconds
  //then all after that wait 0.1 seconds

  if((millis() - lastDebounceTime) > debounceDelay){
    
    if(incomingState==5){//menus
      if(buttonState!=255){
        if(isFirstPress==1){
          Serial.write(buttonState);
          previousMillis = currentMillis;
          interval = 270;
          isFirstPress=0;
        }
        if(isFirstPress==0){
          if(currentMillis - previousMillis > interval){
            Serial.write(buttonState);
            previousMillis = currentMillis;
            interval = 100;
          }
        }
      }
    }
    
    /// if press A or B (with or without direction) we change rotateActive to TRUE until either 
    // button state == 255 or is a direction only state then we set rotateActive to FALSE
    // we can only allow rotates to happen if rotateActive is FALSE
    
    if(incomingState==10){//tetris DAS
      if(buttonState!=255&&buttonState!=251){
        if(isFirstPress==1){
          Serial.write(buttonState);
          previousMillis = currentMillis;
          tmpPreviousMillis = tmpCurrentMillis;
          interval = 260;
          isFirstPress=0;
          if(buttonState==127||buttonState==191){
            rotateActive=true;
          }
        }
        if(isFirstPress==0){
          if(currentMillis - previousMillis > interval){
            if(buttonState==125||buttonState==189){//BandLEFT or AandLEFT
              if(rotateActive==false){
                Serial.write(buttonState);
                rotateActive=true;
              }else{
                Serial.write(253);
                //do only the movement LEFT
              }
            }else if(buttonState==126||buttonState==190){//BandRIGHT or AandRIGHT
              if(rotateActive==false){
                Serial.write(buttonState);
                rotateActive=true;
              }else{
                Serial.write(254);
                //do only the movement RIGHT
              }
            }else if(buttonState==123||buttonState==187){//BandDOWN or AandDOWN
              if(rotateActive==false){
                Serial.write(buttonState);
                rotateActive=true;
              }else{
                Serial.write(251);
                //do only the movement DOWN
              }
            }else if(buttonState==127||buttonState==191){//B or A
              if(rotateActive==false){
                Serial.write(buttonState);
                rotateActive=true;
              }
            }else{
              Serial.write(buttonState);
            }
            previousMillis = currentMillis;
            interval = 95;
          }else if(interval==260){
            if(tmpCurrentMillis - tmpPreviousMillis > tmpInterval){
              tmpButtonState = buttonState;
              if(buttonState==125||buttonState==126){//BandLEFT or BandRight
                if(rotateActive==false){
                  rotateActive = true;
                  Serial.write(127);//B
                  tmpPreviousMillis = tmpCurrentMillis;
                  rotateActive = true;
                }
              }
              if(buttonState==189||buttonState==190){//AandLeft or AandRight
                if(rotateActive==false){
                  Serial.write(191);//A
                  tmpPreviousMillis = tmpCurrentMillis;
                  rotateActive = true;
                }
              }
            }
          }else{
            if((buttonState==125||buttonState==126||buttonState==123)&&rotateActive==false){//BandLEFT or BandRIGHT or BandDOWN
              Serial.write(127);
              rotateActive=true;
            }
            if((buttonState==189||buttonState==190||buttonState==187)&&rotateActive==false){//AandLEFT or AandRIGHT or AandDOWN
              Serial.write(191);
              rotateActive=true;
            }
          }
        }
      }
      if(buttonState==251){
        if(isFirstPress==1){
          Serial.write(buttonState);
          previousMillis = currentMillis;
          interval = 40;
          isFirstPress=0;
        }
        if(isFirstPress==0){
          if(currentMillis - previousMillis > interval){
            Serial.write(buttonState);
            previousMillis = currentMillis;
            interval = 40;
          }
        }
      }
    }
    
    if(incomingState==15){//tetris brick landed from pressing down
      if(buttonState!=255&&buttonState!=127&&buttonState!=191){
        if(buttonState==123&&rotateActive==false){
          Serial.write(127);
          rotateActive=true;
        }
        if(buttonState==187&&rotateActive==false){
          Serial.write(191);
          rotateActive=true;
        }
        
      }else{
        incomingState=10;
      }
    }
    
    if(incomingState==20){//snake
      if(buttonState!=255){
        if(isFirstPress==1){
          Serial.write(buttonState);
          previousMillis = currentMillis;
          interval = 50;
          isFirstPress=0;
        }
        if(isFirstPress==0){
          if(currentMillis - previousMillis > interval){
            Serial.write(buttonState);
            previousMillis = currentMillis;
            interval = 50;
          }
        }
      }
    }
  }

  if(buttonState==255||buttonState==247||buttonState==251||buttonState==253||buttonState==254){
    rotateActive=false;
  }
  
  prevButtonState = buttonState;
}
