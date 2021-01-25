/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty - Modified by: Mark Quinn - Modified again to be like SNES classic by: Greg Beckman
 * 
 * Menu system for the Tetris Game
 */

#define MINLEVELSELECTION  0
#define MAXLEVELSELECTION  19
signed int curLevelSelection = MINLEVELSELECTION;


int sfxON = 1;
int musicType = 0;
int prevMusicType = 1;

int LEVELTEXTSPEED = 30;

void runTetrisMenu(void){
  Serial3.write(uint8_t(5));// back to controller, says we are in menu mode
  
  Serial2.write('T');
  Serial2.flush();
  clearTablePixels();
  setSmallText((char*)"TETRIS", 6, 2, 1, AQUA);
  
  if(curTetrisMenuSelection==0){
    setSmallText((char*)"Start", 5, 4, menuItemYval, WHITE);
  }else if(curTetrisMenuSelection==1){
    setSmallText((char*)"Level", 5, 4, menuItemYval, WHITE);
  }else if(curTetrisMenuSelection==2){
    setSmallText((char*)"Music", 5, 4, menuItemYval, WHITE);
  }else if(curTetrisMenuSelection==3){
    setSmallText((char*)"SFX", 3, 8, menuItemYval, WHITE);
  }else if(curTetrisMenuSelection==4){
    setSmallText((char*)"Ghost", 5, 4, menuItemYval, WHITE);
  }else if(curTetrisMenuSelection==5){
    setSmallText((char*)"RNG", 3, 8, menuItemYval, WHITE);
  }else if(curTetrisMenuSelection==6){
    setSmallText((char*)"Invsble", 7, 0, menuItemYval, WHITE);//<====this one
    }else if(curTetrisMenuSelection==7){
    setSmallText((char*)"Scores", 6, 2, menuItemYval, WHITE);
  }else if(curTetrisMenuSelection==8){
    setSmallText((char*)"Delete", 6, 2, menuItemYval, WHITE);
  }
  showPixels();
  

  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"TETRIS", 6, 2, 1, AQUA);
    
    boolean tetrisSelectionChanged = false;
    boolean runTetrisSelection = false;
    boolean exitTetrisSelection = false;
    boolean playTetris = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      tetrisSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        curTetrisMenuSelection++;
        if (curTetrisMenuSelection>8){
          curTetrisMenuSelection = 8;
        }
        tetrisSelectionChanged = true;
      }
      if (curControl == BTN_UP){
        curTetrisMenuSelection--;
        if (curTetrisMenuSelection<0){
          curTetrisMenuSelection = 0;
        }
        tetrisSelectionChanged = true;
      }
      if (curControl == BTN_A){
        runTetrisSelection = true;
      }
      if(curControl == BTN_START){
        playTetris = true;
      }
      if (curControl == BTN_SELECT || curControl == BTN_B){
        exitTetrisSelection = true;
      } 
    }

    if (tetrisSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        if(curTetrisMenuSelection!=8){
          setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        }
        if(curTetrisMenuSelection!=0){
          setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
        }
      }
      
      if(curTetrisMenuSelection==0){
        setSmallText((char*)"Start", 5, 4, menuItemYval, WHITE);
      }else if(curTetrisMenuSelection==1){
        setSmallText((char*)"Level", 5, 4, menuItemYval, WHITE);
      }else if(curTetrisMenuSelection==2){
        setSmallText((char*)"Music", 5, 4, menuItemYval, WHITE);
      }else if(curTetrisMenuSelection==3){
        setSmallText((char*)"SFX", 3, 8, menuItemYval, WHITE);
      }else if(curTetrisMenuSelection==4){
        setSmallText((char*)"Ghost", 5, 4, menuItemYval, WHITE);
      }else if(curTetrisMenuSelection==5){
        setSmallText((char*)"RNG", 3, 8, menuItemYval, WHITE);
      }else if(curTetrisMenuSelection==6){
        setSmallText((char*)"Invsble", 7, 0, menuItemYval, WHITE);
        }else if(curTetrisMenuSelection==7){
        setSmallText((char*)"Scores", 6, 2, menuItemYval, WHITE);
      }else if(curTetrisMenuSelection==8){
        setSmallText((char*)"Delete", 6, 2, menuItemYval, WHITE);
      }
      
      showPixels();
    }
    
    if (runTetrisSelection) {
      if(curTetrisMenuSelection==0){
        runTetris(curLevelSelection);
        break;
      }else if(curTetrisMenuSelection==1){
        runLevelSelect();
        break;
      }else if(curTetrisMenuSelection==2){
        runMusicSelect();
        break;
      }else if(curTetrisMenuSelection==3){
        runSfxSelect();
        break;
      }else if(curTetrisMenuSelection==4){
        runGhostSelect();
        break;
      }else if(curTetrisMenuSelection==5){
        runRNGSelect();
        break;
      }else if(curTetrisMenuSelection==6){
        runInvisibleSelect();
        break;
      }else if(curTetrisMenuSelection==7){
        runTetrisHighScore();
      }else{
        runTetrisDeleteScore();
      }
    }
    if(playTetris){
      runTetris(curLevelSelection);
      break; 
    }
    if(exitTetrisSelection){
      mainLoop();
      break;
    }
  }
}

