/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty - Modified by: Mark Quinn - Modified again by: Greg Beckman
 * 
 * Menu system for the Snake Game
 */

#define SNAKEMINLEVELSELECTION  0
#define SNAKEMAXLEVELSELECTION  2
signed int curSnakeLevelSelection = SNAKEMINLEVELSELECTION;
int snakeMaxMenuSelect = 6;

int snakeSfxON = 1;
int snakeMusicON = 1;
int prevSnakeMusicON = 1;
int snakeMazeType = 0;

void runSnakeMenu(void){
  Serial3.write(uint8_t(5));// back to controller, says we are in menu mode
  
  Serial2.write('Z');
  Serial2.flush();
  clearTablePixels();
  setSmallText((char*)"SNAKE", 5, 4, 1, AQUA);
  
  if(curSnakeMenuSelection==0){
    setSmallText((char*)"Start", 5, 4, menuItemYval, WHITE);
  }else if(curSnakeMenuSelection==1){
    setSmallText((char*)"Level", 5, 4, menuItemYval, WHITE);
  }else if(curSnakeMenuSelection==2){
    setSmallText((char*)"Maze", 4, 6, menuItemYval, WHITE);
  }else if(curSnakeMenuSelection==3){
    setSmallText((char*)"Music", 5, 4, menuItemYval, WHITE);
  }else if(curSnakeMenuSelection==4){
    setSmallText((char*)"SFX", 3, 8, menuItemYval, WHITE);
  }else if(curSnakeMenuSelection==5){
    setSmallText((char*)"Scores", 6, 2, menuItemYval, WHITE);
  }else if(curSnakeMenuSelection==6){
    setSmallText((char*)"Delete", 6, 2, menuItemYval, WHITE);
  }
  showPixels();
 
  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"SNAKE", 5, 4, 1, AQUA);
    //setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
    
    boolean snakeSelectionChanged = false;
    boolean runSnakeSelection = false;
    boolean exitSnakeSelection = false;
    boolean playSnake = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      snakeSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        curSnakeMenuSelection++;
        if (curSnakeMenuSelection>snakeMaxMenuSelect){
          curSnakeMenuSelection = snakeMaxMenuSelect;
        }
        snakeSelectionChanged = true;
      }
      if (curControl == BTN_UP){
        curSnakeMenuSelection--;
        if (curSnakeMenuSelection<0){
          curSnakeMenuSelection = 0;
        }
        snakeSelectionChanged = true;
      }
      if (curControl == BTN_A){
        runSnakeSelection = true;
      }
      if(curControl == BTN_START){
        playSnake = true;
      }
      if (curControl == BTN_SELECT || curControl == BTN_B){
        exitSnakeSelection = true;
      } 
    }

  
    if (snakeSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        if(curSnakeMenuSelection!=snakeMaxMenuSelect){
          setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        }
        if(curSnakeMenuSelection!=0){
          setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
        }
      }
      
      if(curSnakeMenuSelection==0){
        setSmallText((char*)"Start", 5, 4, menuItemYval, WHITE);
      }else if(curSnakeMenuSelection==1){
        setSmallText((char*)"Level", 5, 4, menuItemYval, WHITE);
      }else if(curSnakeMenuSelection==2){
        setSmallText((char*)"Maze", 4, 6, menuItemYval, WHITE);
      }else if(curSnakeMenuSelection==3){
        setSmallText((char*)"Music", 5, 4, menuItemYval, WHITE);
      }else if(curSnakeMenuSelection==4){
        setSmallText((char*)"SFX", 3, 8, menuItemYval, WHITE);
      }else if(curSnakeMenuSelection==5){
        setSmallText((char*)"Scores", 6, 2, menuItemYval, WHITE);
      }else if(curSnakeMenuSelection==6){
        setSmallText((char*)"Delete", 6, 2, menuItemYval, WHITE);
      }
      
      showPixels();
    }
    
    
    if (runSnakeSelection) {
      if(curSnakeMenuSelection==0){
        runSnake(curSnakeLevelSelection,snakeMazeType);
        break;
      }else if(curSnakeMenuSelection==1){
        runSnakeLevelSelect();
        break;
      }else if(curSnakeMenuSelection==2){
        runSnakeMazeSelect();
        break;
      }else if(curSnakeMenuSelection==3){
        runSnakeMusicSelect();
        break;
      }else if(curSnakeMenuSelection==4){
        runSnakeSfxSelect();
        break;
      }else if(curSnakeMenuSelection==5){
        runSnakeHighScore();
      }else{
        runSnakeDeleteScore();
      }
    }
    if(playSnake){
      runSnake(curSnakeLevelSelection,snakeMazeType);
      break; 
    }
    if(exitSnakeSelection){
      mainLoop();
      break;
    }
  }
}

