/* LedTable

   Written by: Greg Beckman

   High Scores system for the Tetris and Snake Game
*/

#define HSTEXTSPEED 30
unsigned long currentMenuMillis;
long previousMenuMillis = 0; 
long menuInterval = 500;
int cursorOn = 0;
int pageNum = 0;

void runTetrisHighScore(){
  pageNum = 0;
  long CurrentHStet[10];
  char CurrentHSname[10][4] = {{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "}};
  
  for(int i = 0; i <10; i++){
    CurrentHStet[i] = EEPROM.readLong(addressLong[i]);
    EEPROM.readBlock<char>(addressCharArray[i],CurrentHSname[i],3);
  }
  

  char buftet[10][6];
  int lentet[10];
  for(int index=0; index<10; index++){
    lentet[index] = sprintf(buftet[index], "%i", CurrentHStet[index]);
  }

  clearTablePixels();
  setSmallText((char*)"1.", 2, 1, 0, WHITE);
  setSmallWideText(CurrentHSname[0],3, 8, 0, AQUA);
  setSmallText(buftet[0], lentet[0], 5+((5-lentet[0])*2), 6, GREY);
  
  setSmallText((char*)"2.", 2, 1, 13, WHITE);
  setSmallWideText(CurrentHSname[1],3, 8, 13, AQUA);
  setSmallText(buftet[1], lentet[1], 5+((5-lentet[1])*2), 19, GREY);

  showPixels();


  while(true){
    //Show menu system and wait for input
    
    boolean thsSelectionChanged = false;
    boolean exitThsSelection = false;

      //thsSelectionChanged = true;

    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_A || curControl == BTN_RIGHT){
        if(pageNum==4){
          exitThsSelection = true;
        }
        pageNum++;
        if(pageNum>4){
          pageNum=4;
        }
        thsSelectionChanged = true;
      }
      if (curControl == BTN_UP || curControl == BTN_LEFT){
        if(pageNum==0){
          exitThsSelection = true;
        }
        pageNum--;
        if(pageNum<0){
          pageNum=0;
        }
        thsSelectionChanged = true;
      }
      if(curControl == BTN_START || curControl == BTN_SELECT || curControl == BTN_B){
        exitThsSelection = true;
      }
      
      
    }
    if (thsSelectionChanged){
      int space2Spacer = 0;
      if(pageNum==4){
        space2Spacer = 1;
      }
      clearTablePixels();
      char numOneText[2];
      char numTwoText[2];
      int lenNumOne = sprintf(numOneText, "%i", (pageNum*2+1));
      int lenNumTwo = sprintf(numTwoText, "%i", (pageNum*2+2));
      setSmallText((char*)".", lenNumOne, 5, 0, WHITE);
      setSmallText(numOneText,1,1,0,WHITE);
      setSmallWideText(CurrentHSname[pageNum*2],3, 8, 0, AQUA);
      setSmallText(buftet[pageNum*2], lentet[pageNum*2], 5+((5-lentet[pageNum*2])*2), 6, GREY);
      
      setSmallText((char*)".", 1, lenNumTwo*4+1-space2Spacer, 13, WHITE);
      setSmallText(numTwoText,lenNumTwo,1-space2Spacer,13,WHITE);
      setSmallWideText(CurrentHSname[pageNum*2+1],3, lenNumTwo*4+4-(space2Spacer*2), 13, AQUA);
      setSmallText(buftet[pageNum*2+1], lentet[pageNum*2+1], 5+((5-lentet[pageNum*2+1])*2), 19, GREY);
      showPixels();
    }
    //If we are here, it means a selection was changed or a game started
    if (exitThsSelection){
      runTetrisMenu();
      break;
    }
  }
}

