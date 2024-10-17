#include <stdlib.h>    // For random number generation
#include "universal.h"
#include "platform.h"
#include <Arduino.h>

#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

// Define player variables
int play1X = 10, play1Y = 120;  // Player 1 starting position
int play2X = 50, play2Y = 120;  // Player 2 starting position
int playerWidth = 10, playerHeight = 10;
int velocity1Y = 0, velocity2Y = 0;  // Vertical velocities
bool isJumping1 = false, isJumping2 = false;

// Button control booleans (assumed to be set elsewhere in the code)
bool left1 = false, right1 = false, jump1 = false; // Player 1 controls
bool left2 = false, right2 = false, jump2 = false; // Player 2 controls

// Platform variables
const int platformWidth = 40, platformHeight = 10;
const int numPlatforms = 5;  // Number of platforms
int platformX[numPlatforms], platformY[numPlatforms];  // Array of platform positions

// Game physics
int gravity = 2;
int jumpStrength = -15;
int goalY = 10;  // Y position of the goal (top of the screen)

// Function to initialize the display
void setupDisplay() {
  vga.clear(vga.rgb(0, 0, 0));
  vga.show();
}

// Function to check for collisions between player and platforms
bool checkCollision(int px, int py, int pw, int ph, int ox, int oy, int ow, int oh) {
  return (px < ox + ow && px + pw > ox && py < oy + oh && py + ph > oy);
}

// Generate random platform positions, ensuring they are reachable
void generatePlatforms() {
  platformX[0] = 30;  // First platform is static for both players to start on
  platformY[0] = 150;
  
  for (int i = 1; i < numPlatforms; i++) {
    platformX[i] = rand() % (mode.hRes - platformWidth);
    platformY[i] = platformY[i - 1] - (rand() % 40 + 30);  // Random but spaced upwards
  }
}

// Handle player movement and gravity
void movePlayer(int &playerX, int &playerY, int &velocityY, bool &isJumping, uint8_t newData) 
{
  // Move left or right based on button presses

  switch(newData)
  {
    case BUTTON_LEFT_1:
      playerX -= 3;
      break;
    
    case BUTTON_RIGHT_1:
      playerX += 3;
      break;

    case BUTTON_CONFIRM_1:
      if (!isJumping) 
      {
        velocityY = jumpStrength;
        isJumping = true;
      }
      break;
  }

  // Apply gravity
  playerY += velocityY;
  velocityY += gravity;

  // Check collisions with platforms
  for (int i = 0; i < numPlatforms; i++) {
    if (checkCollision(playerX, playerY, playerWidth, playerHeight, platformX[i], platformY[i], platformWidth, platformHeight)) {
      playerY = platformY[i] - playerHeight;  // Place player on top of platform
      velocityY = 0;  // Reset vertical velocity
      isJumping = false;  // Player can jump again
    }
  }

  // Prevent player from falling below the screen
  if (playerY > mode.vRes - playerHeight) {
    playerY = mode.vRes - playerHeight;
    velocityY = 0;
    isJumping = false;
  }

  // Draw the player
  gfx.fillRect(playerX, playerY, playerWidth, playerHeight, (255,255,255));
  vga.show();
}

// Check if either player has won
bool checkWinner(int playerY) {
  return (playerY <= goalY);
}

// Main game loop
void loopGame() {
  vga.clear(vga.rgb(0, 0, 0));

  // Draw the platforms
  for (int i = 0; i < numPlatforms; i++) {
    gfx.fillRect(platformX[i], platformY[i], platformWidth, platformHeight, (0,255,0));
  }

  // Draw the goal line
  gfx.drawLine(0, goalY, mode.hRes, goalY, (255,0,0));

  vga.show();

  // Handle player 1 movement and collisions
  movePlayer(play1X, play1Y, velocity1Y, isJumping1, newData);

  // Handle player 2 movement and collisions
  movePlayer(play2X, play2Y, velocity2Y, isJumping2, newData);

  // Check if Player 1 or Player 2 wins
  if (checkWinner(play1Y)) 
  {
    vga.clear(vga.rgb(0, 0, 255));

    gfx.setCursor(mode.hRes / 5, mode.vRes / 2);
    gfx.print("Player 1 Wins!");

    vga.show();

    while (true);  // Stop the game

  } else if (checkWinner(play2Y)) 
  {
    vga.clear(vga.rgb(255, 0, 0));

    gfx.setCursor(mode.hRes / 5, mode.vRes / 2);
    gfx.print("Player 2 Wins!");

    vga.show();

    while (true);  // Stop the game
  }

  // Update display
  // tft.display();
}

void platformSetup() {
  setupDisplay();
  generatePlatforms();
  
  // Initialize buttons or input (assumed to be done elsewhere)
}

void platformLoop() {
  // Run the main game loop
  loopGame();
}