void runSnakeLevelSelect(void){
  Serial2.write('Z');
  Serial2.flush();
  clearTablePixels();
  setSmallText((char*)"LEVEL", 5, 4, 1, AQUA);
  if(curSnakeLevelSelection==0){
    setSmallText((char*)"Easy",4,6,menuItemYval,WHITE);
  }else if(curSnakeLevelSelection==1){
    setSmallText((char*)"Medium",6,2,menuItemYval,WHITE);
  }else if(curSnakeLevelSelection==2){
    setSmallText((char*)"Hard",4,6,menuItemYval,WHITE);
  }
  showPixels();

  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"LEVEL", 5, 4, 1, AQUA);
    
    boolean snakeLevelSelectionChanged = false;
    //boolean runSnakeLevelSelection = false;
    boolean exitSnakeLevelSelection = false;
    boolean runStartSnake = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      snakeLevelSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_UP){
        curSnakeLevelSelection--;
        if (curSnakeLevelSelection<=SNAKEMINLEVELSELECTION){
          curSnakeLevelSelection = 0;
        }
        snakeLevelSelectionChanged = true;
      } else if (curControl == BTN_DOWN){
        curSnakeLevelSelection++;
        if (curSnakeLevelSelection>SNAKEMAXLEVELSELECTION){
          curSnakeLevelSelection = SNAKEMAXLEVELSELECTION;
        }
        snakeLevelSelectionChanged = true;
      } else if (curControl == BTN_START){
        runStartSnake = true;
      } else if (curControl == BTN_SELECT || curControl == BTN_A || curControl == BTN_B){
        exitSnakeLevelSelection = true;
      }
    }

  
    if (snakeLevelSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-6,AQUA);
      }else{
        if(curSnakeLevelSelection!=2){
          setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        }
        if(curSnakeLevelSelection!=0){
          setSmallText((char*)"^",1,12,menuItemYval-6,AQUA);
        }
      }
      if(curSnakeLevelSelection==0){
        setSmallText((char*)"Easy",4,6,menuItemYval,WHITE);
      }else if(curSnakeLevelSelection==1){
        setSmallText((char*)"Medium",6,2,menuItemYval,WHITE);
      }else if(curSnakeLevelSelection==2){
        setSmallText((char*)"Hard",4,6,menuItemYval,WHITE);
      }
      showPixels();
    }
    if (runStartSnake) {
      runSnake(curSnakeLevelSelection,snakeMazeType);
      break;
    }
    if (exitSnakeLevelSelection){
      runSnakeMenu();
      break;
    }
  }
}

void runSnakeMazeSelect(void){
  Serial2.write('Z');
  Serial2.flush();
  clearTablePixels();
  setSmallText((char*)"MAZE", 4, 6, 1, AQUA);
  if(snakeMazeType==0){
    setSmallText((char*)"None", 4, 6, menuItemYval, WHITE);//on
  }else if(snakeMazeType==1){
    setSmallText((char*)"Tunnel", 6, 2, menuItemYval, WHITE);//off
  }else if(snakeMazeType==2){
    setSmallText((char*)"Box", 3, 8, menuItemYval, WHITE);//off
  }
  showPixels();

  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"MAZE", 4, 6, 1, AQUA);
    boolean snakeMazeSelectionChanged = false;
    boolean exitSnakeMazeSelection = false;
    boolean runStartSnake = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      snakeMazeSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        snakeMazeType++;
        if (snakeMazeType>2){
          snakeMazeType = 2;
        }
        snakeMazeSelectionChanged = true;
      }else if (curControl == BTN_UP){
        snakeMazeType--;
        if (snakeMazeType<0){
          snakeMazeType = 0;
        }
        snakeMazeSelectionChanged = true;
      } else if (curControl == BTN_A || curControl == BTN_B || curControl == BTN_SELECT){
        exitSnakeMazeSelection = true;
      } else if (curControl == BTN_START){
        runStartSnake = true;
      } 
    }

    if (snakeMazeSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        if(snakeMazeType!=2){
          setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        }
        if(snakeMazeType!=0){
          setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
        }
      }
      if(snakeMazeType==0){
        setSmallText((char*)"None", 4, 6, menuItemYval, WHITE);
      }else if(snakeMazeType==1){
        setSmallText((char*)"Tunnel", 6, 2, menuItemYval, WHITE);
      }else if(snakeMazeType==2){
        setSmallText((char*)"Box", 3, 8, menuItemYval, WHITE);
      }
      showPixels();
    }
    
    if (runStartSnake) {
      runSnake(curSnakeLevelSelection,snakeMazeType);
      break;
    }
    
    if (exitSnakeMazeSelection){
      runSnakeMenu();
      break;
    }
  }
}

