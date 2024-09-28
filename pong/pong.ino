#include <TFT_eSPI.h> // Include the graphics and font library

// Screen dimensions
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 170

// Paddle and ball settings
#define PADDLE_WIDTH  10
#define PADDLE_HEIGHT 30
#define BALL_SIZE     8
#define PADDLE_SPEED  4
#define INITIAL_BALL_SPEED  2
#define BALL_SPEED_INCREMENT 0.2
#define MAX_BALL_SPEED  6

// Player input buttons
#define PLAYER1_UP    34
#define PLAYER1_DOWN  35
#define PLAYER2_UP    32
#define PLAYER2_DOWN  33

TFT_eSPI tft = TFT_eSPI();  // Create display object

// Game state variables
int player1Y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
int player2Y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
int ballX = SCREEN_WIDTH / 2, ballY = SCREEN_HEIGHT / 2;
float ballDirX = INITIAL_BALL_SPEED, ballDirY = INITIAL_BALL_SPEED;
int player1Score = 0, player2Score = 0;
float currentBallSpeed = INITIAL_BALL_SPEED;
bool gameStarted = false;
bool player1Ready = false, player2Ready = false;

void setup() {
  tft.init();
  tft.setRotation(1); // Landscape mode
  tft.fillScreen(TFT_BLACK);

  // Initialize player buttons
  pinMode(PLAYER1_UP, INPUT_PULLUP);
  pinMode(PLAYER1_DOWN, INPUT_PULLUP);
  pinMode(PLAYER2_UP, INPUT_PULLUP);
  pinMode(PLAYER2_DOWN, INPUT_PULLUP);

  // Show initial screen
  tft.setCursor(50, 70);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.print("Waiting for players...");
}

void loop() {
  if (!gameStarted) {
    handlePlayerReady();
  } else {
    // Clear screen
    tft.fillScreen(TFT_BLACK);

    // Draw paddles
    drawPaddle(10, player1Y);
    drawPaddle(SCREEN_WIDTH - 20, player2Y);

    // Draw ball
    tft.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, TFT_WHITE);

    // Draw scores
    drawScore(player1Score, player2Score);

    // Handle paddle movement
    handlePaddleMovement();

    // Update ball position and speed
    updateBall();

    delay(20); // Frame rate control
  }
}

void handlePlayerReady() {
  // Check if player 1 is ready
  if (!player1Ready && (digitalRead(PLAYER1_UP) == LOW || digitalRead(PLAYER1_DOWN) == LOW)) {
    player1Ready = true;
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(50, 50);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print("Player 1 is ready");
  }

  // Check if player 2 is ready
  if (!player2Ready && (digitalRead(PLAYER2_UP) == LOW || digitalRead(PLAYER2_DOWN) == LOW)) {
    player2Ready = true;
    tft.setCursor(150, 50);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print("Player 2 is ready");
  }

  // If both players are ready, start a countdown
  if (player1Ready && player2Ready) {
    for (int i = 3; i > 0; i--) {
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(120, 70);
      tft.setTextSize(3);
      tft.setTextColor(TFT_WHITE);
      tft.print(i);
      delay(1000);  // 1 second delay for countdown
    }
    tft.fillScreen(TFT_BLACK);  // Clear the screen after countdown
    gameStarted = true;
  }
}

void drawPaddle(int x, int y) {
  tft.fillRect(x, y, PADDLE_WIDTH, PADDLE_HEIGHT, TFT_WHITE);
}

void drawScore(int p1, int p2) {
  tft.setCursor(90, 10);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.print("Player 1: ");
  tft.print(p1);

  tft.setCursor(190, 10);
  tft.print("Player 2: ");
  tft.print(p2);
}

void handlePaddleMovement() {
  // Player 1 controls
  if (digitalRead(PLAYER1_UP) == LOW && player1Y > 0) {
    player1Y -= PADDLE_SPEED;
  }
  if (digitalRead(PLAYER1_DOWN) == LOW && player1Y < SCREEN_HEIGHT - PADDLE_HEIGHT) {
    player1Y += PADDLE_SPEED;
  }

  // Player 2 controls
  if (digitalRead(PLAYER2_UP) == LOW && player2Y > 0) {
    player2Y -= PADDLE_SPEED;
  }
  if (digitalRead(PLAYER2_DOWN) == LOW && player2Y < SCREEN_HEIGHT - PADDLE_HEIGHT) {
    player2Y += PADDLE_SPEED;
  }
}

void updateBall() {
  // Move ball
  ballX += ballDirX;
  ballY += ballDirY;

  // Increase ball speed as the game goes on, until maximum speed is reached
  if (currentBallSpeed < MAX_BALL_SPEED) {
    currentBallSpeed += BALL_SPEED_INCREMENT / 100; // Gradual speed increase
  }

  // Ball collision with top or bottom wall
  if (ballY <= 0 || ballY >= SCREEN_HEIGHT - BALL_SIZE) {
    ballDirY = -ballDirY;
  }

  // Ball collision with player 1's paddle
  if (ballX <= 20 && ballY + BALL_SIZE >= player1Y && ballY <= player1Y + PADDLE_HEIGHT) {
    ballDirX = currentBallSpeed;  // Reflect direction and use current speed
    ballDirY = (ballY - (player1Y + PADDLE_HEIGHT / 2)) / 4;  // Add some angle to the bounce
  }

  // Ball collision with player 2's paddle
  if (ballX >= SCREEN_WIDTH - 30 && ballY + BALL_SIZE >= player2Y && ballY <= player2Y + PADDLE_HEIGHT) {
    ballDirX = -currentBallSpeed;  // Reflect direction and use current speed
    ballDirY = (ballY - (player2Y + PADDLE_HEIGHT / 2)) / 4;  // Add some angle to the bounce
  }

  // Ball out of bounds (scoring)
  if (ballX < 0) {
    player2Score++;
    resetBall();
  }
  if (ballX > SCREEN_WIDTH) {
    player1Score++;
    resetBall();
  }
}

void resetBall() {
  ballX = SCREEN_WIDTH / 2;
  ballY = SCREEN_HEIGHT / 2;
  currentBallSpeed = INITIAL_BALL_SPEED;  // Reset speed
  ballDirX = (random(0, 2) == 0) ? -INITIAL_BALL_SPEED : INITIAL_BALL_SPEED;  // Randomize initial direction
  ballDirY = INITIAL_BALL_SPEED;
}
