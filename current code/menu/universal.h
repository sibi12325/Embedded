#define BUTTON_UP_1 1    // Player 1 UP button
#define BUTTON_DOWN_1 2  // Player 1 DOWN button
#define BUTTON_LEFT_1 3  // Player 1 LEFT button
#define BUTTON_RIGHT_1 4 // Player 1 RIGHT button

#define BUTTON_CONFIRM_1 5 // Player 1 confirm button

#define BUTTON_UP_2 6    // Player 2 UP button
#define BUTTON_DOWN_2 7  // Player 2 DOWN button
#define BUTTON_LEFT_2 8  // Player 2 LEFT button
#define BUTTON_RIGHT_2 9 // Player 2 RIGHT button

#define BUTTON_CONFIRM_2 10 // Player 2 confirm button

#include <TFT_eSPI.h>

extern TFT_eSPI tft;

#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

// Declare PinConfig pins as extern
extern const PinConfig pins;

// Declare VGA object as extern
extern VGA vga;

// Declare Mode object as extern
extern Mode mode;

// Declare GfxWrapper object as extern
extern GfxWrapper<VGA> gfx;

#define CHANNEL 1

extern uint8_t newData;

// PWM channel for buzzer
#define pwmChannel 0
#define pwmResolution 8
#define pwmFreq 5000

extern int volume;

void setupVolume();
void updateVolume();
