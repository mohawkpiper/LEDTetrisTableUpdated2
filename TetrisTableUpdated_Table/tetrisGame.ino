/* LedTable

   Written by: Klaas De Craemer, Ing. David Hrbaty - Modified to be like SNES classic by: Greg Beckman

   Main code for the Tetris game
*/

//////14 lines and below music regular speed 15 lines and above music fast

//frames per cell grid on 60fps
//0       48    800   1.0
//1       43    717   1.0143
//2       38    633   1.0286
//3       33    550   1.0429
//4       28    467   1.0572
//5       23    383   1.0715
//6       18    300   1.0858
//7       13    217   1.1
//8       8     133   1.1144
//9       6     100   1.1287
//10-12   5     83    1.143
//13-15   4     67    1.1573
//16-18   3     50    1.1716
//19-28   2     33    1.1859
//29+     1     17    1.2
uint16_t brickSpeedMap[15] = {800,727,651,574,494,410,326,239,148,113,96,80,61,41,23};
float brickSpeedMult = 1.06;//1.01//1.16;
uint16_t brickSpeed;

long nbRowsThisLevel;
long nbRowsTotal;
long nbRowsScore;

//bool musicSpeedFast;
uint8_t highestBrickInField = 0;

bool clearedInitialLines;
uint8_t curLevel;
uint8_t startLevel;
uint8_t selectedBrick;
uint8_t selectedGhostBrick;
uint8_t unselectedBrick;
uint8_t prevUnselectedBrick;
unsigned int long selectedCol;
unsigned int long selectedGhostCol;
unsigned int long unselectedCol;
uint8_t duplicateBrickNum = 0;
bool allowBrickMove = true;
int downCountForScore = 0;
int lastAddedBrickHeight = 0;

unsigned long downScoreCurTime;
unsigned long downScorePrevUpdateTime;

unsigned long prevUpdateTime = 0;
unsigned long curTime;
  

boolean tetrisGameOver;

void tetrisInit(uint8_t level) {
  //musicSpeedFast = false;
  Serial2.write('-');
  Serial2.flush();
  
  Serial3.write(uint8_t(10));// back to controller, says we are in tetris mode
  clearField();

  Serial2.write('P');
  Serial2.flush();
  
  clearedInitialLines=0;
  nbRowsThisLevel = 0;
  nbRowsTotal = 0;
  nbRowsScore = 0;
  startLevel = level;
  curLevel = startLevel;
  SetBrickSpeed();
  int initBrickSpeed = 1480;
  tetrisGameOver = false;
  uint8_t tmpStartBrick = random(8);
  if(tmpStartBrick==7){
    tmpStartBrick = random(7);
  }
  if(rngON==1&&tmpStartBrick==1){
    tmpStartBrick = random(7);
  }
  newActiveBrick(tmpStartBrick);
  setVisibleScore();
  nextInactiveBrick();
  setVisibleLines();
  setVisibleLevel();
  updateVisibleLevel();
  printField();
  curTime = millis();
  prevUpdateTime = curTime;
  while ((curTime - prevUpdateTime) < initBrickSpeed) {
    curTime = millis();
    readInput();
    if (curControl != BTN_NONE) {
      playerControlActiveBrick();
      printField();
      if(curControl == BTN_DOWN){
        curTime = millis();
        prevUpdateTime = curTime;
        break;
      }
    }
  }
  prevUpdateTime = curTime;
  
}

boolean tetrisRunning = false;

void setVisibleScore(){
  printSmallText((char*)"000000", 6, 5, -1, AQUA);
}
void setVisibleLines(){
  setSmallText((char*)"Line", 4, TETRIS_FIELD_WIDTH+3, 6, AQUA);
  setSmallText((char*)"000", 3, TETRIS_FIELD_WIDTH+7, 12, AQUA);
}
void setVisibleLevel(){
  setSmallText((char*)"Lv00", 4, TETRIS_FIELD_WIDTH+3, 19, AQUA);
}

void updateVisibleLines(){
  char bufVisibleLines[3];
  int lenVisibleLines = sprintf(bufVisibleLines, "%i", nbRowsTotal);
  if(lenVisibleLines==1){
    setSmallText(bufVisibleLines, lenVisibleLines, TETRIS_FIELD_WIDTH+15, 12, AQUA);
  }
  if(lenVisibleLines==2){
    setSmallText(bufVisibleLines, lenVisibleLines, TETRIS_FIELD_WIDTH+11, 12, AQUA);
  }
  if(lenVisibleLines==3){
    setSmallText(bufVisibleLines, lenVisibleLines, TETRIS_FIELD_WIDTH+7, 12, AQUA);
  }
}

void updateVisibleLevel(){
  char bufVisibleLevel[2];
  int lenVisibleLevel = sprintf(bufVisibleLevel, "%i", curLevel);
  if(lenVisibleLevel==1){
    setSmallText(bufVisibleLevel, lenVisibleLevel, TETRIS_FIELD_WIDTH+15, 19, AQUA);
  }
  if(lenVisibleLevel==2){
    setSmallText(bufVisibleLevel, lenVisibleLevel, TETRIS_FIELD_WIDTH+11, 19, AQUA);
  }
}

