/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty - Modified by: Greg Beckman
 * 
 * Snake game
 */
#define SNAKE_FIELD_WIDTH    20
#define SNAKE_FIELD_HEIGHT   19
#define SNAKE_FIELD_YOFF     6
#define SNAKE_FIELD_XOFF     4
uint16_t lvlTime[3] = {200,110,70};
int snakeColor;

void snakeInit(uint8_t maze){
  Serial3.write(uint8_t(20));// back to controller, says we are in snake mode
  
  //Snake start position and direction & initialise variables
  curLength = 3;
  snakeColor = random(8);
  xs[0]=3; xs[1]=2; xs[2]=1;
  ys[0]=SNAKE_FIELD_HEIGHT/2; ys[1]=SNAKE_FIELD_HEIGHT/2; ys[2]=SNAKE_FIELD_HEIGHT/2;
  dir = DIR_RIGHT;
  score = 0;
  //Generate random apple position
  if(maze==0){
    ax = random(SNAKE_FIELD_WIDTH-1);
    ay = random(SNAKE_FIELD_HEIGHT-1);
  }else if(maze==1){
    ax = random(SNAKE_FIELD_WIDTH-1);
    ay = random(1,SNAKE_FIELD_HEIGHT-2);
  }else if(maze==2){
    ax = random(1,SNAKE_FIELD_WIDTH-2);
    ay = random(1,SNAKE_FIELD_HEIGHT-2);
  }
  //setVisibleSnakeScore();
  //printSmallText((char*)"00", 2, SNAKE_FIELD_WIDTH+SNAKE_FIELD_XOFF, -1, AQUA);
  snakeGameOver = false;
}

