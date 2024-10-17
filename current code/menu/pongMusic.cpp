// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include "pitches.h"
#include "pongMusic.h"
#include "universal.h"
#include <Arduino.h>

#define BUZZER_PIN 13

// change this to make the song slower or faster
int pongTempo = 200;

// pongNotes of the melody followed by the pongDuration.
// a 4 means a quarter note, 8 an eighth, 16 sixteenth, so on
// !!negative numbers are used to represent dotted pongNotes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighth!!
int pongMelody[] = {
  
  // Greensleeves 
  // Score available at https://musescore.com/user/168402/scores/1396946
  // Alexander Trompoukis
  
  NOTE_G4,8,//1
  NOTE_AS4,4, NOTE_C5,8, NOTE_D5,-8, NOTE_DS5,16, NOTE_D5,8,
  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,
  NOTE_AS4,4, NOTE_G4,8, NOTE_G4,-8, NOTE_FS4,16, NOTE_G4,8,
  NOTE_A4,4, NOTE_FS4,8, NOTE_D4,4, NOTE_G4,8,
  
  NOTE_AS4,4, NOTE_C5,8, NOTE_D5,-8, NOTE_DS5,16, NOTE_D5,8,//6
  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,
  NOTE_AS4,-8, NOTE_A4,16, NOTE_G4,8, NOTE_FS4,-8, NOTE_E4,16, NOTE_FS4,8, 
  NOTE_G4,-2,
  NOTE_F5,2, NOTE_E5,16, NOTE_D5,8,

  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,//11
  NOTE_AS4,4, NOTE_G4,8, NOTE_G4,-8, NOTE_FS4,16, NOTE_G4,8,
  NOTE_A4,4, NOTE_FS4,8, NOTE_D4,04,
  NOTE_F5,2, NOTE_E5,16, NOTE_D5,8,
  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,

  NOTE_AS4,-8, NOTE_A4,16, NOTE_G4,8, NOTE_FS4,-8, NOTE_E4,16, NOTE_FS4,8,//16
  NOTE_G4,-2,

  //repeats from the beginning

  NOTE_G4,8,//1
  NOTE_AS4,4, NOTE_C5,8, NOTE_D5,-8, NOTE_DS5,16, NOTE_D5,8,
  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,
  NOTE_AS4,4, NOTE_G4,8, NOTE_G4,-8, NOTE_FS4,16, NOTE_G4,8,
  NOTE_A4,4, NOTE_FS4,8, NOTE_D4,4, NOTE_G4,8,
  
  NOTE_AS4,4, NOTE_C5,8, NOTE_D5,-8, NOTE_DS5,16, NOTE_D5,8,//6
  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,
  NOTE_AS4,-8, NOTE_A4,16, NOTE_G4,8, NOTE_FS4,-8, NOTE_E4,16, NOTE_FS4,8, 
  NOTE_G4,-2,
  NOTE_F5,2, NOTE_E5,16, NOTE_D5,8,

  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,//11
  NOTE_AS4,4, NOTE_G4,8, NOTE_G4,-8, NOTE_FS4,16, NOTE_G4,8,
  NOTE_A4,4, NOTE_FS4,8, NOTE_D4,04,
  NOTE_F5,2, NOTE_E5,16, NOTE_D5,8,
  NOTE_C5,4, NOTE_A4,8, NOTE_F4,-8, NOTE_G4,16, NOTE_A4,8,

  NOTE_AS4,-8, NOTE_A4,16, NOTE_G4,8, NOTE_FS4,-8, NOTE_E4,16, NOTE_FS4,8,//16
  NOTE_G4,-2
  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and pongDuration), so for each note there are four bytes
int pongNotes = sizeof(pongMelody) / sizeof(pongMelody[0]) / 2;

// this calculates the duration of a whole note in ms
int pongWholenote = (60000 * 4) / pongTempo;

int pongDivider = 0, notePongDuration = 0;

// Variables to manage non-blocking music playback
unsigned long previousMillis = 0;
int currentNote = 0;
int pauseBetweenNotes = 0;

void playNote(int frequency, int duration) {
  ledcWriteTone(pwmChannel, frequency);
  ledcWrite(pwmChannel, volume);
  delay(duration);
  ledcWrite(pwmChannel, 0);
}

void playPongMusic() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= notePongDuration + pauseBetweenNotes) {
    if (currentNote < pongNotes * 2) {
      int pongDivider = pongMelody[currentNote + 1];
      if (pongDivider > 0) {
        notePongDuration = (pongWholenote) / pongDivider;
      } else if (pongDivider < 0) {
        notePongDuration = (pongWholenote) / abs(pongDivider);
        notePongDuration *= 1.5;
      }

      playNote(pongMelody[currentNote], notePongDuration * 0.9);

      previousMillis = currentMillis;
      pauseBetweenNotes = notePongDuration * 0.1;
      currentNote += 2;
    } else {
      ledcWrite(pwmChannel, 0);
      currentNote = 0;
    }
  }
}