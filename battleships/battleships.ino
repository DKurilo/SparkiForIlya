#include "mySparki.h"  // include the robot library. It's mmodified to use less memory
#include <math.h>
#include "pitches.h"

// /------^-----\
// |            |
// | 69  70  71 |
// | 68  64  67 |
// |  7  21   9 |
// | 22  25  13 |
// | 12  24  94 |
// |  8  28  90 |
// | 66  82  74 |
// \____________/

  /***
   * mySea | empty                   =  0
   *       | empty, but near to ship =  1
   *       | one-decker 1            = 10
   *       | one-decker 2            = 11
   *       | one-decker 3            = 12
   *       | one-decker 4            = 13
   *       | two-decker 1            = 14
   *       | two-decker 2            = 15
   *       | two-decker 3            = 16
   *       | three-decker 1          = 17
   *       | three-decker 2          = 18
   *       | four-decker 1           = 19
   *       | hit code                = 20 + code
   *       | miss!                   = 50
   *
   * enemySea | empty                =  0
   *          | miss                 =  1
   *          | hit                  =  2
   *          | near to ship         =  3
   */
uint8_t mySea[10][10], enemySea[10][10];
String strBuff;
char chrBuff[20];

void play();
int getCodeFromIR();
int getNumberFromIR();
void initSeas();
int getSizeFromCode(int code);
bool checkIfFit(int x, int y, int orientation, int size);
void setUpShip(int x, int y, int orientation, int size, int code);
int checkHit(int x, int y);
bool generateHit(int *x, int *y, int lastHitX, int lastHitY);
int markAnswerOnMap(int x, int y, int answer);
void playMusicAndDance(uint8_t *notes, uint8_t *durations, uint8_t *dance);

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  sparki.servo(SERVO_CENTER); // center the servo
  sparki.clearLCD();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (getCodeFromIR() != 64) {
    if (random(2) >= 1) {
      int move = random(5);
      switch(move) {
        case 0:
          sparki.moveLeft();
          break;
        case 1:
          sparki.moveRight();
          break;
        case 2:
          sparki.moveForward();
          break;
        case 3:
          sparki.moveBackward();
          break;
        case 4:
          sparki.moveStop();
          break;
      }
    }
  } else {
    play();
  }
  sparki.updateLCD();
  delay(100);
}

void play() {
  initSeas();
  /************
   * state:
   * 0 - player's move
   * 1 - Sparki's move
   * 20 - player won!
   * 21 - Sparki won!
   * 22 - player cheated!
   */
  int state = 0;
  int lastHitX = -1, lastHitY = -1;
  
  while(state < 20) {
    int x = -1, y = -1;
    switch(state) {
      case 0:
        strBuff = String("Your move:");
        strBuff.toCharArray(chrBuff, 20);
        sparki.drawString(0, 0, chrBuff);
        sparki.updateLCD();
        do {
          x = getNumberFromIR();
          strBuff = x;
          strBuff.toCharArray(chrBuff, 20);
          sparki.drawString(0, 1, chrBuff);
          sparki.updateLCD();
          y = getNumberFromIR();
          strBuff = x;
          strBuff.toCharArray(chrBuff, 20);
          sparki.drawString(2, 1, chrBuff);
          sparki.updateLCD();
          state = checkHit(x,y);
        } while(state == 0);
        break;
      case 1:
        if (generateHit(&x, &y, lastHitX, lastHitY)) {
          strBuff = String("I'm hitting... ") + x + String(" ") + y;
          strBuff.toCharArray(chrBuff, 20);
          sparki.drawString(0, 0, chrBuff);
          sparki.updateLCD();
          int code = getCodeFromIR();
          switch(code) {
            case 68: // miss
              state = markAnswerOnMap(x, y, 0);
              strBuff = String("Oops!");
              strBuff.toCharArray(chrBuff, 20);
              sparki.drawString(0, 0, chrBuff);
              sparki.updateLCD();
              delay(1000);
              break;
            case 64: // hit
              state = markAnswerOnMap(x, y, 1);
              lastHitX = x;
              lastHitY = y;
              strBuff = String("Wow!");
              strBuff.toCharArray(chrBuff, 20);
              sparki.drawString(0, 0, chrBuff);
              sparki.updateLCD();
              delay(1000);
              break;
            case 67: // sink
              state = markAnswerOnMap(x, y, 2);
              lastHitX = -1;
              lastHitY = -1;
              strBuff = String("Oh yeah!!!");
              strBuff.toCharArray(chrBuff, 20);
              sparki.drawString(0, 0, chrBuff);
              sparki.updateLCD();
              delay(1000);
              break;
          }
        } else {
          state = 22;
        }
        break;
    }
  }
  
  switch(state) {
    case 20:
      strBuff = String("You win!");
      strBuff.toCharArray(chrBuff, 20);
      sparki.drawString(0, 0, chrBuff);
      sparki.updateLCD();
      delay(3000);
      break;
    case 21:
      strBuff = String("I win!");
      strBuff.toCharArray(chrBuff, 20);
      sparki.drawString(0, 0, chrBuff);
      sparki.updateLCD();
      delay(3000);
      break;
    case 22:
      strBuff = String("You're cheating!");
      strBuff.toCharArray(chrBuff, 20);
      sparki.drawString(0, 0, chrBuff);
      sparki.updateLCD();
      delay(3000);
      break;
  }
}