void runSnakeMusicSelect(void){
  clearTablePixels();
  setSmallText((char*)"MUSIC", 5, 4, 1, AQUA);
  if(snakeMusicON==1){
    if(curSnakeLevelSelection==0){
      Serial2.write('Y');
      Serial2.flush();
    }else if(curSnakeLevelSelection==1){
      Serial2.write('X');
      Serial2.flush();
    }else if(curSnakeLevelSelection==2){
      Serial2.write('W');
      Serial2.flush();
    }
    Serial2.write('$');
    Serial2.flush();
    Serial2.write('V');
    Serial2.flush();
    
    setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
  }else if(snakeMusicON==0){
    Serial2.write('S');
    Serial2.flush();
    Serial2.write('#');
    Serial2.flush();
    setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
  }
  showPixels();

  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"MUSIC", 5, 4, 1, AQUA);
    boolean snakeMusicSelectionChanged = false;
    boolean exitSnakeMusicSelection = false;
    boolean runStartSnake = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      snakeMusicSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_UP){
        snakeMusicON++;
        if (snakeMusicON>1){
          snakeMusicON = 0;
        }
        snakeMusicSelectionChanged = true;
      } else if (curControl == BTN_A || curControl == BTN_B || curControl == BTN_SELECT){
        exitSnakeMusicSelection = true;
      } else if (curControl == BTN_START){
        runStartSnake = true;
      } 
    }

    if (snakeMusicSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
      }

      if(snakeMusicON==1){
        Serial2.write('$');
        Serial2.flush();
        Serial2.write('V');
        Serial2.flush();
        if(curSnakeLevelSelection==0){
          Serial2.write('Y');
          Serial2.flush();
        }else if(curSnakeLevelSelection==1){
          Serial2.write('X');
          Serial2.flush();
        }else if(curSnakeLevelSelection==2){
          Serial2.write('W');
          Serial2.flush();
        }
        setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
      }else if(snakeMusicON==0){
        Serial2.write('#');
        Serial2.flush();
        Serial2.write('S');
        Serial2.flush();
        setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
      }
      showPixels();
    }
    
    if (runStartSnake) {
      runSnake(curSnakeLevelSelection,snakeMazeType);
    }

    if (exitSnakeMusicSelection){
      runSnakeMenu();
      break;
    }
    prevSnakeMusicON = snakeMusicON;
  }

}

void runSnakeSfxSelect(void){
  clearTablePixels();
  setSmallText((char*)"SFX", 3, 8, 1, AQUA);

  if(snakeSfxON==1){
    setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
  }else if(snakeSfxON==0){
    setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
  }
  showPixels();
  
  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setSmallText((char*)"SFX", 3, 8, 1, AQUA);
    boolean snakeSfxSelectionChanged = false;
    boolean exitSnakeSfxSelection = false;
    boolean runSfxStartSnake = false;
    currentMenuMillis = millis();
    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      snakeSfxSelectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN || curControl == BTN_UP){
        snakeSfxON++;
        if (snakeSfxON>1){
          snakeSfxON = 0;
        }
        snakeSfxSelectionChanged = true;
      } else if (curControl == BTN_A || BTN_B || curControl == BTN_SELECT){
        exitSnakeSfxSelection = true;
      } else if (curControl == BTN_START){
        runSfxStartSnake = true;
      } 
    }
  
    if (snakeSfxSelectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
      }
      if(snakeSfxON==0){
        Serial2.write('2');
        Serial2.flush();
        setSmallText((char*)"OFF", 3, 8, menuItemYval, WHITE);//off
      }else if(snakeSfxON==1){
        Serial2.write('3');
        Serial2.flush();
        Serial2.write('a');
        Serial2.flush();
        setSmallText((char*)"ON", 2, 10, menuItemYval, WHITE);//on
      }
      showPixels();
    }

    if (runSfxStartSnake) {
      runSnake(curSnakeLevelSelection,snakeMazeType);
    }
    
    if (exitSnakeSfxSelection){
      runSnakeMenu();
      break;
    }
  }
}