void runTetrisDeleteScore(){
  pageNum = 0;
  int scoreNum = 0;
  long CurrentHStet[10];
  char CurrentHSname[10][4] = {{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "}};
  
  for(int i = 0; i <10; i++){
    CurrentHStet[i] = EEPROM.readLong(addressLong[i]);
    EEPROM.readBlock<char>(addressCharArray[i],CurrentHSname[i],3);
  }
  

  char buftet[10][6];
  int lentet[10];
  for(int index=0; index<10; index++){
    lentet[index] = sprintf(buftet[index], "%i", CurrentHStet[index]);
  }

  clearTablePixels();
  setSmallText((char*)"1.", 2, 1, 0, WHITE);
  setSmallWideText(CurrentHSname[0],3, 8, 0, AQUA);
  setSmallText(buftet[0], lentet[0], 5+((5-lentet[0])*2), 6, GREY);
  
  setSmallText((char*)"2.", 2, 1, 13, WHITE);
  setSmallWideText(CurrentHSname[1],3, 8, 13, AQUA);
  setSmallText(buftet[1], lentet[1], 5+((5-lentet[1])*2), 19, GREY);

  showPixels();


  while(true){
    //Show menu system and wait for input
    
    boolean thsSelectionChanged = false;
    boolean exitThsSelection = false;

    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      thsSelectionChanged = true;
    }

    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_A || curControl == BTN_RIGHT){
        if(scoreNum==9){
          exitThsSelection = true;
        }
        scoreNum++;
        if(scoreNum>9){
          scoreNum=9;
        }
        thsSelectionChanged = true;
      }
      if (curControl == BTN_UP || curControl == BTN_B || curControl == BTN_LEFT){
        if(scoreNum==0){
          exitThsSelection = true;
        }
        scoreNum--;
        if(scoreNum<0){
          scoreNum=0;
        }
        thsSelectionChanged = true;
      }
      if(curControl == BTN_START){
        exitThsSelection = true;
      }

      if(curControl == BTN_SELECT){
        for(int i = scoreNum; i < 9; i ++){
                  //Serial.print("writing HS ");Serial.println(i);
          EEPROM.writeLong(addressLong[i],CurrentHStet[i+1]);
          delay(15);
                  //Serial.print("wrote HS ");Serial.println(i);
                  //Serial.print("writing HS name ");Serial.println(i);
          EEPROM.writeBlock<char>(addressCharArray[i], CurrentHSname[i+1],3);
          delay(15);
                  //Serial.print("wrote HS name ");Serial.println(i);
        }
                //Serial.println("writing HS 9");
        EEPROM.writeLong(addressLong[9],long(1));
        delay(15);
                //Serial.println("wrote HS 9");
                //Serial.println("writing HS name 9");
        EEPROM.writeBlock<char>(addressCharArray[9], inputChar1,3);
        delay(15);
                //Serial.println("wrote HS name 9");
        exitThsSelection = true;
      }
      pageNum = int(floor(scoreNum/2));
    }
    if (thsSelectionChanged){
      
      int space2Spacer = 0;
      if(pageNum==4){
        space2Spacer = 1;
      }
      clearTablePixels();
      char numOneText[2];
      char numTwoText[2];
      int lenNumOne = sprintf(numOneText, "%i", (pageNum*2+1));
      int lenNumTwo = sprintf(numTwoText, "%i", (pageNum*2+2));
      setSmallText((char*)".", lenNumOne, 5, 0, WHITE);
      setSmallText(numOneText,1,1,0,WHITE);
      setSmallText((char*)".", 1, lenNumTwo*4+1-space2Spacer, 13, WHITE);
      setSmallText(numTwoText,lenNumTwo,1-space2Spacer,13,WHITE);
      

      if(scoreNum%2==0){
        if(cursorOn==0){
          setSmallWideText((char*)"   ",3, 8, 0, AQUA);
          setSmallText((char*)"      ", 6, 3, 6, GREY);
        }else{
          setSmallWideText(CurrentHSname[pageNum*2],3, 8, 0, AQUA);
          setSmallText(buftet[pageNum*2], lentet[pageNum*2], 5+((5-lentet[pageNum*2])*2), 6, GREY);
        }
        setSmallWideText(CurrentHSname[pageNum*2+1],3, lenNumTwo*4+4-(space2Spacer*2), 13, AQUA);
        setSmallText(buftet[pageNum*2+1], lentet[pageNum*2+1], 5+((5-lentet[pageNum*2+1])*2), 19, GREY);
      }else{
        setSmallWideText(CurrentHSname[pageNum*2],3, 8, 0, AQUA);
        setSmallText(buftet[pageNum*2], lentet[pageNum*2], 5+((5-lentet[pageNum*2])*2), 6, GREY);
        if(cursorOn==0){
          setSmallWideText(CurrentHSname[pageNum*2+1],3, lenNumTwo*4+4-(space2Spacer*2), 13, AQUA);
          setSmallText(buftet[pageNum*2+1], lentet[pageNum*2+1], 5+((5-lentet[pageNum*2+1])*2), 19, GREY);
        }else{
          setSmallWideText((char*)"   ",3, lenNumTwo*4+4-(space2Spacer*2), 13, AQUA);
          setSmallText((char*)"      ", 6, 3, 19, GREY);
        }
      }
      showPixels();
    }
    //If we are here, it means a selection was changed or a game started
    if (exitThsSelection){
      runTetrisMenu();
      break;
    }
  }
}