void runTetris(uint8_t whichLvl) {
  tetrisInit(whichLvl);
  tetrisRunning = true;
  while (tetrisRunning) {
    curTime = millis();
    downScoreCurTime = millis();
    do {
      readInput();
      //if(curControl == BTN_NONE){
        downScoreCurTime = millis();
        if(downScoreCurTime - downScorePrevUpdateTime > 45){
          downCountForScore = 0;//////////////////////////////////////////////////////////////////////////////////////////
        }
        
      //}
      if (curControl != BTN_NONE) {
        if(curControl==BTN_DOWN){
          downScorePrevUpdateTime = downScoreCurTime;
        }
        playerControlActiveBrick();
        printField();
      }
      if (tetrisGameOver) break;

      curTime = millis();
    } while ((curTime - prevUpdateTime) < brickSpeed);
    //Once enough time  has passed, proceed. The lower this number, the faster the game is
    prevUpdateTime = curTime;
    SetBrickSpeed();//
      
    if (tetrisGameOver) {
      Serial2.write('-');
      Serial2.flush();
      delay(15);
      
      Serial2.write('G');
      Serial2.flush();
      // pixel swap if invisible pixels so we can see what it looked like before we fade
      if(invisibleON==1){
        for (int xl = 0; xl < TETRIS_FIELD_WIDTH; xl++) {
          for (int yl = 0; yl < TETRIS_FIELD_HEIGHT; yl++) {
            if(field.pix[xl][yl] == 1){
              field.color[xl][yl] = field.color2[xl][yl];
            }
          }
        }
        printField();
        delay(2000);
      }
      fadeOut();
      char* GOBuf;
      GOBuf = (char*)"Game Over";
      int GOspeed = 30;
      scrollText(GOBuf, 9, 7, AQUA, GOspeed);
      
      //High score code here
      long CurrentHS[10];
      for(int i = 0; i <10; i++){
        CurrentHS[i] = EEPROM.readLong(addressLong[i]);
      }
      char CurrentHSnames[10][4] = {{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "}};
      Serial.print("your score = ");Serial.println(nbRowsScore);
      for(int i = 0; i <10; i++){
        EEPROM.readBlock<char>(addressCharArray[i],CurrentHSnames[i],3);
        Serial.print("hs name ");Serial.print(i);Serial.print(" = ");Serial.println(CurrentHSnames[i]);
        Serial.print("hs score ");Serial.print(i);Serial.print(" = ");Serial.println(CurrentHS[i]);
        //Serial.println("");
      }
      Serial.println("");

      if(nbRowsScore > CurrentHS[9]){
        Serial2.write('H');
        Serial2.flush();
        
        char* HSbuf;
        HSbuf = (char*)"NEW HIGH SCORE";
        scrollText(HSbuf, 14, 7, WHITE, GOspeed);

////// enter score?
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
              curTetrisMenuSelection = 0;
              mainLoop();
              break;
            }
            if(esON==1){
              break;
            }
            
          }
        }
//////////////////////////////////////

        int checkScorePosition = 0;
        while(nbRowsScore<CurrentHS[checkScorePosition]){
          checkScorePosition++;
        }

        EEPROM.writeLong(addressLong[checkScorePosition],nbRowsScore);
        delay(15);
        for(int i = checkScorePosition; i < 9; i ++){
              //Serial.print("score write pos ");Serial.println(i);
              //Serial.print("writing HS ");Serial.println(i+1);
          EEPROM.writeLong(addressLong[i+1],CurrentHS[i]);
          delay(15);
              //Serial.print("wrote HS ");Serial.println(i+1);
              //Serial.print("writing HS name ");Serial.println(i+1);
          EEPROM.writeBlock<char>(addressCharArray[i+1], CurrentHSnames[i],3);
          delay(15);
              //Serial.print("wrote HS name ");Serial.println(i+1);
          
        }
              //Serial.println("");
        runEnterInitials(checkScorePosition);
      
      }else{
        char buf[4];
        int lenScr = sprintf(buf, "%i", nbRowsScore);
        scrollText(buf, lenScr, 7, WHITE, 38);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Disable loop and exit to main menu of led table
        tetrisRunning = false;
        curTetrisMenuSelection = 0;
        break;
      }
    }

    //If brick is still "on the loose", then move it down by one
    if (!activeBrick.enabled) {
      //Active brick has "crashed", check for full lines
      //and create new brick at top of field
      checkFullLines();
      newActiveBrick(unselectedBrick);
      nextInactiveBrick();
      downCountForScore = 0;
      setARE();
      lastAddedBrickHeight = 0;
      prevUpdateTime = millis();//Reset update time to avoid brick dropping two spaces
    } else {
      shiftActiveBrick(DIR_DOWN,0,2);
    }
    printField();
  }
  fadeOut();
  curTetrisMenuSelection = 0;
  Serial2.write('-');
  Serial2.flush();
  mainLoop();

}

void setARE(){
  //pieces that lock in the bottom two rows are followed by 10 frames of entry delay, 
  //and each group of 4 rows above that has an entry delay 2 frames longer than the last.
  int extraFrames = 0;
  if(lastAddedBrickHeight<19){
    extraFrames = (floor((20-lastAddedBrickHeight-2)/4)*33)+33;
  }
  extraFrames+=167;
  //10frames=167 millis
  //2frames=33 millis
  unsigned long prevARETime = millis();
  unsigned long curARETime = millis();
  while(curARETime-prevARETime<extraFrames){
    curARETime=millis();
    readBlankInput();
  }
}

void pauseTetris(){
  Serial3.write(uint8_t(5));// back to controller, says we are in menu mode
  
  Serial2.write('S');
  Serial2.flush();
  
  int whichPauseSelect = 0;
  clearTablePixels();
  setSmallText((char*)"RESUME", 6, 2, 6, WHITE);
  setSmallText((char*)"QUIT", 4, 6, 14, AQUA);
  showPixels();
  Serial2.write('p');
  Serial2.flush();
  while(true){
    clearTablePixels();
    
    boolean pauseSelectionChanged = false;
    boolean quitGame = false;
    boolean resumeGame = false;
    
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_UP){
        whichPauseSelect++;
        if(whichPauseSelect>1){
          whichPauseSelect=0;
        }
        pauseSelectionChanged = true;
      } else if (curControl == BTN_SELECT || curControl == BTN_A){
        if(whichPauseSelect == 0){
          resumeGame = true;
        } else {
          quitGame = true;
        }
      }
    }
    if (pauseSelectionChanged){
      if(whichPauseSelect==0){
        setSmallText((char*)"RESUME", 6, 2, 6, WHITE);
        setSmallText((char*)"QUIT", 4, 6, 14, AQUA);
      } else {
        setSmallText((char*)"RESUME", 6, 2, 6, AQUA);
        setSmallText((char*)"QUIT", 4, 6, 14, WHITE);
      }
      showPixels();
    }
    if(resumeGame){
      Serial3.write(uint8_t(10));// back to controller, says we are in tetris mode
      
      //start back into tetris
      Serial2.write('P');
      Serial2.flush();
      clearTablePixels();
      setVisibleScore();
      updateVisibleScore();
      setVisibleLines();
      updateVisibleLines();
      setVisibleLevel();
      updateVisibleLevel();
      showInactiveBrick();
      break;
    }
    if(quitGame){
      Serial2.write('S');
      Serial2.flush();
      clearTablePixels();
      setVisibleScore();
      updateVisibleScore();
      setVisibleLines();
      updateVisibleLines();
      setVisibleLevel();
      updateVisibleLevel();
      showInactiveBrick();
      tetrisRunning = false;
      curTetrisMenuSelection = 0;
      break;
    }
    
  }
}