void runSnake(uint8_t whichLvl, uint8_t whichMaze){
  Serial2.write('V');
  Serial2.flush();
  snakeInit(whichMaze);
  unsigned long prevUpdateTime = 0;
  boolean snakeRunning = true;
  while(snakeRunning){    
    //Check self-collision with snake
    int i=curLength-1;
    while (i>=2){
      if (collide(xs[0], xs[i], ys[0], ys[i], SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH)){
        die();
      }
      i = i-1;
    }
    
    if (snakeGameOver){
      Serial2.write('U');
      Serial2.flush();
      fadeOut();
      Serial3.write(uint8_t(5));// back to controller, says we are in menu mode
      char* SGOBuf;
      SGOBuf = (char*)"Game Over";
      int SGOspeed = 30;
      scrollText(SGOBuf, 9, 7, AQUA, SGOspeed);
      //High score code here
      int CurrentSHS[10];
      for(int i = 0; i <10; i++){
        CurrentSHS[i] = EEPROM.readInt(addressInt[i]);
      }

      char CurrentSHSnames[10][4] = {{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "}};
      for(int i = 0; i <10; i++){
        EEPROM.readBlock<char>(addressCharArray2[i],CurrentSHSnames[i],3);
      }

      if ( score > (unsigned)CurrentSHS[9] ) {
        Serial2.write('Q');
        Serial2.flush();
        
        char* SHSbuf;
        SHSbuf = (char*)"NEW HIGH SCORE";
        scrollText(SHSbuf, 14, 7, WHITE, SGOspeed);
////// enter score?//////////////////////////////////////////////////////////////////////////
        Serial3.write(uint8_t(5));// back to controller, says we are in menu mode
        clearTablePixels();
        setSmallText((char*)"ENTER?", 6, 2, 1, AQUA);
        int esON = 1;
          
        if(esON==1){
          setSmallText((char*)"YES", 3, 8, menuItemYval, WHITE);//on
        }else if(esON==0){
          setSmallText((char*)"NO", 2, 10, menuItemYval, WHITE);//off
        }
        showPixels();
        
        while(true){
          //Show menu system and wait for input
          clearTablePixels();
          setSmallText((char*)"ENTER?", 6, 2, 1, AQUA);
          boolean esSelectionChanged = false;
          boolean exitEsSelection = false;
          //boolean runEs = false;
          currentMenuMillis = millis();
          
          if(currentMenuMillis - previousMenuMillis > menuInterval) {
            previousMenuMillis = currentMenuMillis;
            cursorOn++;
            if(cursorOn>1){
              cursorOn=0;
            }
            esSelectionChanged = true;
          }
          readInput();
          if (curControl != BTN_NONE){
            if (curControl == BTN_DOWN || curControl == BTN_UP){
              esON++;
              if (esON>1){
                esON = 0;
              }
              esSelectionChanged = true;
            } else if (curControl == BTN_A || BTN_B || curControl == BTN_SELECT || curControl == BTN_START){
              exitEsSelection = true;
            }
          }
      
        
          if (esSelectionChanged){
            if(cursorOn==0){
              setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
              setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
            }else{
              setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
              setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
            }
            if(esON==0){
              setSmallText((char*)"NO", 2, 10, menuItemYval, WHITE);//off
            }else if(esON==1){
              setSmallText((char*)"YES", 3, 8, menuItemYval, WHITE);//on
            }
            showPixels();
          }
          
          //If we are here, it means a selection was changed or a game started
          if (exitEsSelection){
            if(esON==0){
              fadeOut();
              curSnakeMenuSelection = 0;
              mainLoop();
              break;
            }
            if(esON==1){
              break;
            }
            
          }
        }
///////////////////////////////////////////////////////////////
        int checkScorePosition = 0;
        while(score<CurrentSHS[checkScorePosition]){
          checkScorePosition++;
        }
        EEPROM.writeInt(addressInt[checkScorePosition],score);
        for(int i = checkScorePosition; i < 9; i ++){
          while (!EEPROM.isReady()) { delay(1); }
          EEPROM.writeInt(addressInt[i+1],CurrentSHS[i]);
          while (!EEPROM.isReady()) { delay(1); }
          EEPROM.writeBlock<char>(addressCharArray2[i+1], CurrentSHSnames[i],3);
        }
        runSnakeEnterInitials(checkScorePosition);
        
      }else{
        char sbuf[3];
        int lenSScr = sprintf(sbuf, "%i", score);
        scrollText(sbuf, lenSScr, 7, WHITE, 38);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Disable loop and exit to main menu of led table
        snakeRunning = false;
        curSnakeMenuSelection = 0;
        break;
      }

    }

    //Check collision of snake head with apple
    if (collide(xs[0], ax, ys[0], ay, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH)){
      //Increase score and snake length;
      Serial2.write('a');
      Serial2.flush();
      score = score +(1+(whichLvl*3)+whichMaze);//whichMaze
      curLength = curLength +1;
      //Add snake segment with temporary position of new segments
      xs[curLength-1] = 255;
      ys[curLength-1] = 255;
      //Generate new apple position
      if(whichMaze==0){
        ax = random(SNAKE_FIELD_WIDTH-1);
        ay = random(SNAKE_FIELD_HEIGHT-1);
      }else if(whichMaze==1){
        ax = random(SNAKE_FIELD_WIDTH-1);
        ay = random(1,SNAKE_FIELD_HEIGHT-2);
      }else if(whichMaze==2){
        ax = random(1,SNAKE_FIELD_WIDTH-2);
        ay = random(1,SNAKE_FIELD_HEIGHT-2);
      }
      //Generate apple color
      acolor = appleColorLib[random(9)];

      int i=curLength-1;
      for(int i=0; i<curLength; i++) {
        if (collide(ax, xs[i], ay, ys[i], SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH, SNAKEWIDTH)){
           if(whichMaze==0){
              ax = random(SNAKE_FIELD_WIDTH-1);
              ay = random(SNAKE_FIELD_HEIGHT-1);
            }else if(whichMaze==1){
              ax = random(SNAKE_FIELD_WIDTH-1);
              ay = random(1,SNAKE_FIELD_HEIGHT-2);
            }else if(whichMaze==2){
              ax = random(1,SNAKE_FIELD_WIDTH-2);
              ay = random(1,SNAKE_FIELD_HEIGHT-2);
            }
           i=0;
        }
    }
    }
    
    //Shift snake position array by one
    i = curLength-1;
    while (i>=1){
      xs[i] = xs[i-1];
      ys[i] = ys[i-1];
      i = i-1;
    }
    //Determine new position of head of snake
    if (dir == DIR_RIGHT){
      xs[0] = xs[0] + SNAKEWIDTH;
    } 
    else if (dir == DIR_LEFT){
      xs[0] = xs[0] - SNAKEWIDTH;
    } 
    else if (dir == DIR_UP){
      ys[0] = ys[0] - SNAKEWIDTH;
    } 
    else {//DOWN
      ys[0] = ys[0] + SNAKEWIDTH;
    }
    
    //Check if outside playing field
    if(whichMaze==0){
      if ((xs[0]<0) || (xs[0]>=SNAKE_FIELD_WIDTH) || (ys[0]<0) || (ys[0]>=SNAKE_FIELD_HEIGHT)){
        if (xs[0]<0) {
          xs[0] =SNAKE_FIELD_WIDTH -1;
        }else if (xs[0]>=SNAKE_FIELD_WIDTH) {
          xs[0] = 0;
        }else if (ys[0]<0) {
          ys[0] =SNAKE_FIELD_HEIGHT -1;
        }else if (ys[0]>=SNAKE_FIELD_HEIGHT) {
          ys[0] = 0;
        }    
      }
    }else if(whichMaze==1){
      if ((xs[0]<0) || (xs[0]>=SNAKE_FIELD_WIDTH) || (ys[0]<1) || (ys[0]>=SNAKE_FIELD_HEIGHT-1)){
        if (xs[0]<0) {
          xs[0] =SNAKE_FIELD_WIDTH -1;
        }else if (xs[0]>=SNAKE_FIELD_WIDTH) {
          xs[0] = 0;
        }else if (ys[0]<1||ys[0]>=SNAKE_FIELD_HEIGHT-1) {
          die();
        }   
      }
    }else if(whichMaze==2){
      if ((xs[0]<1) || (xs[0]>=SNAKE_FIELD_WIDTH-1) || (ys[0]<1) || (ys[0]>=SNAKE_FIELD_HEIGHT-1)){
        die();  
      }
    }
    
    clearTablePixels();
    updateVisibleSnakeScore();

    //print maze
    if(whichMaze>0){
      for(int fx=SNAKE_FIELD_XOFF;fx<SNAKE_FIELD_WIDTH+SNAKE_FIELD_XOFF;fx++){
        setTablePixelRGBW(fx,SNAKE_FIELD_YOFF,WHITE);
        setTablePixelRGBW(fx,SNAKE_FIELD_HEIGHT+SNAKE_FIELD_YOFF-1,WHITE);
      }
    }
    if(whichMaze==2){
      for(int fy=SNAKE_FIELD_YOFF+1;fy<SNAKE_FIELD_HEIGHT+SNAKE_FIELD_YOFF-1;fy++){
        setTablePixelRGBW(SNAKE_FIELD_XOFF,fy,WHITE);
        setTablePixelRGBW(SNAKE_FIELD_WIDTH+SNAKE_FIELD_XOFF-1,fy,WHITE);
      }
    }
    
    //Draw apple
    setTablePixelRGBW (ax+SNAKE_FIELD_XOFF,ay+SNAKE_FIELD_YOFF,acolor);//YELLOW);
    
    //Draw snake
    for (int i=0; i<curLength; i++){
      float divisor = (float)i/(float)curLength;
      uint8_t sr = (int)floor(((WHITE >> 24) & 0xFF)*divisor);
      uint8_t sg = (int)floor(((WHITE >> 16) & 0xFF)*divisor);
      uint8_t sb = (int)floor(((WHITE >> 8) & 0xFF)*divisor);
      uint8_t sw = (int)floor(((WHITE >> 0) & 0xFF)*divisor);
      uint8_t sr2 = (int)floor(((snakeColorLib[snakeColor] >> 24) & 0xFF)*(1-divisor));
      uint8_t sg2 = (int)floor(((snakeColorLib[snakeColor] >> 16) & 0xFF)*(1-divisor));
      uint8_t sb2 = (int)floor(((snakeColorLib[snakeColor] >> 8) & 0xFF)*(1-divisor));
      uint8_t sw2 = (int)floor(((snakeColorLib[snakeColor] >> 0) & 0xFF)*(1-divisor));
      setTablePixelRGBW(xs[i]+SNAKE_FIELD_XOFF, ys[i]+SNAKE_FIELD_YOFF, sr+sr2,sg+sg2,sb+sb2,sw+sw2);
    }

//print game frame
    for(int fy=SNAKE_FIELD_YOFF-1;fy<SNAKE_FIELD_HEIGHT+SNAKE_FIELD_YOFF+1;fy++){
      setTablePixelRGBW(SNAKE_FIELD_XOFF-1,fy,FRAMECOL);
      setTablePixelRGBW(SNAKE_FIELD_WIDTH+SNAKE_FIELD_XOFF,fy,FRAMECOL);
    }
    for(int fx=SNAKE_FIELD_XOFF;fx<SNAKE_FIELD_WIDTH+SNAKE_FIELD_XOFF;fx++){
      setTablePixelRGBW(fx,SNAKE_FIELD_YOFF-1,FRAMECOL);
      setTablePixelRGBW(fx,SNAKE_FIELD_HEIGHT+SNAKE_FIELD_YOFF,FRAMECOL);
    }
    showPixels();

    //Check buttons and set snake movement direction while we are waiting to draw the next move
    unsigned long curTime;
    boolean dirChanged = false;
    do{
      readInput();
      if (curControl == BTN_START){
        pauseSnake();
        //break;
      }
      if (curControl != BTN_NONE && !dirChanged){//Can only change direction once per loop
        dirChanged = true;
        setDirection();
      }
      curTime = millis();
    } 
    while ((curTime - prevUpdateTime) <lvlTime[whichLvl]);//250);//Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
  }
  
  //fadeOut();
}

/* Set direction from current button state */
void setDirection(){
  switch(curControl){
    case BTN_LEFT:
    if(dir==DIR_RIGHT){
      break;
    } else{
      dir = DIR_LEFT;
      break;
    }
    case BTN_RIGHT:
    if(dir==DIR_LEFT){
      break;
    } else{
      dir = DIR_RIGHT;
      break;   
    }
    case BTN_DOWN:
    if(dir==DIR_UP){
      break;
    }else{
      dir = DIR_DOWN;
      break;
    }
    case BTN_UP:
    if(dir==DIR_DOWN){
      break;
    }else{
      dir = DIR_UP;
      break;
    }
    case BTN_START:
      break;
  }
}

/* Ending, show score */
void die(){
  Serial2.write('b');
  Serial2.flush();
  snakeGameOver = true;
}

/* Collision detection function */
boolean collide(int x1, int x2, int y1, int y2, int w1, int w2, int h1, int h2){
  if ((x1+w1>x2) && (x1<x2+w2) && (y1+h1>y2) && (y1<y2+h2)){
    return true;
  } 
  else {
    return false;
  }
}

void setVisibleSnakeScore(){
  printSmallText((char*)"000000", 6, 1, -1, AQUA);
}

void updateVisibleSnakeScore(){
  char bufSnakeVisibleScore[6];
  int lenSnakeVisibleScore = sprintf(bufSnakeVisibleScore, "%i", score);
  if(lenSnakeVisibleScore==1){
    setSmallText(bufSnakeVisibleScore, lenSnakeVisibleScore, 25, -1, AQUA);
  }
  if(lenSnakeVisibleScore==2){
    setSmallText(bufSnakeVisibleScore, lenSnakeVisibleScore, 21, -1, AQUA);
  }
  if(lenSnakeVisibleScore==3){
    setSmallText(bufSnakeVisibleScore, lenSnakeVisibleScore, 17, -1, AQUA);
  }
  if(lenSnakeVisibleScore==4){
    setSmallText(bufSnakeVisibleScore, lenSnakeVisibleScore, 13, -1, AQUA);
  }
  
}

void pauseSnake(){
  Serial3.write(uint8_t(5));// back to controller, says we are in menu mode
  Serial2.write('S');
  Serial2.flush();
  Serial2.write('e');
  Serial2.flush();
  int whichSnakePauseSelect = 0;
  clearTablePixels();
  setSmallText((char*)"RESUME", 6, 2, 6, WHITE);
  setSmallText((char*)"QUIT", 4, 6, 14, AQUA);
  showPixels();

  while(true){
    clearTablePixels();

    boolean pauseSnakeSelectionChanged = false;
    boolean quitSnakeGame = false;
    boolean resumeSnakeGame = false;
    
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_UP){
        whichSnakePauseSelect++;
        if(whichSnakePauseSelect>1){
          whichSnakePauseSelect=0;
        }
        pauseSnakeSelectionChanged = true;
      } else if (curControl == BTN_SELECT || curControl == BTN_A){
        if(whichSnakePauseSelect == 0){
          resumeSnakeGame = true;
        } else {
          quitSnakeGame = true;
        }
      }
    }
    if (pauseSnakeSelectionChanged){
      if(whichSnakePauseSelect==0){
        setSmallText((char*)"RESUME", 6, 2, 6, WHITE);
        setSmallText((char*)"QUIT", 4, 6, 14, AQUA);
      } else {
        setSmallText((char*)"RESUME", 6, 2, 6, AQUA);
        setSmallText((char*)"QUIT", 4, 6, 14, WHITE);
      }
      showPixels();
    }
    if(resumeSnakeGame){
      //start back into snake
      Serial3.write(uint8_t(20));// back to controller, says we are in snake mode
      Serial2.write('V');
      Serial2.flush();
      clearTablePixels();
      break;
    }
    if(quitSnakeGame){
      fadeOut();
      mainLoop();
      curSnakeMenuSelection = 0;
      break;
    }
    
  }
}