int getCodeFromIR(){
  int code = sparki.readIR();
  delay(100);
  while (sparki.readIR() != -1) {
    delay(100);
  }; // Clear IR buffer
  return code;
}

int getNumberFromIR() {
  switch (getNumberFromIR()) {
    case 25:
      return 0;
      break;
    case 12:
      return 1;
      break;
    case 24:
      return 2;
      break;
    case 94:
      return 3;
      break;
    case 8:
      return 4;
      break;
    case 28:
      return 5;
      break;
    case 90:
      return 6;
      break;
    case 66:
      return 7;
      break;
    case 82:
      return 8;
      break;
    case 74:
      return 9;
      break;
    default:
      return -1;
      break;
  }
  return -1;
}

void initSeas() {
  for(int i = 0; i < 10; i++) {
    for(int k = 0; k < 10; k++) {
      mySea[i][k] = 0;
      enemySea[i][k] = 0;
    }
  }
  /***
   * mySea:
   * 0 - empty
   * 1 - empty, but near to ship
   * 10 - one-decker 1
   * 11 - one-decker 2
   * 12 - one-decker 3
   * 13 - one-decker 4
   * 14 - two-decker 1
   * 15 - two-decker 2
   * 16 - two-decker 3
   * 17 - three-decker 1
   * 18 - three-decker 2
   * 19 - four-decker
   * 50 - miss!
   * 30-39 - hit!
   */
  for (int j = 19; j > 9; j--) {
    bool setup = false;
    int size = getSizeFromCode(j);
    while(!setup) {
      int x = random(10), y = random(10), orientation = random(2); // orientation | 0         = horizontal 
                                                                   //             | otherwise = vertical
      if (checkIfFit(x, y, orientation, size)) {
        setUpShip(x, y, orientation, size, j);
        setup = true;
      }
    }
  }
  return;
}

int getSizeFromCode(int code) {
  int size = -1;
  if (code > 9 && code < 14) {
    size = 1;
  } else if (code < 17) {
    size = 2;
  } else if (code < 19) {
    size = 3;
  } else if (code < 20) {
    size = 4;
  }
  return size;
}

bool checkIfFit(int x, int y, int orientation, int size) {
  if (x < 0 || x > 9 || y < 0 || y > 9 || (orientation == 0 && (x + size) > 9) || (orientation != 0 && (y + size) > 9)) {
    return false;
  }
  bool fit = true;
  for (int i = 0; i < size; i++) {
    for (int j = -1; j < 2; j++){
      int currentX = x + j;
      if (orientation == 0) {
        currentX += i;
      }
      if (currentX < 0 || currentX > 9) {
        break;
      }
      for (int k = -1; k < 2; k++) {
        int currentY = y + k;
        if (orientation != 0) {
          currentY += i;
        }
        if (currentY < 0 || currentY > 9) {
          break;
        }
        if (mySea[currentX][currentY] != 0) {
          fit = false;
          break;
        }
      }
      if (!fit) {
        break;
      }
      if (!fit) {
        break;
      }
    }
  }
  return fit;
}

void setUpShip(int x, int y, int orientation, int size, int code) {
  for (int i = 0; i < size; i++) {
    if (orientation == 0) {
      mySea[x + i][y] = code;
    } else {
      mySea[x][y + i] = code;
    }
  }
}

int checkHit(int x, int y) {
  int state = 20; // lets be pessemistic...
  int code = mySea[x][y];
  if (code > 9 && code < 20) {
    mySea[x][y] = code + 20;
  }
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++) {
      if (mySea[i][j] < 10 || mySea[i][j] > 19) { // player missed
        // Sparki should celebrate it.
        strBuff = String("Miss!");
        strBuff.toCharArray(chrBuff, 20);
        sparki.drawString(0, 0, chrBuff);
        sparki.updateLCD();
        delay(2000);
        return 1;
      } else if (mySea[i][j] == code) { // player hit
        // Sparki is sad.
        strBuff = String("Hit!");
        strBuff.toCharArray(chrBuff, 20);
        sparki.drawString(0, 0, chrBuff);
        sparki.updateLCD();
        delay(2000);
        return 0;
      } else if (mySea[i][j] > 9 && mySea[i][j] < 20) { // hit, but we have more ships!
        state = 0;
      }
    }
  }
  if (state == 0) { // sink..
    // Sparki is sad.
    strBuff = String("Sink!");
    strBuff.toCharArray(chrBuff, 20);
    sparki.drawString(0, 0, chrBuff);
    sparki.updateLCD();
    delay(2000);
  }
  return state;
}

