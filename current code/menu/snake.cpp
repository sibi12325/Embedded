#include <Arduino.h>
// #include <TFT_eSPI.h>  // Library for TFT display
#include <SPI.h>
#include "snake.h"
#include "universal.h"

#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

#define SNAKE_SIZE 10  // Size of each snake block
const int SCREEN_WIDTH = mode.hRes;
const int SCREEN_HEIGHT = mode.vRes;

int snakeLength = 5;
int snakeX[100], snakeY[100];  // Coordinates of the snake
int foodX, foodY;  // Coordinates of the food
int direction = 0;  // 0 = Right, 1 = Down, 2 = Left, 3 = Up

unsigned long lastMoveTime = 0;
int moveInterval = 200;  // Move snake every 200 ms

bool gameOver = false;

// Function to generate new food at a random position
void generateFood() {
  foodX = random(0, SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;  // Ensure food aligns with grid
  foodY = random(0, SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;
}

// Function to initialize the game
void initGame() {
  snakeLength = 5;
  for (int i = 0; i < snakeLength; i++) {
    snakeX[i] = SCREEN_WIDTH / 2 - i * SNAKE_SIZE;
    snakeY[i] = SCREEN_HEIGHT / 2 - 5;
  }
  generateFood();
  gameOver = false;
  direction = 0;  // Start by moving right
}

// Function to update snake's position
void updateSnake() {
  if (millis() - lastMoveTime > moveInterval) {
    lastMoveTime = millis();

    // Move the body
    for (int i = snakeLength - 1; i > 0; i--) {
      snakeX[i] = snakeX[i - 1];
      snakeY[i] = snakeY[i - 1];
    }

    // Move the head
    if (direction == 0) snakeX[0] += SNAKE_SIZE;  // Right
    else if (direction == 1) snakeY[0] += SNAKE_SIZE;  // Down
    else if (direction == 2) snakeX[0] -= SNAKE_SIZE;  // Left
    else if (direction == 3) snakeY[0] -= SNAKE_SIZE;  // Up

    // Check if snake eats the food (aligned with the grid)
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
      snakeLength++;
      generateFood();
    }

    // Check if snake runs into the walls
    if (snakeX[0] < 0 || snakeX[0] >= SCREEN_WIDTH || snakeY[0] < 0 || snakeY[0] >= SCREEN_HEIGHT) {
      gameOver = true;
    }

    // Check if snake runs into itself
    for (int i = 1; i < snakeLength; i++) {
      if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
        gameOver = true;
      }
    }
  }
}

// Function to draw the game elements
void drawGame() {
  // tft.fillScreen(TFT_BLACK);  // Clear screen
  vga.clear(vga.rgb(0, 0, 0));

  // Draw the snake
  gfx.fillRect(snakeX[0], snakeY[0], SNAKE_SIZE, SNAKE_SIZE, (0,255,0));  // Snake head
  for (int i = 1; i < snakeLength; i++) {
    gfx.fillRect(snakeX[i], snakeY[i], SNAKE_SIZE, SNAKE_SIZE, (255,255,255));  // Snake body
  }

  // Draw the food
  gfx.fillRect(foodX, foodY, SNAKE_SIZE, SNAKE_SIZE, (255,0,0));  // Food

  // If game over, show "Game Over" message
  if (gameOver) {
    gfx.setTextSize(2);
    gfx.setTextColor((255,0,0));
    gfx.setCursor(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 10);
    gfx.print("Game Over!");
  }

  vga.show();
}

// Function to handle button inputs for controlling the snake
void handleInput() 
{
  switch(newData)
  {
    case BUTTON_UP_1:
      if(direction != 1)
      {
        direction = 3;
      }
      break;
    
    case BUTTON_DOWN_1:
      if(direction != 3)
      {
        direction = 1;
      }
      break;

    case BUTTON_LEFT_1:
      if(direction != 0)
      {
        direction = 2;
      }
      break;

    case BUTTON_RIGHT_1:
      if(direction != 2)
      {
        direction = 0;
      }
      break;
  }
  // if (analogRead(joyXPin) > 3000 && !digitalRead(joyZPin) == HIGH && direction != 2) direction = 0;  // Right
  // else if (analogRead(joyYPin) > 3000 && direction != 3) direction = 1;  // Down
  // else if (analogRead(joyXPin) < 1000 && direction != 0) direction = 2;  // Left
  // else if (analogRead(joyYPin) < 1000 && direction != 1) direction = 3;  // Up
}

void snakeSetup() {
  // tft.init();
  // tft.setRotation(1);  // Set rotation for landscape mode
  // tft.fillScreen(TFT_BLACK);

  vga.clear(vga.rgb(0, 0, 0));

  // pinMode(joyZPin,INPUT);
  // digitalWrite(joyZPin,HIGH);

  initGame();  // Initialize the game

  // Serial.begin(115200);

  vga.show();
}

void snakeLoop() {
  
  if (!gameOver) {
    handleInput();
    updateSnake();
    drawGame();
  }
  delay(50);  // Small delay to control game speed
}