void runEnterInitials(int highScoreNum){
  Serial3.write(uint8_t(5));// back to controller, says we are in menu mode
  clearTablePixels();
  char bufYourScore[6];
  int lenYourScore = sprintf(bufYourScore, "%i", nbRowsScore);
  int scoreOffset = (6 - lenYourScore)*4;
  int whichLetterSelect = 0;
  int letterSelect1 = 0;
  int letterSelect2 = 0;
  int letterSelect3 = 0;
  char buf[4];
      
    
  
  setSmallText(bufYourScore, lenYourScore, scoreOffset+2, -1, AQUA);
  
  setPixelText(alphabet[letterSelect1], 1, 5, 9, WHITE);
  setPixelText(alphabet[letterSelect2], 1, 11, 9, AQUA);
  setPixelText(alphabet[letterSelect3], 1, 17, 9, AQUA);

  showPixels();

  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText(bufYourScore, lenYourScore, scoreOffset+2, -1, AQUA);

    boolean initialsSelectionChanged = false;
    boolean exitInitials = false;

    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        if(whichLetterSelect == 0){
          letterSelect1--;
          if(letterSelect1<0){
            letterSelect1=40;
          }
        }
        if(whichLetterSelect == 1){
          letterSelect2--;
          if(letterSelect2<0){
            letterSelect2=40;
          }
        }
        if(whichLetterSelect == 2){
          letterSelect3--;
          if(letterSelect3<0){
            letterSelect3=40;
          }
        }
        initialsSelectionChanged = true;
      } else if (curControl == BTN_UP){
        if(whichLetterSelect == 0){
          letterSelect1++;
          if(letterSelect1>=41){
            letterSelect1=0;
          }
        }
        if(whichLetterSelect == 1){
          letterSelect2++;
          if(letterSelect2>=41){
            letterSelect2=0;
          }
        }
        if(whichLetterSelect == 2){
          letterSelect3++;
          if(letterSelect3>=41){
            letterSelect3=0;
          }
        }
        initialsSelectionChanged = true;
      } else if (curControl == BTN_LEFT){
        whichLetterSelect--;
        if(whichLetterSelect<0){
          whichLetterSelect=0;
        }
        initialsSelectionChanged = true;
      } else if (curControl == BTN_RIGHT){
        whichLetterSelect++;
        if(whichLetterSelect>=3){
          whichLetterSelect=2;
        }
        initialsSelectionChanged = true;
      } else if (curControl == BTN_START){
        exitInitials = true;
      } else if (curControl == BTN_SELECT){
        exitInitials = true;
      } else if (curControl == BTN_A){
        if(whichLetterSelect<2){
          whichLetterSelect++;
          initialsSelectionChanged = true;
        }else if (whichLetterSelect==2){
          exitInitials = true;
        }
      } else if (curControl == BTN_B){
        whichLetterSelect--;
        if(whichLetterSelect<0){
          whichLetterSelect=0;
        }
        initialsSelectionChanged = true;
      }
    }

  
    if (initialsSelectionChanged){
      if(whichLetterSelect==0){
        setPixelText(alphabet[letterSelect1], 1, 5, 9, WHITE);
        setPixelText(alphabet[letterSelect2], 1, 11, 9, AQUA);
        setPixelText(alphabet[letterSelect3], 1, 17, 9, AQUA);
      }
      if(whichLetterSelect==1){
        setPixelText(alphabet[letterSelect1], 1, 5, 9, AQUA);
        setPixelText(alphabet[letterSelect2], 1, 11, 9, WHITE);
        setPixelText(alphabet[letterSelect3], 1, 17, 9, AQUA);
      }
      if(whichLetterSelect==2){
        setPixelText(alphabet[letterSelect1], 1, 5, 9, AQUA);
        setPixelText(alphabet[letterSelect2], 1, 11, 9, AQUA);
        setPixelText(alphabet[letterSelect3], 1, 17, 9, WHITE);
      }
      char* c1 = alphabet[letterSelect1];
      char* c2 = alphabet[letterSelect2];
      char* c3 = alphabet[letterSelect3];
      strcpy(buf, c1);
      strcpy(buf+strlen(c1),c2);
      strcpy(buf+strlen(c1)+strlen(c2),c3);

      showPixels();
    }

    if (exitInitials){
            //Serial.print("writing HS ");Serial.println(highScoreNum);
      EEPROM.writeBlock<char>(addressCharArray[highScoreNum],buf,3);
            //Serial.print("wrote HS ");Serial.println(highScoreNum);
            //Serial.println("");
      delay(15);
      fadeOut();
      curTetrisMenuSelection = 0;
      mainLoop();
      break;
    }
  }

}