void runLevelSelect(void){
  Serial2.write('T');
  Serial2.flush();

  char bufLevel[2];
  int lenLevel = sprintf(bufLevel, "%i", curLevelSelection);
  
  clearTablePixels();
  setSmallText((char*)"LEVEL", 5, 4, 1, AQUA);
  if(curLevelSelection<10){
    setPixelText(bufLevel, lenLevel, 11, 12, WHITE);
  } else {
    setPixelText(bufLevel, lenLevel, 7, 12, WHITE);
  }
  showPixels();

  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"LEVEL", 5, 4, 1, AQUA);
    
    boolean levelSelectionChanged = false;
    boolean runLevelSelection = false;
    boolean exitLevelSelection = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      levelSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        curLevelSelection--;
        if (curLevelSelection<=MINLEVELSELECTION){
          curLevelSelection = 0;
        }
        levelSelectionChanged = true;
      } else if (curControl == BTN_UP){
        curLevelSelection++;
        if (curLevelSelection>MAXLEVELSELECTION){
          curLevelSelection = MAXLEVELSELECTION;
        }
        levelSelectionChanged = true;
      } else if (curControl == BTN_START){
        runLevelSelection = true;
      } else if (curControl == BTN_SELECT || curControl == BTN_A || curControl == BTN_B){
        exitLevelSelection = true;
      }
    }

  
    if (levelSelectionChanged){
      lenLevel = sprintf(bufLevel, "%i", curLevelSelection);
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-6,AQUA);
      }else{
        if(curLevelSelection!=0){
          setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        }
        if(curLevelSelection!=19){
          setSmallText((char*)"^",1,12,menuItemYval-6,AQUA);
        }
      }
      if(curLevelSelection<10){
        setPixelText(bufLevel, lenLevel, 11, 12, WHITE);
      } else {
        setPixelText(bufLevel, lenLevel, 7, 12, WHITE);

      }
      showPixels();
    }

    if (runLevelSelection){//Start selected game
      runTetris(curLevelSelection);
      break;   
    } else if (exitLevelSelection){
      runTetrisMenu();
      break;
    }
  }
}

