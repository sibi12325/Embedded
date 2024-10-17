#include <TFT_eSPI.h>
#include "universal.h"

#define volumeUp 0
#define volumeDown 14
#define BUZZER_PIN 13

int volume = 128; // Default volume, range: 0-255
int volumeStep = 40; // Adjust volume by 16 steps

void setupVolume()
{
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(BUZZER_PIN, pwmChannel);

  pinMode(volumeUp,INPUT_PULLUP);
  pinMode(volumeDown,INPUT_PULLUP);
}

void updateVolume()
{
  static bool volumeUpPressed = false;
  static bool volumeDownPressed = false;

  // Check button on pin 0 for volume increase
  if (digitalRead(volumeUp) == LOW) {
    if (!volumeUpPressed) {
      volumeUpPressed = true;
      volume = min(255, volume + volumeStep);
    }
  } else {
    volumeUpPressed = false;
  }

  // Check button on pin 14 for volume decrease
  if (digitalRead(volumeDown) == LOW) {
    if (!volumeDownPressed) {
      volumeDownPressed = true;
      volume = max(0, volume - volumeStep);
    }
  } else {
    volumeDownPressed = false;
  }
}

TFT_eSPI tft = TFT_eSPI();