void playerControlActiveBrick() {
  if(allowBrickMove){
    switch (curControl) {
      case BTN_A:
        if(selectedBrick==1||selectedBrick==5||selectedBrick==6){
          if(activeBrick.isFlat==true){
            rotateActiveBrickCC();
          }else{
            rotateActiveBrick();
          }
        }else{
          rotateActiveBrick();
        }
        //rotateActiveBrick();
        break;
      case BTN_A_AND_LEFT:
        if(selectedBrick==1||selectedBrick==5||selectedBrick==6){
          if(activeBrick.isFlat==true){
            rotateActiveBrickCC();
          }else{
            rotateActiveBrick();
          }
        }else{
          rotateActiveBrick();
        }
        shiftActiveBrick(DIR_LEFT,0,0);
        break;
      case BTN_A_AND_DOWN:
        if(selectedBrick==1||selectedBrick==5||selectedBrick==6){
          if(activeBrick.isFlat==true){
            rotateActiveBrickCC();
          }else{
            rotateActiveBrick();
          }
        }else{
          rotateActiveBrick();
        }
        shiftActiveBrick(DIR_DOWN,0,1);
        break;
      case BTN_A_AND_RIGHT:
        if(selectedBrick==1||selectedBrick==5||selectedBrick==6){
          if(activeBrick.isFlat==true){
            rotateActiveBrickCC();
          }else{
            rotateActiveBrick();
          }
        }else{
          rotateActiveBrick();
        }
        shiftActiveBrick(DIR_RIGHT,0,0);
        break;
      case BTN_B_AND_LEFT:
        if(selectedBrick==1||selectedBrick==5||selectedBrick==6){
          if(activeBrick.isFlat==true){
            rotateActiveBrickCC();
          }else{
            rotateActiveBrick();
          }
        }else{
          rotateActiveBrickCC();
        }
        shiftActiveBrick(DIR_LEFT,0,0);
        break;
      case BTN_B_AND_DOWN:
        if(selectedBrick==1||selectedBrick==5||selectedBrick==6){
          if(activeBrick.isFlat==true){
            rotateActiveBrickCC();
          }else{
            rotateActiveBrick();
          }
        }else{
          rotateActiveBrickCC();
        }
        shiftActiveBrick(DIR_DOWN,0,1);
        break;
      case BTN_B_AND_RIGHT:
        if(selectedBrick==1||selectedBrick==5||selectedBrick==6){
          if(activeBrick.isFlat==true){
            rotateActiveBrickCC();
          }else{
            rotateActiveBrick();
          }
        }else{
          rotateActiveBrickCC();
        }
        shiftActiveBrick(DIR_RIGHT,0,0);
        break;
      case BTN_B:
        if(selectedBrick==1||selectedBrick==5||selectedBrick==6){
          if(activeBrick.isFlat==true){
            rotateActiveBrickCC();
          }else{
            rotateActiveBrick();
          }
        }else{
          rotateActiveBrickCC();
        }
        break;
      case BTN_LEFT:
        shiftActiveBrick(DIR_LEFT,1,0);
        break;
      case BTN_RIGHT:
        shiftActiveBrick(DIR_RIGHT,1,0);
        break;
      case BTN_DOWN:
        shiftActiveBrick(DIR_DOWN,0,1);
        break;
      case BTN_UP:
        for(int f=0;f<20;f++){
          shiftActiveBrick(DIR_DOWN,0,1);
        }
        break;
      case BTN_START:
        pauseTetris();
    }
  }
}

void playerControlIdleBrick() {// to get rid of bluetooth commands being sent while stuff is happening and movement shouldnt happen
  if(allowBrickMove){
    switch (curControl) {
      case BTN_A:
        break;
      case BTN_A_AND_LEFT:
        break;
      case BTN_A_AND_DOWN:
        break;
      case BTN_A_AND_RIGHT:
        break;
      case BTN_B_AND_LEFT:
        break;
      case BTN_B_AND_DOWN:
        break;
      case BTN_B_AND_RIGHT:
        break;
      case BTN_B:
        break;
      case BTN_LEFT:
        break;
      case BTN_RIGHT:
        break;
      case BTN_DOWN:
        break;
      case BTN_UP:
        break;
      case BTN_START:
        pauseTetris();
    }
  }
}

void printField() {
  int x, y;
  for (x = 0; x < TETRIS_FIELD_WIDTH; x++) {
    for (y = 0; y < TETRIS_FIELD_HEIGHT; y++) {
      uint8_t activeBrickPix = 0;
      uint8_t ghostBrickPix = 0;
      if (activeBrick.enabled) { //Only draw brick if it is enabled
        //Now check if brick is "in view"
        if ((x >= activeBrick.xpos) && (x < (activeBrick.xpos + (activeBrick.siz)))
            && (y >= activeBrick.ypos) && (y < (activeBrick.ypos + (activeBrick.siz)))) {
          activeBrickPix = (activeBrick.pix)[x - activeBrick.xpos][y - activeBrick.ypos];
        }

        //// this would get turned on only if ghost brick is set to ON in the menu
        if(ghostON==1){  
          uint8_t ghostBrickHit = 1;
          uint8_t ghostBrickNumSpaces = 0;
          while(ghostBrickHit){
            uint8_t bx, by;
            uint8_t fx, fy;
            for (by = 0; by < activeBrick.siz; by++) {
              for (bx = 0; bx < activeBrick.siz; bx++) {
                fx = activeBrick.xpos + bx;
                fy = activeBrick.ypos+ ghostBrickNumSpaces + by;
                if (( activeBrick.pix[bx][by] == 1)
                    && ( field.pix[fx][fy] == 1)) {
                  ghostBrickNumSpaces-1;
                  ghostBrickHit = 0;
                }
              }
            }
            ghostBrickNumSpaces++;
          }
          if ((x >= activeBrick.xpos) && (x < (activeBrick.xpos + (activeBrick.siz)))
              && (y >= activeBrick.ypos+ghostBrickNumSpaces-2) && (y < (activeBrick.ypos + (activeBrick.siz)+ghostBrickNumSpaces-2))) {
            ghostBrickPix = (activeBrick.pix)[x - activeBrick.xpos][y - activeBrick.ypos-ghostBrickNumSpaces+2];
          }
        }//// to here for menu ghost brick
      }

      if (activeBrickPix == 1) {
        setTablePixelRGBW(x+TETRIS_FIELD_XOFF, y+TETRIS_FIELD_YOFF, activeBrick.col);
      } else if (field.pix[x][y] == 1) {
        setTablePixelRGBW(x+TETRIS_FIELD_XOFF, y+TETRIS_FIELD_YOFF, field.color[x][y]);
      }  else if (ghostBrickPix == 1) {
        setTablePixelRGBW(x+TETRIS_FIELD_XOFF, y+TETRIS_FIELD_YOFF, LTGREY);
      } else {
        setTablePixelRGBW(x+TETRIS_FIELD_XOFF, y+TETRIS_FIELD_YOFF, 0x000000);
      }
    }
  }
  for(y=6;y<26;y++){
    setTablePixelRGBW(0,y,FRAMECOL);
    setTablePixelRGBW(11,y,FRAMECOL);
  }
  for(x=0;x<12;x++){
    setTablePixelRGBW(x,5,FRAMECOL);
  }
  showPixels();
}

