#include "pitches.h"
#include "tetrisMusic.h"
#include <Arduino.h>

#define BUZZER_PIN 13

// Tetris melody and note durations
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

// Size of the Tetris melody array
int tetrisNotes = sizeof(tetrisMelody) / sizeof(tetrisMelody[0]) / 2;

// Tempo settings
int tetrisTempo = 144;
int tetrisWholenote = (60000 * 4) / tetrisTempo;

// Variables for note playback
int tetrisDivider = 0, tetrisNoteDuration = 0;
unsigned long tetrisPreviousMillis = 0;
int tetrisCurrentNote = 0;
int tetrisPauseBetweenNotes = 0;

void playTetrisMusic() {
  unsigned long currentMillis = millis();

  // Check if enough time has passed to move to the next note
  if (currentMillis - tetrisPreviousMillis >= tetrisNoteDuration + tetrisPauseBetweenNotes) {
    
    // Check if there are more notes to play
    if (tetrisCurrentNote < tetrisNotes * 2) {
      // Calculate the duration of the note
      tetrisDivider = tetrisMelody[tetrisCurrentNote + 1];
      if (tetrisDivider > 0) {
        tetrisNoteDuration = (tetrisWholenote) / tetrisDivider;
      } else if (tetrisDivider < 0) {
        tetrisNoteDuration = (tetrisWholenote) / abs(tetrisDivider);
        tetrisNoteDuration *= 1.5; // Handle dotted notes
      }

      // Play the note
      tone(BUZZER_PIN, tetrisMelody[tetrisCurrentNote], tetrisNoteDuration * 0.9);

      // Record the time when the note started playing
      tetrisPreviousMillis = currentMillis;

      // Set the pause between notes (10% of the note duration)
      tetrisPauseBetweenNotes = tetrisNoteDuration * 0.1;

      // Move to the next note
      tetrisCurrentNote += 2;
    } else {
      // If the melody is finished, stop the tone
      noTone(BUZZER_PIN);
      tetrisCurrentNote = 0; // Optionally reset the melody or stop completely
    }
  }
}
