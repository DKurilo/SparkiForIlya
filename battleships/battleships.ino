/*******
 * 
 * Author: Dima Kurilo (dkurilo@gmail.com)
 * 
 * This program intentionally as simple as it possible. Because it for my son.
 * If you want classes and other things, check it 3-4 years later.
 * My son will be able to understand more complex things and we will make this game's code more modern.
 * Maybe 6-8 years later you find Haskell implementation here. But it will not be for Arduino.
 * 
 ******/

#define MYDEBUG

#include "sparki.h"
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

/*
 * we need more dynamic memory. It's only one reason why we have one sea
 * Each cell:
 * 00     000000
 * enemy  Sparki's
 * Check: getMySea, setMySea and getEnemySea, setEnemySea functions
 */
int8_t sea[10][10];
String strBuff;
char chrBuff[20];

void play();
int getCodeFromIR();
int getNumberFromIR();
int8_t getMySea(int x, int y);
int8_t getEnemySea(int x, int y);
void setMySea(int x, int y, int8_t code);
void setEnemySea(int x, int y, int8_t code);
void initSeas();
int8_t getSizeFromCode(int8_t code);
bool checkIfFit(int x, int y, int orientation, int size);
void setUpShip(int x, int y, int orientation, int size, int8_t code);
int checkHit(int x, int y);
bool generateHit(int *x, int *y, int lastHitX, int lastHitY);
int markAnswerOnMap(int x, int y, int answer);
void playMusicAndDance(int8_t *notes, int8_t *durations, int8_t *dance);

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0)*1000);
  sparki.servo(SERVO_CENTER); // center the servo
  sparki.clearLCD();
  strBuff = String("Ilya let's play!");
  strBuff.toCharArray(chrBuff, 20);
  sparki.drawString(0, 0, chrBuff);
  sparki.updateLCD();
  sparki.servo(0);
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (sparki.readIR() != 64) {
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
    sparki.moveStop();
    play();
  }
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
        do {
          sparki.clearLCD();
          strBuff = String("Your move:");
          strBuff.toCharArray(chrBuff, 20);
          sparki.drawString(0, 0, chrBuff);
          sparki.updateLCD();
          x = getNumberFromIR();
          strBuff = x;
          strBuff.toCharArray(chrBuff, 20);
          sparki.drawString(0, 1, chrBuff);
          sparki.updateLCD();
          y = getNumberFromIR();
          strBuff = y;
          strBuff.toCharArray(chrBuff, 20);
          sparki.drawString(20, 1, chrBuff);
          sparki.updateLCD();
          delay(1000);
          state = checkHit(x,y);
        } while(state == 0);
        break;
      case 1:
        if (generateHit(&x, &y, lastHitX, lastHitY)) {
          sparki.clearLCD();
          strBuff = String("My shot: ");
          strBuff += String(x);
          strBuff += String(" ");
          strBuff += String(y);
          strBuff.toCharArray(chrBuff, 20);
          sparki.drawString(0, 0, chrBuff);
          sparki.updateLCD();
          bool done = false;
          while(!done) {
            int code = getCodeFromIR();
            switch(code) {
              case 68: // miss
                state = markAnswerOnMap(x, y, 0);
                sparki.clearLCD();
                strBuff = String("Oops!");
                strBuff.toCharArray(chrBuff, 20);
                sparki.drawString(0, 0, chrBuff);
                sparki.updateLCD();
                delay(1000);
                done = true;
                break;
              case 64: // hit
                state = markAnswerOnMap(x, y, 1);
                lastHitX = x;
                lastHitY = y;
                sparki.clearLCD();
                strBuff = String("Wow!");
                strBuff.toCharArray(chrBuff, 20);
                sparki.drawString(0, 0, chrBuff);
                sparki.updateLCD();
                delay(1000);
                done = true;
                break;
              case 67: // sink
                state = markAnswerOnMap(x, y, 2);
                lastHitX = -1;
                lastHitY = -1;
                sparki.clearLCD();
                strBuff = String("Oh yeah!!!");
                strBuff.toCharArray(chrBuff, 20);
                sparki.drawString(0, 0, chrBuff);
                sparki.updateLCD();
                delay(1000);
                done = true;
                break;
            }
          }
        } else {
          state = 22;
        }
        break;
    }
  }
  
  switch(state) {
    case 20:
      sparki.clearLCD();
      strBuff = String("You win!");
      strBuff.toCharArray(chrBuff, 20);
      sparki.drawString(0, 0, chrBuff);
      sparki.updateLCD();
      delay(3000);
      break;
    case 21:
      sparki.clearLCD();
      strBuff = String("I win!");
      strBuff.toCharArray(chrBuff, 20);
      sparki.drawString(0, 0, chrBuff);
      sparki.updateLCD();
      delay(3000);
      break;
    case 22:
      sparki.clearLCD();
      strBuff = String("You're cheating!");
      strBuff.toCharArray(chrBuff, 20);
      sparki.drawString(0, 0, chrBuff);
      sparki.updateLCD();
      delay(3000);
      break;
  }
  sparki.clearLCD();
  strBuff = String("Ilya let's play!");
  strBuff.toCharArray(chrBuff, 20);
  sparki.drawString(0, 0, chrBuff);
  sparki.updateLCD();
}

int getCodeFromIR(){
  int code = -1;
  while(code == -1) {
    code = sparki.readIR();
    delay(100);
  }
  delay(100);
  while (sparki.readIR() != -1) {
    delay(100);
  }; // Clear IR buffer
  return code;
}

int getNumberFromIR() {
  while(true) {
    switch (getCodeFromIR()) {
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
        break;
    }
  }
  return -1;
}