void showInactiveBrick() {
  for (int ly=0; ly<MAX_BRICK_SIZE; ly++){ 
    for (int lx=0; lx<MAX_BRICK_SIZE; lx++){
      setTablePixelRGBW(lx,ly+1,(brickLib[unselectedBrick]).pix[ly][lx]*unselectedCol); 
    }   
  } 
}
/* *** Game functions *** */
void nextInactiveBrick() {
  prevUnselectedBrick = unselectedBrick;
  //unselectedBrick = random(8);
  //if(unselectedBrick==prevUnselectedBrick||unselectedBrick==7){
  //  unselectedBrick = random(7);
  //}
  unselectedBrick = random(9);
  if(unselectedBrick>6){
    unselectedBrick = random(7);
  }
  if(rngON==1&&unselectedBrick==1){
    unselectedBrick = random(7);
  }

  unselectedCol = colorLibLvl[curLevel%10][brickLib[unselectedBrick].colorIdentifier];
  //Set properties of brick
  nextBrick.siz = brickLib[unselectedBrick].siz;
  nextBrick.yOffset = brickLib[unselectedBrick].yOffset;
  nextBrick.xpos = TETRIS_FIELD_WIDTH + 2 - nextBrick.siz / 2;
  nextBrick.ypos = BRICKOFFSET - nextBrick.yOffset - 1;
  //inactiveBrick.enabled = false;

  nextBrick.col = unselectedCol;
  // print in next brick field
  for (int ly=0; ly<MAX_BRICK_SIZE; ly++){ 
    for (int lx=0; lx<MAX_BRICK_SIZE; lx++){
      setTablePixelRGBW(lx,ly+1,(brickLib[unselectedBrick]).pix[ly][lx]*unselectedCol); 
    }   
  } 
}

void newActiveBrick(uint8_t bricknum) {
  //randomSeed(analogRead(5) / millis());
  selectedBrick = bricknum;
  selectedCol = colorLibLvl[curLevel%10][brickLib[selectedBrick].colorIdentifier];
  //Set properties of brick
  activeBrick.siz = brickLib[selectedBrick].siz;
  activeBrick.yOffset = brickLib[selectedBrick].yOffset;
  activeBrick.xpos = TETRIS_FIELD_WIDTH / 2 - activeBrick.siz / 2;
  activeBrick.ypos = BRICKOFFSET - activeBrick.yOffset;
  activeBrick.enabled = true;
  activeBrick.isFlat = true;

  //Set color of brick
  activeBrick.col = selectedCol;

  //Copy pix array of selected Brick
  uint8_t x, y;
  for (y = 0; y < MAX_BRICK_SIZE; y++) {
    for (x = 0; x < MAX_BRICK_SIZE; x++) {
      activeBrick.pix[x][y] = (brickLib[selectedBrick]).pix[y][x];
    }
  }

  //Check collision, if already, then game is over
  if (checkFieldCollision(&activeBrick)) {
    tetrisGameOver = true;
  }
}


//Check collision between bricks in the field and the specified brick
boolean checkFieldCollision(struct Brick* brick) {
  uint8_t bx, by;
  uint8_t fx, fy;
  for (by = 0; by < MAX_BRICK_SIZE; by++) {
    for (bx = 0; bx < MAX_BRICK_SIZE; bx++) {
      fx = (*brick).xpos + bx;
      fy = (*brick).ypos + by;
      if (( (*brick).pix[bx][by] == 1)
          && ( field.pix[fx][fy] == 1)) {
        return true;
      }
    }
  }
  return false;
}

//Check collision between specified brick and all sides of the playing field
boolean checkSidesCollision(struct Brick* brick) {
  //Check vertical collision with sides of field
  uint8_t bx, by;
  uint8_t fx;//, fy;
  for (by = 0; by < MAX_BRICK_SIZE; by++) {
    for (bx = 0; bx < MAX_BRICK_SIZE; bx++) {
      if ( (*brick).pix[bx][by] == 1) {
        fx = (*brick).xpos + bx;//Determine actual position in the field of the current pix of the brick
        //fy = (*brick).ypos + by;
        if (fx < 0 || fx >= TETRIS_FIELD_WIDTH) {
          return true;
        }
      }
    }
  }
  return false;
}

Brick tmpBrick;

void rotateActiveBrick() {
  Serial2.write('r');
  Serial2.flush();

  activeBrick.isFlat = !activeBrick.isFlat;
  
  //Copy active brick pix array to temporary pix array
  uint8_t x, y;
  for (y = 0; y < MAX_BRICK_SIZE; y++) {
    for (x = 0; x < MAX_BRICK_SIZE; x++) {
      tmpBrick.pix[x][y] = activeBrick.pix[x][y];
    }
  }
  tmpBrick.xpos = activeBrick.xpos;
  tmpBrick.ypos = activeBrick.ypos;
  tmpBrick.siz = activeBrick.siz;

  //Depending on size of the active brick, we will rotate differently
  if (activeBrick.siz == 3) {
    //Perform rotation around center pix
    tmpBrick.pix[0][0] = activeBrick.pix[0][2];
    tmpBrick.pix[0][1] = activeBrick.pix[1][2];
    tmpBrick.pix[0][2] = activeBrick.pix[2][2];
    tmpBrick.pix[1][0] = activeBrick.pix[0][1];
    tmpBrick.pix[1][1] = activeBrick.pix[1][1];
    tmpBrick.pix[1][2] = activeBrick.pix[2][1];
    tmpBrick.pix[2][0] = activeBrick.pix[0][0];
    tmpBrick.pix[2][1] = activeBrick.pix[1][0];
    tmpBrick.pix[2][2] = activeBrick.pix[2][0];
    //Keep other parts of temporary block clear
    tmpBrick.pix[0][3] = 0;
    tmpBrick.pix[1][3] = 0;
    tmpBrick.pix[2][3] = 0;
    tmpBrick.pix[3][3] = 0;
    tmpBrick.pix[3][2] = 0;
    tmpBrick.pix[3][1] = 0;
    tmpBrick.pix[3][0] = 0;

  } else if (activeBrick.siz == 4) {
    //Perform rotation around center "cross"
    tmpBrick.pix[0][0] = activeBrick.pix[0][3];
    tmpBrick.pix[0][1] = activeBrick.pix[1][3];
    tmpBrick.pix[0][2] = activeBrick.pix[2][3];
    tmpBrick.pix[0][3] = activeBrick.pix[3][3];
    tmpBrick.pix[1][0] = activeBrick.pix[0][2];
    tmpBrick.pix[1][1] = activeBrick.pix[1][2];
    tmpBrick.pix[1][2] = activeBrick.pix[2][2];
    tmpBrick.pix[1][3] = activeBrick.pix[3][2];
    tmpBrick.pix[2][0] = activeBrick.pix[0][1];
    tmpBrick.pix[2][1] = activeBrick.pix[1][1];
    tmpBrick.pix[2][2] = activeBrick.pix[2][1];
    tmpBrick.pix[2][3] = activeBrick.pix[3][1];
    tmpBrick.pix[3][0] = activeBrick.pix[0][0];
    tmpBrick.pix[3][1] = activeBrick.pix[1][0];
    tmpBrick.pix[3][2] = activeBrick.pix[2][0];
    tmpBrick.pix[3][3] = activeBrick.pix[3][0];
  } else {
    //Serial.println("Brick size error");
  }

  //Now validate by checking collision.
  //Collision possibilities:
  //      -Brick now sticks outside field
  //      -Brick now sticks inside fixed bricks of field
  //In case of collision, we just discard the rotated temporary brick
  if ((!checkSidesCollision(&tmpBrick)) && (!checkFieldCollision(&tmpBrick))) {
    //Copy temporary brick pix array to active pix array
    for (y = 0; y < MAX_BRICK_SIZE; y++) {
      for (x = 0; x < MAX_BRICK_SIZE; x++) {
        activeBrick.pix[x][y] = tmpBrick.pix[x][y];
      }
    }
  }
}