void runSnakeEnterInitials(int highSnakeScoreNum){
  Serial3.write(uint8_t(5));// back to controller, says we are in menu mode
  clearTablePixels();
  char bufYourSnakeScore[3];
  int lenYourSnakeScore = sprintf(bufYourSnakeScore, "%i", score);
  int snakeScoreOffset = (3 - lenYourSnakeScore)*4;
  int whichSLetterSelect = 0;
  int letterSSelect1 = 0;
  int letterSSelect2 = 0;
  int letterSSelect3 = 0;
  char sbuf[4];
      
    
  
  setSmallText(bufYourSnakeScore, lenYourSnakeScore, snakeScoreOffset+9, -1, AQUA);
  
  setPixelText(alphabet[letterSSelect1], 1, 5, 7, WHITE);
  setPixelText(alphabet[letterSSelect2], 1, 11, 7, AQUA);
  setPixelText(alphabet[letterSSelect3], 1, 17, 7, AQUA);

  showPixels();

  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText(bufYourSnakeScore, lenYourSnakeScore, snakeScoreOffset+9, -1, AQUA);

    boolean initialsSnakeSelectionChanged = false;
    boolean exitSnakeInitials = false;

    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        if(whichSLetterSelect == 0){
          letterSSelect1--;
          if(letterSSelect1<0){
            letterSSelect1=40;
          }
        }
        if(whichSLetterSelect == 1){
          letterSSelect2--;
          if(letterSSelect2<0){
            letterSSelect2=40;
          }
        }
        if(whichSLetterSelect == 2){
          letterSSelect3--;
          if(letterSSelect3<0){
            letterSSelect3=40;
          }
        }
        initialsSnakeSelectionChanged = true;
      } else if (curControl == BTN_UP){
        if(whichSLetterSelect == 0){
          letterSSelect1++;
          if(letterSSelect1>=41){
            letterSSelect1=0;
          }
        }
        if(whichSLetterSelect == 1){
          letterSSelect2++;
          if(letterSSelect2>=41){
            letterSSelect2=0;
          }
        }
        if(whichSLetterSelect == 2){
          letterSSelect3++;
          if(letterSSelect3>=41){
            letterSSelect3=0;
          }
        }
        initialsSnakeSelectionChanged = true;
      } else if (curControl == BTN_LEFT){
        whichSLetterSelect--;
        if(whichSLetterSelect<0){
          whichSLetterSelect=0;
        }
        initialsSnakeSelectionChanged = true;
      } else if (curControl == BTN_RIGHT){
        whichSLetterSelect++;
        if(whichSLetterSelect>=3){
          whichSLetterSelect=2;
        }
        initialsSnakeSelectionChanged = true;
      } else if (curControl == BTN_START){
        exitSnakeInitials = true;
      } else if (curControl == BTN_SELECT){
        exitSnakeInitials = true;
      } else if (curControl == BTN_A){
        if(whichSLetterSelect<2){
          whichSLetterSelect++;
          initialsSnakeSelectionChanged = true;
        }else if (whichSLetterSelect==2){
          exitSnakeInitials = true;
        }
      } else if (curControl == BTN_B){
        whichSLetterSelect--;
        if(whichSLetterSelect<0){
          whichSLetterSelect=0;
        }
        initialsSnakeSelectionChanged = true;
      }
    }

  
    if (initialsSnakeSelectionChanged){
      if(whichSLetterSelect==0){
        setPixelText(alphabet[letterSSelect1], 1, 5, 7, WHITE);
        setPixelText(alphabet[letterSSelect2], 1, 11, 7, AQUA);
        setPixelText(alphabet[letterSSelect3], 1, 17, 7, AQUA);
      }
      if(whichSLetterSelect==1){
        setPixelText(alphabet[letterSSelect1], 1, 5, 7, AQUA);
        setPixelText(alphabet[letterSSelect2], 1, 11, 7, WHITE);
        setPixelText(alphabet[letterSSelect3], 1, 17, 7, AQUA);
      }
      if(whichSLetterSelect==2){
        setPixelText(alphabet[letterSSelect1], 1, 5, 7, AQUA);
        setPixelText(alphabet[letterSSelect2], 1, 11, 7, AQUA);
        setPixelText(alphabet[letterSSelect3], 1, 17, 7, WHITE);
      }
      char* sc1 = alphabet[letterSSelect1];
      char* sc2 = alphabet[letterSSelect2];
      char* sc3 = alphabet[letterSSelect3];
      //char buf[4];
      strcpy(sbuf, sc1);
      strcpy(sbuf+strlen(sc1),sc2);
      strcpy(sbuf+strlen(sc1)+strlen(sc2),sc3);

      showPixels();
    }

    if (exitSnakeInitials){
      EEPROM.writeBlock<char>(addressCharArray2[highSnakeScoreNum], sbuf,3);

      fadeOut();
      curSnakeMenuSelection = 0;
      mainLoop();
      break;
    }
  }

}
