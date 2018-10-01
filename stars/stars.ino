#include <Sparki.h>  // include the robot library
#include <math.h>
#include "pitches.h"

#define PI 3.1415926535897932384626433832795
#define DEG_TO_RAD 0.017453292519943295769236907684886

void setup() {
  sparki.servo(SERVO_CENTER); // center the servo
  sparki.clearLCD();
}

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

bool program = false;
bool drawing = false;
double a = 10;
// notes in the melody:
int melody[] = { NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4 };
 
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void loop() {
  sparki.clearLCD();
  //Scan for IR Receiver
  int code = sparki.readIR();
  int distance = sparki.ping();
  String strDist = String("Distance: ") + distance;
  char chrDist[20];
  strDist.toCharArray(chrDist, 20);
  sparki.drawString(0, 1, chrDist);
  if (distance < 15 && distance >= 0) {
    sparki.drawString(0,2, (char *) "Hello Ilya!");
  }
  if (!program) {    
    switch(code){
      case 70: sparki.moveForward(); break;
      case 21: sparki.moveBackward(); break;
      case 67: 
      case 71: sparki.moveRight(); break;
      case 68: 
      case 69: sparki.moveLeft(); break;
      case 64: sparki.moveStop();
               sparki.gripperStop(); 
               break;      
  
      // Gripper Buttons
      case 9:  sparki.gripperOpen(); break;
      case 7:  sparki.gripperClose(); break;
  
      // buzzer
      case 74: sparki.beep(); break;
  
      // Servo
      case 90: sparki.servo(SERVO_LEFT); break;
      case 28: sparki.servo(SERVO_CENTER); break;
      case 8: sparki.servo(SERVO_RIGHT); break;
  
      // RGB LED
      case 25: sparki.RGB(RGB_OFF); break;
      case 12: sparki.RGB(RGB_RED); break;
      case 24: sparki.RGB(RGB_GREEN); break;
      case 94: sparki.RGB(RGB_BLUE); break;
  
      // Program Control
      case 13:
        sparki.moveStop();
        sparki.RGB(0,0,0);
        sparki.beep(NOTE_A4, 500);
        program = true;
        break;
    }
  } else if (!drawing) {
    if (code == 22) {
      sparki.beep(NOTE_F4, 500);
      program = false;
      return;
    }
    int rays = 0;
    switch(code){
      case 25: rays = 3; break;
      case 12: rays = 4; break;
      case 24: rays = 5; break;
      case 94: rays = 6; break;
      case 8: rays = 7; break;
      case 28: rays = 8; break;
      case 90: rays = 9; break;
      case 66: rays = 10; break;
      case 82: rays = 11; break;
      case 74: rays = 12; break;
      default: rays = 0; break;
    }
    if (rays > 0) {
      drawing = true;
      sparki.beep(NOTE_C4, 500);
      String strRays = String("Rays: ") + rays;
      char chrRays[20];
      strRays.toCharArray(chrRays, 20);
      sparki.drawString(0, 0, chrRays);
      sparki.updateLCD();
      double alpha = PI / (double)rays;
      double sina = sin(alpha);
      double cosa = cos(alpha);
      double b = a * sqrt(10.0 - 6.0 * cosa) / 3.0;
      double alpha1Deg = RAD_TO_DEG * (PI - 2.0 * atan(3.0 * sina / (3.0 * cosa - 1.0)));
      double alpha2Deg = RAD_TO_DEG * (PI - 2.0 * atan(sina / (3.0 - cosa)));
      for (int i = 0; i < rays; i++) {
        sparki.moveForward(b);
        sparki.moveLeft(alpha1Deg);
        sparki.moveForward(b);
        sparki.moveRight(alpha2Deg);
      }
      // play each note in the arrays
      for (int thisNote = 0; thisNote < 8; thisNote++) {
     
        // calculate the note duration as 1 second divided by note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000/noteDurations[thisNote];
        sparki.beep(melody[thisNote],noteDuration);
     
        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        sparki.noBeep();
      }
      delay(1000);
      
      while (sparki.readIR() != -1); // Clear IR buffer
      drawing = false;
      rays = 0;
    }
  }
  sparki.updateLCD();
  delay(100);
}
