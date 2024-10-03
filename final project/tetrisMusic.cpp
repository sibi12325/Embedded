#include <Arduino.h>
#include "tetrisMusic.h"
#include "pitches.h"

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int tetrisMelody[] = {

  //Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192
  
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,

  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  

  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  REST,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,

};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int tetrisNotes=sizeof(tetrisMelody)/sizeof(tetrisMelody[0])/2; 

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, tetrisNoteDuration = 0;

int subwayMelody[] = {
  NOTE_C4, REST, NOTE_G4, REST, NOTE_AS4, NOTE_C5, NOTE_AS4, REST, NOTE_F4, NOTE_DS4, REST,
  NOTE_C4, REST, NOTE_G4, REST, NOTE_AS4, NOTE_C5, NOTE_AS4, REST, NOTE_F4, NOTE_DS4, REST,
  NOTE_C4, REST, NOTE_G4, REST, NOTE_AS4, NOTE_C5, NOTE_AS4, REST, NOTE_F4, NOTE_DS4, REST,

  NOTE_C4, REST, NOTE_E4, REST, NOTE_G4, NOTE_A4, NOTE_AS4,
  NOTE_C5, REST, NOTE_C5, REST, NOTE_AS4, REST, NOTE_A4, REST,
  NOTE_AS4, REST, NOTE_AS4, NOTE_C5, REST, NOTE_AS4, NOTE_A4, REST,
  REST,
  NOTE_C5, REST, NOTE_AS4, REST, NOTE_A4, REST, NOTE_AS4, REST, NOTE_E5,
  REST,

  NOTE_C5, REST, NOTE_C5, REST, NOTE_AS4, REST, NOTE_A4, REST,
  NOTE_AS4, REST, NOTE_AS4, NOTE_C5, REST, NOTE_AS4, NOTE_A4, REST,
  REST,
  NOTE_C5, REST, NOTE_AS4, REST, NOTE_A4, REST, NOTE_AS4, REST, NOTE_E4,
  REST,
};

int subwayDurations[] = {
  4, 8, 4, 8, 4, 8, 8, 16, 8, 8, 16,
  4, 8, 4, 8, 4, 8, 8, 16, 8, 8, 16,
  4, 8, 4, 8, 4, 8, 8, 16, 8, 8, 16,

  4, 8, 4, 8, 4, 4, 4,
  8, 16, 8, 16, 8, 16, 8, 16,
  8, 16, 8, 8, 16, 8, 8, 16,
  4,
  8, 16, 8, 16, 8, 16, 8, 4, 8,
  4,

  8, 16, 8, 16, 8, 16, 8, 16,
  8, 16, 8, 8, 16, 8, 8, 16,
  4,
  8, 16, 8, 16, 8, 16, 8, 4, 8,
  1
};

void playTetrisMusic()
{
  for (int thisNote = 0; thisNote < tetrisNotes * 2; thisNote = thisNote + 2) 
  {

    // calculates the duration of each note
    divider = tetrisMelody[thisNote + 1];
    if (divider > 0) 
    {
      // regular note, just proceed
      tetrisNoteDuration = (wholenote) / divider;
    } 
    else if (divider < 0) 
    {
      // dotted notes are represented with negative durations!!
      tetrisNoteDuration = (wholenote) / abs(divider);
      tetrisNoteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, tetrisMelody[thisNote], tetrisNoteDuration*0.9);

    // Wait for the specief duration before playing the next note.
    delay(tetrisNoteDuration);
    
    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
}