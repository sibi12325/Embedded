#include "universal.h"
#include "spaceship.h"
#include <Arduino.h>

#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

// Game variables
int spaceshipX = 60;      // X position of the spaceship
int spaceshipY = 110;     // Y position of the spaceship (fixed at bottom)
int spaceshipWidth = 20;  // Width of the spaceship
int spaceshipHeight = 10; // Height of the spaceship

int asteroidX;
int asteroidY;
int asteroidSpeed = 2;

bool spaceGameOver = false;
unsigned long spaceScore = 0;

// Reset asteroid position to top
void resetAsteroid() {
  asteroidX = random(0, mode.hRes - 10); // Random X position
  asteroidY = 0;                           // Start from the top
}

void spaceshipSetup() {
  // Initialize the display
  vga.clear(vga.rgb(0, 0, 255));

  vga.show();

  // Initial asteroid position
  randomSeed(analogRead(0));
  resetAsteroid();
}

void spaceshipLoop() {
  if (!spaceGameOver) {
    // Clear previous frame
    vga.clear(vga.rgb(0, 0, 255));

    // Draw spaceship
    gfx.fillRect(spaceshipX, spaceshipY, spaceshipWidth, spaceshipHeight, (255,255,255));

    vga.show();

    // Move spaceship
    if (newData == BUTTON_LEFT_1) {
      spaceshipX -= 3;
      if (spaceshipX < 0) spaceshipX = 0;  // Boundary check
    }
    if (newData == BUTTON_RIGHT_1) {
      spaceshipX += 3;
      if (spaceshipX > mode.hRes - spaceshipWidth) spaceshipX = mode.hRes - spaceshipWidth;  // Boundary check
    }

    // Move asteroid
    asteroidY += asteroidSpeed;

    // Check if the asteroid goes off the screen
    if (asteroidY > mode.vRes) {
      resetAsteroid(); // Reset asteroid
      spaceScore++;
      asteroidSpeed++;
    }

    // Draw asteroid
    gfx.fillRect(asteroidX, asteroidY, 10, 10, (255,0,0));

    vga.show();

    // Collision detection
    if (asteroidY + 10 > spaceshipY && asteroidY < spaceshipY + spaceshipHeight) {
      if (asteroidX + 10 > spaceshipX && asteroidX < spaceshipX + spaceshipWidth) {
        spaceGameOver = true;
      }
    }

    // Display spaceScore
    gfx.setCursor(0, 0);
    gfx.setTextSize(2);
    gfx.setTextColor((255,255,255));
    gfx.print("spaceScore: ");
    gfx.print(spaceScore);

    vga.show();

    // Delay for smooth movement
    delay(20);
  } else {
    // Game over screen
    vga.clear(vga.rgb(0, 0, 255));
    gfx.setCursor(20, 50);
    gfx.setTextSize(2);
    gfx.setTextColor((255,0,0));
    gfx.print("GAME OVER");

    gfx.setCursor(20, 80);
    gfx.setTextSize(2);
    gfx.setTextColor((255,0,0));
    gfx.print("spaceScore: ");
    gfx.print(spaceScore);

    vga.show();

    delay(5000); // Wait 5 seconds before resetting
    spaceGameOver = false;
    spaceScore = 0;
    resetAsteroid();
  }
}
