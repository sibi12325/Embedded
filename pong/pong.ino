#include <TFT_eSPI.h> // Include the graphics and font library

// Screen dimensions
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 170

// Paddle and ball settings
#define PADDLE_WIDTH  10
#define PADDLE_HEIGHT 30
#define BALL_SIZE     8
#define PADDLE_SPEED  4
#define BALL_SPEED    2

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
int ballDirX = BALL_SPEED, ballDirY = BALL_SPEED;
int player1Score = 0, player2Score = 0;

void setup() {
  tft.init();
  tft.setRotation(1); // Landscape mode
  tft.fillScreen(TFT_BLACK);

  // Initialize player buttons
  pinMode(PLAYER1_UP, INPUT_PULLUP);
  pinMode(PLAYER1_DOWN, INPUT_PULLUP);
  pinMode(PLAYER2_UP, INPUT_PULLUP);
  pinMode(PLAYER2_DOWN, INPUT_PULLUP);
}

void loop() {
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

  // Update ball position
  updateBall();

  delay(20); // Frame rate control
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

  // Ball collision with top or bottom wall
  if (ballY <= 0 || ballY >= SCREEN_HEIGHT - BALL_SIZE) {
    ballDirY = -ballDirY;
  }

  // Ball collision with paddles
  if (ballX <= 20 && ballY >= player1Y && ballY <= player1Y + PADDLE_HEIGHT) {
    ballDirX = -ballDirX;
  }
  if (ballX >= SCREEN_WIDTH - 30 && ballY >= player2Y && ballY <= player2Y + PADDLE_HEIGHT) {
    ballDirX = -ballDirX;
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
  ballDirX = -ballDirX;  // Change direction after scoring
}