void rotateActiveBrickCC() {
  Serial2.write('r');
  Serial2.flush();
  
  activeBrick.isFlat = !activeBrick.isFlat;
  
  //Copy active brick pix array to temporary pix array
  uint8_t x, y;
  for (y = 0; y < MAX_BRICK_SIZE; y++) {
    for (x = 0; x < MAX_BRICK_SIZE; x++) {
      tmpBrick.pix[x][y] = activeBrick.pix[x][y];
    }
  }
  tmpBrick.xpos = activeBrick.xpos;
  tmpBrick.ypos = activeBrick.ypos;
  tmpBrick.siz = activeBrick.siz;

  //Depending on size of the active brick, we will rotate differently
  if (activeBrick.siz == 3) {
    //Perform rotation around center pix
    tmpBrick.pix[0][0] = activeBrick.pix[2][0];
    tmpBrick.pix[0][1] = activeBrick.pix[1][0];
    tmpBrick.pix[0][2] = activeBrick.pix[0][0];
    tmpBrick.pix[1][0] = activeBrick.pix[2][1];
    tmpBrick.pix[1][1] = activeBrick.pix[1][1];
    tmpBrick.pix[1][2] = activeBrick.pix[0][1];
    tmpBrick.pix[2][0] = activeBrick.pix[2][2];
    tmpBrick.pix[2][1] = activeBrick.pix[1][2];
    tmpBrick.pix[2][2] = activeBrick.pix[0][2];
    //Keep other parts of temporary block clear
    tmpBrick.pix[0][3] = 0;
    tmpBrick.pix[1][3] = 0;
    tmpBrick.pix[2][3] = 0;
    tmpBrick.pix[3][3] = 0;
    tmpBrick.pix[3][2] = 0;
    tmpBrick.pix[3][1] = 0;
    tmpBrick.pix[3][0] = 0;

  } else if (activeBrick.siz == 4) {
    //Perform rotation around center "cross"
    tmpBrick.pix[0][0] = activeBrick.pix[3][0];
    tmpBrick.pix[0][1] = activeBrick.pix[2][0];
    tmpBrick.pix[0][2] = activeBrick.pix[1][0];
    tmpBrick.pix[0][3] = activeBrick.pix[0][0];
    tmpBrick.pix[1][0] = activeBrick.pix[3][1];
    tmpBrick.pix[1][1] = activeBrick.pix[2][1];
    tmpBrick.pix[1][2] = activeBrick.pix[1][1];
    tmpBrick.pix[1][3] = activeBrick.pix[0][1];
    tmpBrick.pix[2][0] = activeBrick.pix[3][2];
    tmpBrick.pix[2][1] = activeBrick.pix[2][2];
    tmpBrick.pix[2][2] = activeBrick.pix[1][2];
    tmpBrick.pix[2][3] = activeBrick.pix[0][2];
    tmpBrick.pix[3][0] = activeBrick.pix[3][3];
    tmpBrick.pix[3][1] = activeBrick.pix[2][3];
    tmpBrick.pix[3][2] = activeBrick.pix[1][3];
    tmpBrick.pix[3][3] = activeBrick.pix[0][3];
  } else {
    //Serial.println("Brick size error");
  }

  //Now validate by checking collision.
  //Collision possibilities:
  //      -Brick now sticks outside field
  //      -Brick now sticks inside fixed bricks of field
  //In case of collision, we just discard the rotated temporary brick
  if ((!checkSidesCollision(&tmpBrick)) && (!checkFieldCollision(&tmpBrick))) {
    //Copy temporary brick pix array to active pix array
    for (y = 0; y < MAX_BRICK_SIZE; y++) {
      for (x = 0; x < MAX_BRICK_SIZE; x++) {
        activeBrick.pix[x][y] = tmpBrick.pix[x][y];
      }
    }
  }
}

//Shift brick left/right/down by one if possible
void shiftActiveBrick(int dir, int playSound, int holdForScore) {

  //Change position of active brick (no copy to temporary needed)
  if (dir == DIR_LEFT) {
    activeBrick.xpos--;
  } else if (dir == DIR_RIGHT) {
    activeBrick.xpos++;
  } else if (dir == DIR_DOWN) {
    activeBrick.ypos++;
  } else if (dir == DIR_UP) {

  }

  //Check position of active brick
  //Two possibilities when collision is detected:
  //    -Direction was LEFT/RIGHT, just revert position back
  //    -Direction was DOWN, revert position and fix block to field on collision
  //When no collision, keep activeBrick coordinates
  
  
  if ((checkSidesCollision(&activeBrick)) || (checkFieldCollision(&activeBrick))) {
    //Serial.println("coll");
    if (dir == DIR_LEFT) {
      activeBrick.xpos++;
      playSound=0;
    } else if (dir == DIR_RIGHT) {
      activeBrick.xpos--;
      playSound=0;
    } else if (dir == DIR_DOWN) {
      downCountForScore--;
      activeBrick.ypos--;//Go back up one
      playSound=0;
      addActiveBrickToField();
      activeBrick.enabled = false;//Disable brick, it is no longer moving
      brickSpeed = 120;
      //prevUpdateTime = millis();
      
      if(holdForScore==1){
        Serial3.write(uint8_t(15));// back to controller, says a brick landed. reset movement control
      }
      
      
      Serial2.write('h');
      Serial2.flush();
    }
  }
  if(holdForScore==1&&activeBrick.enabled == true){
    prevUpdateTime = millis();
    downCountForScore++;
  }
  if(holdForScore==2&&activeBrick.enabled == true){
    downCountForScore = 0;
  }
  if(playSound==1){
    Serial2.write('r');
    Serial2.flush();
  }
  
  
}

