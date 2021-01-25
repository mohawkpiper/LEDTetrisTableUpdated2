/* LedTable
 *
 * Written by: Klaas De Craemer, Ing. David Hrbaty - Modified by: Mark Quinn - Modified again to be like SNES classic by: Greg Beckman
 * 
 * Menu system for the LED table
 */


int curMainMenuSelection = 0;
//long previousMenuMillis = 0; 
//long menuInterval = 500;
//unsigned long currentMenuMillis;

int menuItemYval = 14;
//int cursorOn = 0;
int curTetrisMenuSelection = 0;
int curSnakeMenuSelection = 0;


//int TEXTSPEED = 30;

void OffScreen(void){
  Serial2.write('S');
  Serial2.flush();
  clearTablePixels();
  showPixels();
  while(true){
    boolean exitOffScreen = false;

    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        exitOffScreen = true;
      } else if (curControl == BTN_UP){
        exitOffScreen = true;
      } else if (curControl == BTN_LEFT){
        exitOffScreen = true;
      } else if (curControl == BTN_RIGHT){
        exitOffScreen = true;
      } else if (curControl == BTN_A){
        exitOffScreen = true;
      } else if (curControl == BTN_B){
        exitOffScreen = true;
      } else if (curControl == BTN_START){
        exitOffScreen = true;
      } else if (curControl == BTN_SELECT){
        exitOffScreen = true;
      }
    }
    //If we are here, it means a selection was changed
    if (exitOffScreen){//Start Menu
      mainLoop();
      break;   
    }
  }
}

void mainLoop(void){
  Serial3.write(uint8_t(5));// back to controller, says we are in menu mode
  
  curTetrisMenuSelection = 0;
  curSnakeMenuSelection = 0;
  Serial2.write('S');
  Serial2.flush();
  clearTablePixels();
  setPixelText((char*)"MENU", 4, 2, 1, AQUA);
  if(curMainMenuSelection==0){
    setSmallText((char*)"Tetris", 6, 2, menuItemYval, WHITE);//typeA
  }else if(curMainMenuSelection==1){
    setSmallText((char*)"Snake", 5, 4, menuItemYval, WHITE);//typeC
  }else if(curMainMenuSelection==2){
    setSmallText((char*)"Animate", 7, 0, menuItemYval, WHITE);//nes music 1
  }
  showPixels();

        
  while(true){
    //Show menu system and wait for input
    clearTablePixels();
    setPixelText((char*)"MENU", 4, 2, 1, AQUA);
    //setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
    
    boolean selectionChanged = false;
    boolean runSelection = false;
    currentMenuMillis = millis();

    
    if(currentMenuMillis - previousMenuMillis > menuInterval) {
      previousMenuMillis = currentMenuMillis;
      cursorOn++;
      if(cursorOn>1){
        cursorOn=0;
      }
      selectionChanged = true;
    }
    readInput();
    if (curControl != BTN_NONE){
      if (curControl == BTN_DOWN){
        curMainMenuSelection++;
        if (curMainMenuSelection>2){
          curMainMenuSelection = 2;
        }
        selectionChanged = true;
      }
      if (curControl == BTN_UP){
        curMainMenuSelection--;
        if (curMainMenuSelection<0){
          curMainMenuSelection = 0;
        }
        selectionChanged = true;
      }
      if (curControl == BTN_A || curControl == BTN_START){
        runSelection = true;
      }
    }

  
    if (selectionChanged){
      if(cursorOn==0){
        setSmallText((char*)" ",1,12,menuItemYval+4,AQUA);
        setSmallText((char*)" ",1,12,menuItemYval-4,AQUA);
      }else{
        if(curMainMenuSelection!=2){
          setSmallText((char*)"v",1,12,menuItemYval+4,AQUA);
        }
        if(curMainMenuSelection!=0){
          setSmallText((char*)"^",1,12,menuItemYval-4,AQUA);
        }
      }
      if(curMainMenuSelection==0){
        setSmallText((char*)"Tetris", 6, 2, menuItemYval, WHITE);
      }else if(curMainMenuSelection==1){
        setSmallText((char*)"Snake", 5, 4, menuItemYval, WHITE);
      }else if(curMainMenuSelection==2){
        setSmallText((char*)"Animate", 7, 0, menuItemYval, WHITE);
      }
      
      showPixels();
    }
    
    if (runSelection) {
      if(curMainMenuSelection==0){
        runTetrisMenu();
        break;
      }else if(curMainMenuSelection==1){
        runSnakeMenu();
        //break;
      }else{
        runScreenSaver();
        //break;
      }
    }
  }
}
