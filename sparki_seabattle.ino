#include <Sparki.h>  // include the robot library
#include <math.h>
#include "pitches.h"

#define PI 3.1415926535897932384626433832795
#define DEG_TO_RAD 0.017453292519943295769236907684886

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

int mySea[10][10], enemySea[10][10];

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
void playMusicAndDance(int *notes, int *dance);

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
    switch(state) {
      case 0:
        int x = -1;
        int y = -1;
        do {
          x = getNumberFromIR();
          y = getNumberFromIR();
          state = checkHit(x,y);
        } while(state == 0);
        break;
      case 1:
        int x = -1, y = -1;
        if (generateHit(&x, &y, lastHitX, lastHitY)) {
          int code = getCodeFromIR();
          switch(code) {
            case 68: // miss
              state = markAnswerOnMap(x, y, 0);
              break;
            case 64: // hit
              state = markAnswerOnMap(x, y, 1);
              lastHitX = x;
              lastHitY = y;
              break;
            case 67: // sink
              state = markAnswerOnMap(x, y, 2);
              lastHitX = -1;
              lastHitY = -1;
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
      break;
    case 21:
      break;
    case 22:
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
    int size = getSizeFromCose(j);
    while(!setup) {
      int x = random(10), y = random(10), orientation = random(2); // orientation | 0         = horizontal 
                                                                   //             | otherwise = vertical
      if (checkifFit(x, y, orientation, size)) {
        setUpShip(x, y, orientation, size, j);
        done = true;
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
        return 1;
      } else if (mySea[i][j] == code) { // player hit
        // Sparki is sad.
        return 0;
      } else if (mySea[i][j] > 9 && mySea[i][j] < 20) { // hit, but we have more ships!
        state = 0;
      }
    }
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
    int cells[100], count = 0;
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
      x = hit / 10;
      y = hit % 10;
    } else {
      fail = true;
    }
  } else {
    int direction = random(4);
    bool done = false;
    for (int i = 0; i < 3 && done == false; i++){
      for (int k = 1; k < 4 && done == false; k++){
        switch(direction) {
          case 0:
            x = lastHitX - k;
            y = lastHitY;
            if (x > 0 && enemySea[x][y] == 0) {
              done = true;
            }
            break;
          case 1:
            x = lastHitX + k;
            y = lastHitY;
            if (x < 10 && enemySea[x][y] == 0) {
              done = true;
            }
            break;
          case 2:
            x = lastHitX;
            y = lastHitY - k;
            if (y > 0 && enemySea[x][y] == 0) {
              done = true;
            }
            break;
          case 3:
          default:
            x = lastHitX;
            y = lastHitY + k;
            if (y < 10 && enemySea[x][y] == 0) {
              done = true;
            }
            break;
        }
      }
      direction = (direction + 1) % 4;
    }
    if (done == false) {
      fail = true;
    }
  }
  return fail;
}

int markAnswerOnMap(int x, int y, int answer){
  return 21;
}

void playMusicAndDance(int *notes, int *dance){
  return;
}