void runSnakeHighScore(){

  pageNum = 0;
  int CurrentSnakeHStet[10];
  char CurrentSnakeHSname[10][4] = {{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "}};
  
  for(int i = 0; i <10; i++){
    CurrentSnakeHStet[i] = EEPROM.readInt(addressInt[i]);
    EEPROM.readBlock<char>(addressCharArray2[i],CurrentSnakeHSname[i],3);
  }
  

  char buftet[10][6];
  int lentet[10];
  for(int index=0; index<10; index++){
    lentet[index] = sprintf(buftet[index], "%i", CurrentSnakeHStet[index]);
  }

  clearTablePixels();
  setSmallText((char*)"1.", 2, 1, 0, WHITE);
  setSmallWideText(CurrentSnakeHSname[0],3, 8, 0, AQUA);
  setSmallText(buftet[0], lentet[0], 6+((5-lentet[0])*2), 6, GREY);
  
  setSmallText((char*)"2.", 2, 1, 13, WHITE);
  setSmallWideText(CurrentSnakeHSname[1],3, 8, 13, AQUA);
  setSmallText(buftet[1], lentet[1], 6+((5-lentet[1])*2), 19, GREY);

  showPixels();


  while(true){
    //Show menu system and wait for input
    
    boolean thsSelectionChanged = false;
    boolean exitThsSelection = false;

      //thsSelectionChanged = true;

    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_A || curControl == BTN_RIGHT){
        if(pageNum==4){
          exitThsSelection = true;
        }
        pageNum++;
        if(pageNum>4){
          pageNum=4;
        }
        thsSelectionChanged = true;
      }
      if (curControl == BTN_UP || curControl == BTN_LEFT){
        if(pageNum==0){
          exitThsSelection = true;
        }
        pageNum--;
        if(pageNum<0){
          pageNum=0;
        }
        thsSelectionChanged = true;
      }
      if(curControl == BTN_START || curControl == BTN_SELECT || curControl == BTN_B){
        exitThsSelection = true;
      }
   
    }
    if (thsSelectionChanged){
      int space2Spacer = 0;
      if(pageNum==4){
        space2Spacer = 1;
      }
      clearTablePixels();
      char numOneText[2];
      char numTwoText[2];
      int lenNumOne = sprintf(numOneText, "%i", (pageNum*2+1));
      int lenNumTwo = sprintf(numTwoText, "%i", (pageNum*2+2));
      setSmallText((char*)".", lenNumOne, 5, 0, WHITE);
      setSmallText(numOneText,1,1,0,WHITE);
      setSmallWideText(CurrentSnakeHSname[pageNum*2],3, 8, 0, AQUA);
      setSmallText(buftet[pageNum*2], lentet[pageNum*2], 6+((5-lentet[pageNum*2])*2), 6, GREY);
      
      setSmallText((char*)".", 1, lenNumTwo*4+1-space2Spacer, 13, WHITE);
      setSmallText(numTwoText,lenNumTwo,1-space2Spacer,13,WHITE);
      setSmallWideText(CurrentSnakeHSname[pageNum*2+1],3, lenNumTwo*4+4-(space2Spacer*2), 13, AQUA);
      setSmallText(buftet[pageNum*2+1], lentet[pageNum*2+1], 6+((5-lentet[pageNum*2+1])*2), 19, GREY);
      showPixels();
    }
    //If we are here, it means a selection was changed or a game started
    if (exitThsSelection){
      runSnakeMenu();
      break;
    }
  }
  
}


