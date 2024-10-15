// -------------------------------------------------
// Copyright (c) 2024 HiBit <https://www.hibit.dev>
// -------------------------------------------------

#include <Arduino.h>
#include "pitches.h"
#include "mainMusic.h"

#define BUZZER_PIN 1

int mainMelody[] = {
  NOTE_C5, NOTE_GS4, NOTE_A4, NOTE_C5, NOTE_D5, NOTE_F5, NOTE_F5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_FS5, NOTE_D5, NOTE_A5,
  NOTE_A5, NOTE_A5, NOTE_G5, NOTE_A5, NOTE_C6, NOTE_A5,
  NOTE_C5, NOTE_A5, NOTE_F5
};

int mainDurations[] = {
  6, 6, 6, 6, 6, 6, 2,
  6, 6, 6, 6, 6, 2,
  6, 6, 6, 6, 6, 3,
  6, 6, 1
};

void playMainMusic()
{
  int size = sizeof(mainDurations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / mainDurations[note];
    tone(BUZZER_PIN, mainMelody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing:
    noTone(BUZZER_PIN);
  }
}