//Copy active pixels to field, including color
void addActiveBrickToField() {
  if(downCountForScore>0){
    if(downCountForScore>=10){
      downCountForScore = max(10,downCountForScore-4);
    }
    nbRowsScore += (downCountForScore);
    updateVisibleScore();
  }
  uint8_t bx, by;
  uint8_t fx, fy;
      
  for (by = 0; by < MAX_BRICK_SIZE; by++) {
    fy = activeBrick.ypos + by;
    //Serial.print("brickpart y poses = ");Serial.println(fy);
    for (bx = 0; bx < MAX_BRICK_SIZE; bx++) {
      fx = activeBrick.xpos + bx;
      if (fx >= 0 && fy >= 0 && fx < TETRIS_FIELD_WIDTH && fy < TETRIS_FIELD_HEIGHT && activeBrick.pix[bx][by]) { //Check if inside playing field
        field.pix[fx][fy] = activeBrick.pix[bx][by];
        if(invisibleON==1){
          field.color[fx][fy] = BLACK;
          field.color2[fx][fy] = activeBrick.col;
        }else{
          field.color[fx][fy] = activeBrick.col;
          field.color2[fx][fy] = BLACK;
        }
        //curBrickHeight = max(lastAddedBrickHeight,by);
        lastAddedBrickHeight = max(lastAddedBrickHeight,by);
      }
    }
  }
  lastAddedBrickHeight = lastAddedBrickHeight+activeBrick.ypos+1;
  checkHighestBrickInField();
}

void checkHighestBrickInField(){// this check is for determining if we should speed up or slow down audio
  uint8_t hbfx, hbfy;
  highestBrickInField = 0;
  for (hbfx=0; hbfx < TETRIS_FIELD_WIDTH; hbfx++){
    for (hbfy=0; hbfy < TETRIS_FIELD_HEIGHT; hbfy++){
      if(field.pix[hbfx][hbfy]==1){
        highestBrickInField = max(highestBrickInField,20-hbfy);
      }
    }
  }
  if(highestBrickInField<15){
    Serial2.write('-');
    Serial2.flush();
  }else{
    Serial2.write('+');
    Serial2.flush();
  }
  
}

//Move all pix from te field above startRow down by one. startRow is overwritten
void moveFieldDownOne(uint8_t startRow) {
  if (startRow == 0) { //Topmost row has nothing on top to move...
    return;
  }
  uint8_t x, y;
  for (y = startRow - 1; y > 0; y--) {
    for (x = 0; x < TETRIS_FIELD_WIDTH; x++) {
      field.pix[x][y + 1] = field.pix[x][y];
      field.color[x][y + 1] = field.color[x][y];
      field.color2[x][y + 1] = field.color2[x][y];
      if (y == 1) {
        field.pix[x][1] = field.pix[x][0];;
        field.color[x][1] = field.color[x][0];
        field.color2[x][1] = field.color2[x][0];
        field.pix[x][0] = 0;
      }
    }
  }
}

void SetBrickSpeed(){
  if(curLevel<=10){
    brickSpeed = floor(brickSpeedMult*brickSpeedMap[curLevel]);
  }else if(curLevel<13){
    brickSpeed = floor(brickSpeedMult*brickSpeedMap[10]);
  }else if(curLevel<16){
    brickSpeed = floor(brickSpeedMult*brickSpeedMap[11]);
  }else if(curLevel<19){
    brickSpeed = floor(brickSpeedMult*brickSpeedMap[12]);
  }else if(curLevel<29){
    brickSpeed = floor(brickSpeedMult*brickSpeedMap[13]);
  }else{
    brickSpeed = floor(brickSpeedMult*brickSpeedMap[14]);
  }
 // brickSpeed = floor(brickSpeed*brickSpeedMult);

}

void LevelUp() {
  if(clearedInitialLines==0){
    if (nbRowsThisLevel >= (unsigned)(startLevel*10+10) || nbRowsThisLevel >= (unsigned)max(100,(startLevel*10-50))) {
      unsigned int long switchColor;
      unsigned int long switchColor2;
      int x, y, z;
      for (y = (TETRIS_FIELD_HEIGHT - 1); y >= 0; y--) {
        for (x = 0; x < TETRIS_FIELD_WIDTH; x++) {
          if(field.pix[x][y]){
            switchColor = field.color[x][y];//getPixel(x);
            switchColor2 = field.color2[x][y];
            for(z=0;z<3;z++){
              if(switchColor==colorLibLvl[curLevel%10][z]){
                field.color[x][y]=colorLibLvl[(curLevel+1)%10][z];
              }
              if(switchColor2==colorLibLvl[curLevel%10][z]){
                field.color2[x][y]=colorLibLvl[(curLevel+1)%10][z];
              }
            }
          }
        }
      }
      printField();
      curLevel++;
      updateVisibleLevel();
      Serial2.write('c');
      Serial2.flush();
      SetBrickSpeed();
      float ridRows = float(nbRowsThisLevel/10);
      nbRowsThisLevel = nbRowsThisLevel - (ridRows*10);
      clearedInitialLines=1;
    }
  }
  if(clearedInitialLines!=0){
    if(nbRowsThisLevel>=10){
      unsigned int long switchColor;
      unsigned int long switchColor2;
      int x, y, z;
      for (y = (TETRIS_FIELD_HEIGHT - 1); y >= 0; y--) {
        for (x = 0; x < TETRIS_FIELD_WIDTH; x++) {
          if(field.pix[x][y]){
            switchColor = field.color[x][y];//getPixel(x);
            switchColor2 = field.color2[x][y];
            for(z=0;z<3;z++){
              if(switchColor==colorLibLvl[curLevel%10][z]){
                field.color[x][y]=colorLibLvl[(curLevel+1)%10][z]; 
              }
              if(switchColor2==colorLibLvl[curLevel%10][z]){
                field.color2[x][y]=colorLibLvl[(curLevel+1)%10][z]; 
              }
            }
          }
        }
      }
      printField();
      nbRowsThisLevel -= 10;
      curLevel++;
      updateVisibleLevel();
      Serial2.write('c');
      Serial2.flush();
      SetBrickSpeed();
    }
  }
}

