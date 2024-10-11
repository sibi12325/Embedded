// -------------------------------------------------
// Copyright (c) 2022 HiBit <https://www.hibit.dev>
// -------------------------------------------------

// #include "pitches.h"
// #include "pongMusic.h"
// #include <Arduino.h>

/*************************************************
 * Public Constants
 *************************************************/

#define BUTTON_PIN_0 0
#define BUTTON_PIN_14 14

 #include <TFT_eSPI.h>
// #include "universal.h"

TFT_eSPI tft = TFT_eSPI();

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0

#define BUZZER_PIN 1

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

int pongTempo = 200;
int volume = 128; // Default volume, range: 0-255
int volumeStep = 40; // Adjust volume by 16 steps

int pongNotes = sizeof(pongMelody) / sizeof(pongMelody[0]) / 2;
int pongWholenote = (60000 * 4) / pongTempo;

unsigned long previousMillis = 0;
int currentNote = 0;
int notePongDuration = 0;
int pauseBetweenNotes = 0;

// PWM channel for buzzer
const int pwmChannel = 0;
const int pwmResolution = 8;
const int pwmFreq = 5000;

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

void updateVolumeDisplay() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("Volume: ");
  tft.print(map(volume, 0, 255, 0, 100));
  tft.print("%");
}

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  pinMode(BUTTON_PIN_0, INPUT_PULLUP);
  pinMode(BUTTON_PIN_14, INPUT_PULLUP);

  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(BUZZER_PIN, pwmChannel);

  updateVolumeDisplay();
}

void loop() {
  static bool button0Pressed = false;
  static bool button14Pressed = false;

  // Check button on pin 0 for volume increase
  if (digitalRead(BUTTON_PIN_0) == LOW) {
    if (!button0Pressed) {
      button0Pressed = true;
      volume = min(255, volume + volumeStep);
      updateVolumeDisplay();
    }
  } else {
    button0Pressed = false;
  }

  // Check button on pin 14 for volume decrease
  if (digitalRead(BUTTON_PIN_14) == LOW) {
    if (!button14Pressed) {
      button14Pressed = true;
      volume = max(0, volume - volumeStep);
      updateVolumeDisplay();
    }
  } else {
    button14Pressed = false;
  }
  
  playPongMusic();

  delay(10); // Small delay to prevent CPU hogging
}