void runMusicSelect(void){
  Serial2.write('P');
  Serial2.flush();
  clearTablePixels();
  setSmallText((char*)"MUSIC", 5, 4, 1, AQUA);
  if(musicType==0){
    setSmallText((char*)"GB A", 4, 6, menuItemYval, WHITE);//typeA
  }else if(musicType==1){
    setSmallText((char*)"GB B", 4, 6, menuItemYval, WHITE);//typeB
  }else if(musicType==2){
    setSmallText((char*)"GB C", 4, 6, menuItemYval, WHITE);//typeC
  }else if(musicType==3){
    setSmallText((char*)"NES 1", 5, 5, menuItemYval, WHITE);//nes music 1
  }else if(musicType==4){
    setSmallText((char*)"NES 2", 5, 5, menuItemYval, WHITE);//nes music 2
  }else if(musicType==5){
    setSmallText((char*)"NES 3", 5, 5, menuItemYval, WHITE);//nes music 3
  }else if(musicType==6){
    setSmallText((char*)"REMIX 1", 8, 0, menuItemYval, WHITE);
  }else if(musicType==7){
    setSmallText((char*)"REMIX 2", 8, 0, menuItemYval, WHITE);
  }else if(musicType==8){
    setSmallText((char*)"REMIX 3", 8, 0, menuItemYval, WHITE);
  }else if(musicType==9){
    setSmallText((char*)"REMIX 4", 8, 0, menuItemYval, WHITE);
  }else if(musicType==10){
    setSmallText((char*)"REMIX 5", 8, 0, menuItemYval, WHITE);
  }else if(musicType==11){
    setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
  }
  showPixels();

  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"MUSIC", 5, 4, 1, AQUA);
    boolean musicSelectionChanged = false;
    boolean exitMusicSelection = false;
    boolean runStartTetris = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      musicSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        musicType++;
        if (musicType>11){
          musicType = 11;
        }
        musicSelectionChanged = true;
      } else if (curControl == BTN_UP){
        musicType--;
        if (musicType<0){
          musicType = 0;
        }
        musicSelectionChanged = true;
      } else if (curControl == BTN_A || curControl == BTN_B || curControl == BTN_SELECT){
        exitMusicSelection = true;
      } else if (curControl == BTN_START){
        runStartTetris = true;
      } 
    }

  
    if (musicSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        if(musicType!=11){
          setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        }
        if(musicType!=0){
          setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
        }
      }
      if(musicType==0){
        setSmallText((char*)"GB A", 4, 6, menuItemYval, WHITE);//typeA
        if(prevMusicType!=0){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('A');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==1){
        setSmallText((char*)"GB B", 4, 6, menuItemYval, WHITE);//typeB
        if(prevMusicType!=1){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('B');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==2){
        setSmallText((char*)"GB C", 4, 6, menuItemYval, WHITE);//typec
        if(prevMusicType!=2){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('C');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==3){
        setSmallText((char*)"NES 1", 5, 5, menuItemYval, WHITE);//nes music 1
        if(prevMusicType!=3){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('D');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==4){
        setSmallText((char*)"NES 2", 5, 5, menuItemYval, WHITE);//nes music 2
        if(prevMusicType!=4){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('E');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==5){
        setSmallText((char*)"NES 3", 5, 5, menuItemYval, WHITE);//nes music 3
        if(prevMusicType!=5){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('F');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==6){
        setSmallText((char*)"REMIX 1", 8, 0, menuItemYval, WHITE);
        if(prevMusicType!=6){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('I');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==7){
        setSmallText((char*)"REMIX 2", 8, 0, menuItemYval, WHITE);
        if(prevMusicType!=7){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('J');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==8){
        setSmallText((char*)"REMIX 3", 8, 0, menuItemYval, WHITE);
        if(prevMusicType!=8){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('K');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==9){
        setSmallText((char*)"REMIX 4", 8, 0, menuItemYval, WHITE);
        if(prevMusicType!=9){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('L');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==10){
        setSmallText((char*)"REMIX 5", 8, 0, menuItemYval, WHITE);
        if(prevMusicType!=10){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('!');
          Serial2.flush();
          Serial2.write('M');
          Serial2.flush();
          Serial2.write('P');
          Serial2.flush();
        }
      }else if(musicType==11){
        setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
        if(prevMusicType!=9){
          Serial2.write('S');
          Serial2.flush();
          Serial2.write('@');
          Serial2.flush();
        }
      }
      showPixels();
    }
    
    if (runStartTetris) {
      runTetris(curLevelSelection);
    }
    
    //If we are here, it means a selection was changed or a game started
    if (exitMusicSelection){
      runTetrisMenu();
      break;
    }
    prevMusicType = musicType;
  }
}

void runSfxSelect(void){
  clearTablePixels();
  setSmallText((char*)"SFX", 3, 8, 1, AQUA);

  if(sfxON==1){
    setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
  }else if(sfxON==0){
    setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
  }
  showPixels();
  
  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"SFX", 3, 8, 1, AQUA);
    boolean sfxSelectionChanged = false;
    boolean exitSfxSelection = false;
    boolean runFxStartTetris = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      sfxSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_UP){
        sfxON++;
        if (sfxON>1){
          sfxON = 0;
        }
        sfxSelectionChanged = true;
      } else if (curControl == BTN_A || BTN_B || curControl == BTN_SELECT){
        exitSfxSelection = true;
      } else if (curControl == BTN_START){
        runFxStartTetris = true;
      } 
    }

  
    if (sfxSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
      }
      if(sfxON==0){
        Serial2.write('0');
        Serial2.flush();
        setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
      }else if(sfxON==1){
        Serial2.write('1');
        Serial2.flush();
        Serial2.write('r');
        Serial2.flush();
        setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
      }
      showPixels();
    }

    if (runFxStartTetris) {
      runTetris(curLevelSelection);
    }
    
    //If we are here, it means a selection was changed or a game started
    if (exitSfxSelection){
      runTetrisMenu();
      break;
    }
  }
}

void runGhostSelect(void){
  clearTablePixels();
  setSmallText((char*)"GHOST", 5, 4, 1, AQUA);

  if(ghostON==1){
    setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
  }else if(ghostON==0){
    setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
  }
  showPixels();
  
  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"GHOST", 5, 4, 1, AQUA);
    boolean ghostSelectionChanged = false;
    boolean exitGhostSelection = false;
    boolean runGhostStartTetris = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      ghostSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_UP){
        ghostON++;
        if (ghostON>1){
          ghostON = 0;
        }
        ghostSelectionChanged = true;
      } else if (curControl == BTN_A || BTN_B || curControl == BTN_SELECT){
        exitGhostSelection = true;
      } else if (curControl == BTN_START){
        runGhostStartTetris = true;
      } 
    }

  
    if (ghostSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
      }
      if(ghostON==0){
        setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
      }else if(ghostON==1){
        setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
      }
      showPixels();
    }

    if (runGhostStartTetris) {
      runTetris(curLevelSelection);
    }
    
    //If we are here, it means a selection was changed or a game started
    if (exitGhostSelection){
      runTetrisMenu();
      break;
    }
  }
}