long previousLineMillis = 0; 
long lineInterval = 20;
long lineDownInterval = 50;
long previousFlashMillis = 0;
long flashInterval = 23;
unsigned long currentLineMillis;

void checkFullLines() {
  int x, y, centerX;
  int minY = 0;
  int totalRows = 0;
  int whichRows[TETRIS_FIELD_HEIGHT] = {0};//-1] = {0};
  for (y = (TETRIS_FIELD_HEIGHT - 1); y >= minY; y--) {
    uint8_t rowSum = 0;
    for (x = 0; x < TETRIS_FIELD_WIDTH; x++) {
      rowSum = rowSum + (field.pix[x][y]);
    }
    if (rowSum >= TETRIS_FIELD_WIDTH) {
      //found full row, count up
      whichRows[y]=1;
      totalRows++;
    }
  }

  //IF found full row(s), animate then removal
  if(totalRows>0){
    if(totalRows>=4){
      Serial2.write('t');
      Serial2.flush();
    }else{
      Serial2.write('l');
      Serial2.flush();
    }
    //remove
    
    if(invisibleON==1){
      int invX = 0;
      while(invX<TETRIS_FIELD_WIDTH/2*3){
        for (int invY = (TETRIS_FIELD_HEIGHT - 1); invY >= minY; invY--) {
          if(whichRows[invY]==1){
            field.color[invX][invY] = field.color2[invX][invY];
          }
        }
        invX++;
      }
      printField();
    }

     x = 0;
     int flashx = 0;
     while(x<TETRIS_FIELD_WIDTH/2*3){
      currentLineMillis = millis();
      if(currentLineMillis - previousLineMillis > lineInterval) {
        centerX = floor(x/3)+TETRIS_FIELD_WIDTH/2;
        for (y = (TETRIS_FIELD_HEIGHT - 1); y >= minY; y--) {
          if(whichRows[y]==1){
            field.pix[centerX][y] = 0;
            field.pix[TETRIS_FIELD_WIDTH-centerX-1][y] = 0;
          }
        }
  
        //blink the background if you get a tetris
        if(totalRows>=4&&flashx<8&&currentLineMillis - previousFlashMillis > flashInterval){
          for (int xl = 0; xl < TETRIS_FIELD_WIDTH; xl++) {
            for (int yl = 0; yl < TETRIS_FIELD_HEIGHT; yl++) {
              if(field.pix[xl][yl] == 0){
                field.pix[xl][yl] = 1;
                field.color[xl][yl] = CHOCOLATE;// chocolate is ONLY USED for bg blinking so this works.
              }
              if(x%2&&field.color[xl][yl]==CHOCOLATE){
                field.pix[xl][yl] = 0;
                field.color[xl][yl] = 0;
              }
            }
          }
          flashx++;
          previousFlashMillis = currentLineMillis;
        }
  
        printField();
        x++;
        previousLineMillis = currentLineMillis; 
        readInput();
        if (curControl != BTN_NONE) {
        playerControlIdleBrick();
      }
  
      }
    }
    
    //Move all upper rows down by one
    for (y = 0; y < TETRIS_FIELD_HEIGHT; y++) {
      if(whichRows[y]==1){
        Serial2.write('d');
        Serial2.flush();
        moveFieldDownOne(y);
        printField();
      }
    }
    x=0;
    while(x<1){
      currentLineMillis = millis();
      if(currentLineMillis - previousLineMillis > lineDownInterval) {
        x++;
        previousLineMillis = currentLineMillis;
      }
    //delay(50);
    }

    nbRowsThisLevel+=totalRows; 
    nbRowsTotal+=totalRows;
    if(totalRows==1){
      nbRowsScore += (40*(curLevel+1));
    }else if(totalRows==2){
      nbRowsScore += (100*(curLevel+1));
    }else if(totalRows==3){
      nbRowsScore += (300*(curLevel+1));
    }else{
      nbRowsScore += (1200*(curLevel+1));
    }
    updateVisibleScore();
    updateVisibleLines();
    LevelUp();
    checkHighestBrickInField();
  }//if(totalRows>0){
}

void clearField() {
  uint8_t x, y;
  for (y = 0; y < TETRIS_FIELD_HEIGHT; y++) {
    for (x = 0; x < TETRIS_FIELD_WIDTH; x++) {
      field.pix[x][y] = 0;
      field.color[x][y] = 0;
    }
  }
  for (x = 0; x < TETRIS_FIELD_WIDTH; x++) { //This last row is invisible to the player and only used for the collision detection routine
    field.pix[x][TETRIS_FIELD_HEIGHT] = 1;
  }
}

void updateVisibleScore(){
  char bufVisibleScore[6];
  int lenVisibleScore = sprintf(bufVisibleScore, "%i", nbRowsScore);
  if(lenVisibleScore==2){
    setSmallText(bufVisibleScore, lenVisibleScore, 21, -1, AQUA);
  }
  if(lenVisibleScore==3){
    setSmallText(bufVisibleScore, lenVisibleScore, 17, -1, AQUA);
  }
  if(lenVisibleScore==4){
    setSmallText(bufVisibleScore, lenVisibleScore, 13, -1, AQUA);
  }
  if(lenVisibleScore==5){
    setSmallText(bufVisibleScore, lenVisibleScore, 9, -1, AQUA);
  }
  if(lenVisibleScore==6){
    setSmallText(bufVisibleScore, lenVisibleScore, 5, -1, AQUA);
  }
}