/****
 * return | can find where to hit = true
 *        | otherwise             = false
 */
bool generateHit(int *x, int *y, int lastHitX, int lastHitY) {
  bool fail = false;
  if (lastHitX < 0 || lastHitY < 0) {
    uint8_t cells[100], count = 0;
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        if (enemySea[i][j] == 0) {
          cells[count] = i * 10 + j;
          count++;
        }
      }
    }
    if (count > 0 ) {
      int hit = random(count);
      *x = hit / 10;
      *y = hit % 10;
    } else {
      fail = true;
    }
  } else {
    int direction = random(4), orientation = -1, decks = 0;
    if ((lastHitX > 0 && enemySea[lastHitX - 1][lastHitY] == 2) || (lastHitX < 9 && enemySea[lastHitX + 1][lastHitY] == 2)) {
      orientation = 0;
      direction = 0;
    } else if ((lastHitY > 0 && enemySea[lastHitX][lastHitY - 1] == 2) || (lastHitY < 9 && enemySea[lastHitX][lastHitY + 1] == 2)) {
      orientation = 1;
      direction = 2;
    }
    bool done = false;
    for (int i = 0; i < (orientation == -1 ? 4 : 2) && done == false; i++){
      for (int k = 1; k < 4 && done == false; k++){
        switch(direction) {
          case 0:
            *x = lastHitX - k;
            *y = lastHitY;
            if (*x >= 0) {
              if (enemySea[*x][*y] == 0) {
                done = true;
              } else if (enemySea[*x][*y] == 2) {
                if (orientation == -1 || orientation == 0) {
                  decks++;
                }
              }
            }
            break;
          case 1:
            *x = lastHitX + k;
            *y = lastHitY;
            if (*x < 10) {
              if (enemySea[*x][*y] == 0) {
                done = true;
              } else if (enemySea[*x][*y] == 2) {
                if (orientation == -1 || orientation == 0) {
                  decks++;
                }
              }
            }
            break;
          case 2:
            *x = lastHitX;
            *y = lastHitY - k;
            if (y >= 0) {
              if (enemySea[*x][*y] == 0) {
                done = true;
              } else if (enemySea[*x][*y] == 2) {
                if (orientation == -1 || orientation == 1) {
                  decks++;
                }
              }
            }            
            break;
          case 3:
          default:
            *x = lastHitX;
            *y = lastHitY + k;
            if (*y < 10) {
              if (enemySea[*x][*y] == 0) {
                done = true;
              } else if (enemySea[*x][*y] == 2) {
                if (orientation == -1 || orientation == 1) {
                  decks++;
                }
              }
            } 
            break;
        }
      }
      direction = (direction + 1) % 4;
    }
    if (done == false || decks > 4) {
      fail = true;
    }
  }
  return fail;
}

/****
 * answer | miss = 0
 *        | hit  = 1
 *        | sink = 2
 */
int markAnswerOnMap(int x, int y, int answer){
  int state = 21;
  int count = 0;
  switch (answer) {
    case 0:
      enemySea[x][y] = 1;
      state = 0;
      break;
    case 1:
      enemySea[x][y] = 2;
      state = 1;
      for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++) {
          if (enemySea[i][j] == 2) {
            count++;
          }
        }
      }
      if (count >= 20) {
        state = 22;
      }
      break;
    case 3:
      enemySea[x][y] = 2;
      state = 1;
      for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++) {
          if (enemySea[i][j] == 2) {
            count++;
          }
        }
      }
      if (count == 20) {
        state = 21;
      } else if (count > 20) {
        state = 22;
      } else {
        int orientation = 0;
        if ((y > 0 && enemySea[x][y - 1] == 2) || (y < 0 && enemySea[x][y + 1] == 2)) {
          orientation = 1;
        }
        for (int i = -3; i < 4; i++) {
          int x1 = x, y1 = y;
          if (orientation == 0) {
            x1 += i;
          } else {
            y1 += i;
          }
          if (x1 >= 0 && x1 < 10 && y1 >= 0 && y1 < 10) {
            if (enemySea[x1][y1] == 2) {
              for (int j = -1; j < 2; j++){
                int x2 = x1 + j;
                if (x2 >= 0 && x2 < 10) {
                  for (int k = -1; k < 2; k++){
                    int y2 = y1 + k;
                    if (y2 >= 0 && y2 < 10 && enemySea[x2][y2] == 0) {
                      enemySea[x2][y2] = 3;
                    }
                  }
                }
              }
            }
          }
        }
      }
      break;
    default:
      state = 22;
      break;
  }
  return state;
}

void playMusicAndDance(uint8_t *notes, uint8_t *durations, uint8_t *dance){
  return;
}

