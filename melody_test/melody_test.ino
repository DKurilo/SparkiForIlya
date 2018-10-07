#include <sparki.h>
#include "pitches.h"

void playMusicAndDance(int *notes, int *durations, int *dance, int size);

int melody[][18] = {
  {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {NOTE_D4, 0, NOTE_F4, NOTE_D4, 0, NOTE_D4, NOTE_G4, NOTE_D4, NOTE_C4, NOTE_D4, 0, NOTE_A4, NOTE_D4, 0, NOTE_D4, NOTE_AS4, 0,0},
  {NOTE_A4, NOTE_F4, NOTE_D4, NOTE_A4, NOTE_D5, NOTE_D4, NOTE_C4, 0, NOTE_C4, NOTE_A3, NOTE_E4, NOTE_D4, 0, 0, 0, 0, 0, 0},
  {NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5, NOTE_GS4, NOTE_F4, NOTE_C5, NOTE_A4}
};
int noteDurations[][18] = {
  { 4, 8, 8, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {8, 8, 6, 16, 16, 16, 8, 8, 8, 8, 8, 6, 16, 16, 16, 8, 0, 0},
  {8, 8, 8, 8, 8, 16, 16, 16, 16, 8, 8, 2, 2, 0, 0, 0, 0, 0},
  {4, 4, 4, 8, 8, 4, 8, 8, 2, 4, 4, 4, 8, 8, 4, 8, 8, 2}
};
int dances[][18] = {
  { 0, 1, 2, 3, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 2, 3, 4, 2, 1, 3, 2, 1, 1, 4, 4, 2, 2, 1, 0, 0 },
  { 0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 0, 0 },
  { 0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3 }
};
int sizes[] = {16, 16, 14, 18};

int melodyNumb = 3;


void setup()
{
  sparki.servo(0);
}

void loop()
{
  playMusicAndDance(melody[melodyNumb], noteDurations[melodyNumb], dances[melodyNumb], sizes[melodyNumb]);
  //melodyNumb = (melodyNumb + 1) % 3;
  delay( 5000 );
}

void playMusicAndDance(int *notes, int *durations, int *dance, int size){
  // play each note in the arrays
  for (int i = 0; i < size; i++) {
    // calculate the note duration as 1 second divided by note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000/(durations[i]==0?32:durations[i]);
    sparki.beep(notes[i], duration);
 
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    switch(dance[i]) {
      case 0:
        sparki.moveStop();
        break;
      case 1:
        sparki.moveForward();
        break;
      case 2:
        sparki.moveBackward();
        break;
      case 3:
        sparki.moveRight();
        break;
      case 4:
        sparki.moveLeft();
        break;
    }
    delay(pauseBetweenNotes);
    // stop the tone playing:
    sparki.noBeep();
  }
  sparki.moveStop();
  return;
}