void runSnakeDeleteScore(){
  pageNum = 0;
  int scoreNum = 0;
  int CurrentHStet[10];
  char CurrentHSname[10][4] = {{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "},{"   "}};
  
  for(int i = 0; i <10; i++){
    CurrentHStet[i] = EEPROM.readInt(addressInt[i]);
    EEPROM.readBlock<char>(addressCharArray2[i],CurrentHSname[i],3);
  }
  

  char buftet[10][6];
  int lentet[10];
  for(int index=0; index<10; index++){
    lentet[index] = sprintf(buftet[index], "%i", CurrentHStet[index]);
  }

  clearTablePixels();
  setSmallText((char*)"1.", 2, 1, 0, WHITE);
  setSmallWideText(CurrentHSname[0],3, 8, 0, AQUA);
  setSmallText(buftet[0], lentet[0], 5+((5-lentet[0])*2), 6, GREY);
  
  setSmallText((char*)"2.", 2, 1, 13, WHITE);
  setSmallWideText(CurrentHSname[1],3, 8, 13, AQUA);
  setSmallText(buftet[1], lentet[1], 5+((5-lentet[1])*2), 19, GREY);

  showPixels();


  while(true){
    //Show menu system and wait for input
    
    boolean thsSelectionChanged = false;
    boolean exitThsSelection = false;

    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      thsSelectionChanged = true;
    }

    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_A || curControl == BTN_RIGHT){
        if(scoreNum==9){
          exitThsSelection = true;
        }
        scoreNum++;
        if(scoreNum>9){
          scoreNum=9;
        }
        thsSelectionChanged = true;
      }
      if (curControl == BTN_UP || curControl == BTN_B || curControl == BTN_LEFT){
        if(scoreNum==0){
          exitThsSelection = true;
        }
        scoreNum--;
        if(scoreNum<0){
          scoreNum=0;
        }
        thsSelectionChanged = true;
      }
      if(curControl == BTN_START){
        exitThsSelection = true;
      }

      if(curControl == BTN_SELECT){
        for(int i = scoreNum; i < 9; i ++){
                  //Serial.print("writing HS ");Serial.println(i);
          EEPROM.writeInt(addressInt[i],CurrentHStet[i+1]);
          delay(15);
                  //Serial.print("wrote HS ");Serial.println(i);
                  //Serial.print("writing HS name ");Serial.println(i);
          EEPROM.writeBlock<char>(addressCharArray2[i], CurrentHSname[i+1],3);
          delay(15);
                  //Serial.print("wrote HS name ");Serial.println(i);
        }
                //Serial.println("writing HS 9");
        EEPROM.writeInt(addressInt[9],long(1));
        delay(15);
                //Serial.println("wrote HS 9");
                //Serial.println("writing HS name 9");
        EEPROM.writeBlock<char>(addressCharArray2[9], inputChar1,3);
        delay(15);
                //Serial.println("wrote HS name 9");
        exitThsSelection = true;
      }
      pageNum = int(floor(scoreNum/2));
    }
    if (thsSelectionChanged){
      
      int space2Spacer = 0;
      if(pageNum==4){
        space2Spacer = 1;
      }
      clearTablePixels();
      char numOneText[2];
      char numTwoText[2];
      int lenNumOne = sprintf(numOneText, "%i", (pageNum*2+1));
      int lenNumTwo = sprintf(numTwoText, "%i", (pageNum*2+2));
      setSmallText((char*)".", lenNumOne, 5, 0, WHITE);
      setSmallText(numOneText,1,1,0,WHITE);
      setSmallText((char*)".", 1, lenNumTwo*4+1-space2Spacer, 13, WHITE);
      setSmallText(numTwoText,lenNumTwo,1-space2Spacer,13,WHITE);
      

      if(scoreNum%2==0){
        if(cursorOn==0){
          setSmallWideText((char*)"   ",3, 8, 0, AQUA);
          setSmallText((char*)"      ", 6, 3, 6, GREY);
        }else{
          setSmallWideText(CurrentHSname[pageNum*2],3, 8, 0, AQUA);
          setSmallText(buftet[pageNum*2], lentet[pageNum*2], 5+((5-lentet[pageNum*2])*2), 6, GREY);
        }
        setSmallWideText(CurrentHSname[pageNum*2+1],3, lenNumTwo*4+4-(space2Spacer*2), 13, AQUA);
        setSmallText(buftet[pageNum*2+1], lentet[pageNum*2+1], 5+((5-lentet[pageNum*2+1])*2), 19, GREY);
      }else{
        setSmallWideText(CurrentHSname[pageNum*2],3, 8, 0, AQUA);
        setSmallText(buftet[pageNum*2], lentet[pageNum*2], 5+((5-lentet[pageNum*2])*2), 6, GREY);
        if(cursorOn==0){
          setSmallWideText(CurrentHSname[pageNum*2+1],3, lenNumTwo*4+4-(space2Spacer*2), 13, AQUA);
          setSmallText(buftet[pageNum*2+1], lentet[pageNum*2+1], 5+((5-lentet[pageNum*2+1])*2), 19, GREY);
        }else{
          setSmallWideText((char*)"   ",3, lenNumTwo*4+4-(space2Spacer*2), 13, AQUA);
          setSmallText((char*)"      ", 6, 3, 19, GREY);
        }
      }
      showPixels();
    }
    //If we are here, it means a selection was changed or a game started
    if (exitThsSelection){
      runSnakeMenu();
      break;
    }
  }
}
