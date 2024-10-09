#include <Arduino.h>

const int buzzerPin = 27;  // Pin where the buzzer is connected
const int buttonUpPin = 15;  // Pin for volume up button
const int buttonDownPin = 2; // Pin for volume down button

int volume = 128;  // Initial volume (0-255 for 8-bit resolution)
int frequency = 1000;  // Frequency of the buzzer sound

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonDownPin, INPUT_PULLUP);
}

void loop() {
  // Check if volume up button is pressed
  if (digitalRead(buttonUpPin) == LOW) {
    volume = min(255, volume + 10);  // Increase volume, max is 255
    delay(100);  // Debouncing delay
  }

  // Check if volume down button is pressed
  if (digitalRead(buttonDownPin) == LOW) {
    volume = max(0, volume - 10);  // Decrease volume, min is 0
    delay(100);  // Debouncing delay
  }

  // Generate PWM signal based on the volume
  analogWrite(buzzerPin, volume);

  // Simulate the buzzer sound with a specific frequency
  tone(buzzerPin, frequency);

  delay(10);
}
