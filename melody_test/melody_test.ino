#include <sparki.h>
#include "pitches.h"

void playMusicAndDance(int number);


// music
const PROGMEM uint16_t melodies[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  NOTE_C5, NOTE_C5, NOTE_F4, NOTE_D4, 0, NOTE_D4, NOTE_G4, NOTE_D4, NOTE_C4, NOTE_D4, 0, NOTE_A4, NOTE_D4, 0, NOTE_D4, NOTE_AS4, 0,0,
  NOTE_A4, NOTE_F4, NOTE_D4, NOTE_A4, NOTE_D5, NOTE_D4, NOTE_C4, 0, NOTE_C4, NOTE_A3, NOTE_E4, NOTE_D4, 0, 0, 0, 0, 0, 0,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5, NOTE_GS4, NOTE_F4, NOTE_C5, NOTE_A4,
  NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, 0, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  NOTE_DS5, NOTE_CS5, NOTE_B4, NOTE_GS4, 0, NOTE_DS5, NOTE_CS5, NOTE_B4, NOTE_CS5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  NOTE_E4, NOTE_D4, NOTE_C4, NOTE_A3, 0, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_F4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_D5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_D5,
  NOTE_E5, NOTE_A5, NOTE_G5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_D5, NOTE_G5, NOTE_FS5,NOTE_E5, NOTE_D5, NOTE_E5, NOTE_E5,       0, NOTE_G5, NOTE_FS5,NOTE_E5, NOTE_D5, 
  NOTE_E5, NOTE_E5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  NOTE_D5,NOTE_DS5, NOTE_D5,NOTE_DS5, NOTE_D5, NOTE_C5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  NOTE_D5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_F4, NOTE_D4, NOTE_D4, 0, 0,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_E4, 0, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_D4, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const PROGMEM uint8_t noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  8, 8, 6, 16, 16, 16, 8, 8, 8, 8, 8, 6, 16, 16, 16, 8, 0, 0,
  8, 8, 8, 8, 8, 16, 16, 16, 16, 8, 8, 2, 2, 0, 0, 0, 0, 0,
  4, 4, 4, 8, 8, 4, 8, 8, 2, 4, 4, 4, 8, 8, 4, 8, 8, 2,
  8, 8, 4, 2, 2, 8, 8, 4, 2, 4, 2, 2, 0, 0, 0, 0, 0, 0,
  4, 4, 2, 1, 1, 4, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  4, 4, 2, 1, 1, 4, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  4, 4, 4, 8, 8, 4, 4, 2, 4, 4, 4, 8, 8, 8, 4, 8, 2, 4,
  8, 8, 8, 4, 8, 4, 8, 8, 2, 4, 4, 4, 8, 8, 8, 4, 8, 3,
  8, 8, 8, 8, 4, 8, 8, 4, 4, 8, 8, 8, 8, 4, 8, 8, 8, 8,
  4, 4, 4, 8, 8, 4, 4, 8, 8, 4, 4, 4, 4, 2, 8, 8, 4, 4,
  4, 8, 8, 4, 4, 8, 8, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2,
  4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  4, 4, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  8, 8, 8, 4, 8, 8, 3, 3, 8, 8, 8, 4, 8, 4, 4, 2, 0, 0,
  8, 8, 8, 2, 2, 8, 8, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0
  
};
const PROGMEM uint8_t dances[] = {
  0, 1, 2, 3, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 2, 3, 4, 2, 1, 3, 2, 1, 1, 4, 4, 2, 2, 1, 0, 0,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 0, 0,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3,
  0, 1, 2, 3, 4, 2, 1, 4, 4, 3, 0, 2, 2, 3, 2, 1, 2, 3
};

const PROGMEM uint8_t sizes[] = {16, 16, 14, 18, 9, 5, 5, 36, 0, 56, 0, 0, 0, 6, 16, 9};


int melodyNumb = 0;


void setup()
{
  sparki.servo(0);
}

void loop()
{
  playMusicAndDance(melodyNumb);
  melodyNumb = (melodyNumb + 1) % 16;
  delay( 5000 );
}

void playMusicAndDance(int number){
  uint8_t size = (uint8_t) pgm_read_byte_near(sizes + number);
  // play each note in the arrays
  for (uint8_t i = 0; i < size; i++) {
    // calculate the note duration as 1 second divided by note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    uint8_t parts = (uint8_t) pgm_read_byte_near(noteDurations + number * 18 + i);
    int duration = 1000 / (parts==0 ? 32 : parts);
    sparki.beep((uint16_t)pgm_read_word_near(melodies + number * 18 + i), duration);
 
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration + 100;
    switch((uint8_t) pgm_read_byte_near(dances + number * 18 + i)) {
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