void initSeas() {
  for(int i = 0; i < 10; i++) {
    for(int k = 0; k < 10; k++) {
      sea[i][k] = 0;
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
  for (int8_t j = 19; j > 9; j--) {
    bool setup = false;
    int8_t size = getSizeFromCode(j);
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

int8_t getSizeFromCode(int8_t code) {
  int8_t size = -1;
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

bool checkIfFit(int x, int y, int orientation, int8_t size) {
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
        if (getMySea(currentX, currentY) != 0) {
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

void setUpShip(int x, int y, int orientation, int size, int8_t code) {
  for (int i = 0; i < size; i++) {
    if (orientation == 0) {
      setMySea(x + i, y, code);
    } else {
      setMySea(x, y + i, code);
    }
  }
}

int checkHit(int x, int y) {
  int state = 20; // lets be pessemistic...
  int8_t code = getMySea(x, y);
  if (code > 9 && code < 20) { // player hit. need to know if ship sank
    setMySea(x, y, code + 20);
  } else { // player missed
    // Sparki should celebrate it.
    sparki.clearLCD();
    strBuff = String("Miss!");
    strBuff.toCharArray(chrBuff, 20);
    sparki.drawString(0, 0, chrBuff);
    sparki.updateLCD();
    delay(2000);
    return 1;
  }
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++) {
      if (getMySea(i, j) == code) { // player hit, but ship is still alive!
        // Sparki is sad.
        sparki.clearLCD();
        strBuff = String("Hit!");
        strBuff.toCharArray(chrBuff, 20);
        sparki.drawString(0, 0, chrBuff);
        sparki.updateLCD();
        delay(2000);
        return 0;
      } else if (getMySea(i, j) > 9 && getMySea(i, j) < 20) { // hit, but we have more ships!
        state = 0;
      }
    }
  }
  if (state == 0) { // sink.. But Sparki have ship or more!
    // Sparki is sad.
    sparki.clearLCD();
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
    int8_t cells[100], count = 0;
    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        if (getEnemySea(i, j) == 0) {
          cells[count] = i * 10 + j;
          count++;
        }
      }
    }
    if (count > 0 ) {
      int hit = random(count);
      *x = cells[hit] / 10;
      *y = cells[hit] % 10;
    } else {
      fail = true;
    }
  } else {
    int direction = random(4), orientation = -1, decks = 0;
    if ((lastHitX > 0 && getEnemySea(lastHitX - 1, lastHitY) == 2) || (lastHitX < 9 && getEnemySea(lastHitX + 1, lastHitY) == 2)) {
      orientation = 0;
      direction = 0;
    } else if ((lastHitY > 0 && getEnemySea(lastHitX, lastHitY - 1) == 2) || (lastHitY < 9 && getEnemySea(lastHitX, lastHitY + 1) == 2)) {
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
              if (getEnemySea(*x, *y) == 0) {
                done = true;
              } else if (getEnemySea(*x, *y) == 2) {
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
              if (getEnemySea(*x, *y) == 0) {
                done = true;
              } else if (getEnemySea(*x, *y) == 2) {
                if (orientation == -1 || orientation == 0) {
                  decks++;
                }
              }
            }
            break;
          case 2:
            *x = lastHitX;
            *y = lastHitY - k;
            if (*y >= 0) {
              if (getEnemySea(*x, *y) == 0) {
                done = true;
              } else if (getEnemySea(*x, *y) == 2) {
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
              if (getEnemySea(*x, *y) == 0) {
                done = true;
              } else if (getEnemySea(*x, *y) == 2) {
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
  return !fail;
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
      setEnemySea(x, y, 1);
      state = 0;
      break;
    case 1:
      setEnemySea(x, y, 2);
      state = 1;
      for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++) {
          if (getEnemySea(i, j) == 2) {
            count++;
          }
        }
      }
      if (count >= 20) {
        state = 22;
      }
      break;
    case 2:
      setEnemySea(x, y, 2);
      state = 1;
      for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++) {
          if (getEnemySea(i, j) == 2) {
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
        if ((y > 0 && getEnemySea(x, y - 1) == 2) || (y < 0 && getEnemySea(x, y + 1) == 2)) {
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
            if (getEnemySea(x1, y1) == 2) {
              for (int j = -1; j < 2; j++){
                int x2 = x1 + j;
                if (x2 >= 0 && x2 < 10) {
                  for (int k = -1; k < 2; k++){
                    int y2 = y1 + k;
                    if (y2 >= 0 && y2 < 10 && getEnemySea(x2, y2) == 0) {
                      setEnemySea(x2, y2, 3);
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

  #ifdef MYDEBUG
  if (Serial1.available()) {
    Serial1.print("Coords: ");
    Serial1.print(x);
    Serial1.print(" ");
    Serial1.println(y);
    Serial1.print("Answer: ");
    Serial1.println(answer);
    Serial1.println("Enemy sea: ");
    for(int i = 0; i < 10; i++){
      for (int j = 0; j < 10; j++) {
        Serial1.print(getEnemySea(i, j));
        Serial1.print(" ");
      }
      Serial1.println(" ");
    }
    Serial1.println(" ");
  }
  #endif
  
  return state;
}

int8_t getMySea(int x, int y){
  return (int8_t) sea[x][y] & 63; // 0b00111111
}

int8_t getEnemySea(int x, int y){
  return (int8_t) sea[x][y] >> 6; // (0-3)
}

void setMySea(int x, int y, int8_t code){
  sea[x][y] = getEnemySea(x, y) << 6 + code;
}

void setEnemySea(int x, int y, int8_t code){
  sea[x][y] = code << 6 + getMySea(x, y);
}

void playMusicAndDance(int8_t *notes, int8_t *durations, int8_t *dance){
  return;
}