void runRNGSelect(void){
  clearTablePixels();
  setSmallText((char*)"RNG", 3, 8, 1, AQUA);

  if(rngON==1){
    setSmallText((char*)"BAD", 3, 8, menuItemYval, WHITE);//on
  }else if(rngON==0){
    setSmallText((char*)"GOOD", 4, 6, menuItemYval, WHITE);//off
  }
  showPixels();
  
  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"RNG", 3, 8, 1, AQUA);
    boolean rngSelectionChanged = false;
    boolean exitRngSelection = false;
    boolean runRngStartTetris = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      rngSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_UP){
        rngON++;
        if (rngON>1){
          rngON = 0;
        }
        rngSelectionChanged = true;
      } else if (curControl == BTN_A || BTN_B || curControl == BTN_SELECT){
        exitRngSelection = true;
      } else if (curControl == BTN_START){
        runRngStartTetris = true;
      } 
    }

  
    if (rngSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
      }
      if(rngON==0){
        setSmallText((char*)"GOOD", 4, 6, menuItemYval, WHITE);//off
      }else if(rngON==1){
        setSmallText((char*)"BAD", 3, 8, menuItemYval, WHITE);//on
      }
      showPixels();
    }

    if (runRngStartTetris) {
      runTetris(curLevelSelection);
    }
    
    //If we are here, it means a selection was changed or a game started
    if (exitRngSelection){
      runTetrisMenu();
      break;
    }
  }
}

void runInvisibleSelect(void){
  clearTablePixels();
  setSmallText((char*)"INVSBLE", 7, 0, 1, AQUA);

  if(invisibleON==1){
    setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
  }else if(invisibleON==0){
    setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
  }
  showPixels();
  
  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"INVSBLE", 7, 0, 1, AQUA);
    boolean invisibleSelectionChanged = false;
    boolean exitInvisibleSelection = false;
    boolean runInvisibleStartTetris = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      invisibleSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_UP){
        invisibleON++;
        if (invisibleON>1){
          invisibleON = 0;
        }
        invisibleSelectionChanged = true;
      } else if (curControl == BTN_A || BTN_B || curControl == BTN_SELECT){
        exitInvisibleSelection = true;
      } else if (curControl == BTN_START){
        runInvisibleStartTetris = true;
      } 
    }

  
    if (invisibleSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
      }
      if(invisibleON==0){
        setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
      }else if(invisibleON==1){
        setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
      }
      showPixels();
    }

    if (runInvisibleStartTetris) {
      runTetris(curLevelSelection);
    }
    
    //If we are here, it means a selection was changed or a game started
    if (exitInvisibleSelection){
      runTetrisMenu();
      break;
    }
  }
}
