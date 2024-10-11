#include "universal.h"
#include "pong.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "pongMusic.h"

// Paddle and ball settings
int PADDLE_WIDTH = 10;
int PADDLE_HEIGHT = 30;
int BALL_SIZE = 8;
int PADDLE_SPEED = 4;

// Ball speed settings
int INITIAL_BALL_SPEED = 3;
int BALL_SPEED_INCREMENT = 0.2;
int MAX_BALL_SPEED = 10;

bool collided = false;
bool scored = false;

// Player input pins

int PLAYER2_UP = 14;
int PLAYER2_DOWN = 0;

// direction of ball
float ballDirX = INITIAL_BALL_SPEED, ballDirY = INITIAL_BALL_SPEED;

// scores
int player1Score = 0, player2Score = 0;

// current speed
float currentBallSpeed = INITIAL_BALL_SPEED;

// pre check variables
bool gameStarted = false;
bool player1Ready = false, player2Ready = false;

// Starting positions of the ball and paddles
int player1Y;            
int player2Y;
int ballX;
int ballY;

void pongSetup()
{
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1); // puts in landscape mode

  player1Y = (tft.height() - PADDLE_HEIGHT) / 2;            
  player2Y = (tft.height() - PADDLE_HEIGHT) / 2;
  ballX = tft.width() / 2;
  ballY = tft.height() / 2;

  // set up pins
  // pinMode(PLAYER1_UP, INPUT_PULLUP);
  // pinMode(PLAYER1_DOWN, INPUT_PULLUP);
  pinMode(PLAYER2_UP, INPUT_PULLUP);
  pinMode(PLAYER2_DOWN, INPUT_PULLUP);

  // Show waiting screen
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  String text = "Waiting for players...";
  tft.drawString(text, tft.width() / 2 - 5*text.length(), tft.height() / 2);
}

void pongLoop()
{
    if (!gameStarted) 
    {
        // check that both players are ready

        // Check if player 1 is ready
        if (!player1Ready && (newData == 1 || newData == 2)) 
        {
        tone(1, 200, 200);
        tft.fillScreen(TFT_BLACK);
        player1Ready = true;
        String text = "Player 1 is ready";
        tft.drawString(text,tft.width() / 2 - 5*text.length(),tft.height() / 3);
        }

        // Check if player 2 is ready
        if (!player2Ready && (!digitalRead(PLAYER2_UP) == HIGH || !digitalRead(PLAYER2_DOWN) == HIGH)) 
        {
        tone(1,200,200);
        player2Ready = true;
        String text = "Player 2 is ready";
        tft.drawString(text,tft.width() / 2 - 5*text.length(),2*tft.height() / 3);
        }

        // wait half a second to look nice
        delay(100);
        
        // If both players are ready, start a countdown
        if (player1Ready && player2Ready) 
        {
        for (int i = 3; i > 0; i--) 
        {
            tone(1,400,400);
            tft.fillScreen(TFT_BLACK);
            tft.drawString(String(i),tft.width() / 2, tft.height() / 2);
            delay(1000);  // 1 second delay for countdown
        }

        tft.fillScreen(TFT_BLACK);  // Clear the screen after countdown
        gameStarted = true;
        }
    } 
    else 
    {
        playPongMusic();
        tft.fillScreen(TFT_BLACK);

        // Draw paddles
        tft.fillRect(10, player1Y, PADDLE_WIDTH, PADDLE_HEIGHT, TFT_WHITE);
        tft.fillRect(tft.width() - 20, player2Y, PADDLE_WIDTH, PADDLE_HEIGHT, TFT_WHITE);

        // Draw ball
        tft.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, TFT_WHITE);

        // Draw scores
        tft.drawString(String(player1Score), tft.width() / 3, 10);
        tft.drawString(String(player2Score), 2*tft.width() / 3, 10);

        // Player 1 controls
        if (newData == 1 && player1Y > 0) 
        {
        player1Y -= PADDLE_SPEED;
        }
        if (newData == 2 && player1Y < tft.height() - PADDLE_HEIGHT) 
        {
        player1Y += PADDLE_SPEED;
        }

        // Player 2 controls
        if (!digitalRead(PLAYER2_UP) == HIGH && player2Y > 0) 
        {
        player2Y -= PADDLE_SPEED;
        }
        if (!digitalRead(PLAYER2_DOWN) == HIGH && player2Y < tft.height() - PADDLE_HEIGHT) 
        {
        player2Y += PADDLE_SPEED;
        }

        // Update ball position and speed
        
        // Move ball
        ballX += ballDirX;
        ballY += ballDirY;

        // Increase ball speed as the game goes on, until maximum speed is reached
        if (currentBallSpeed < MAX_BALL_SPEED) 
        {
        currentBallSpeed += BALL_SPEED_INCREMENT / 100;
        }

        // If ball collides with the ceiling or floor then flip direction
        if (ballY <= 0 || ballY >= tft.height() - BALL_SIZE) 
        {
        ballDirY = -ballDirY;
        }

        // If it hits player 1s paddle, then flip x direciton
        if (ballX <= 20 && ballX >= 10 && ballY + BALL_SIZE >= player1Y && ballY <= player1Y + PADDLE_HEIGHT) 
        {
          noTone(1);
          tone(1,50,50);
        ballDirX = currentBallSpeed;
        ballDirY = (ballY - (player1Y + PADDLE_HEIGHT / 2)) / 4;  // Add some angle to the bounce
        }

        // If it hits player 2s paddle, then flip x direciton
        if (ballX >= tft.width() - 30 && ballX <= tft.width() - 20 && ballY + BALL_SIZE >= player2Y && ballY <= player2Y + PADDLE_HEIGHT) 
        {
          noTone(1);
          tone(1,50,50);
        ballDirX = -currentBallSpeed;
        ballDirY = (ballY - (player2Y + PADDLE_HEIGHT / 2)) / 4;  // Add some angle to the bounce
        }

        // IF ball goes behind the paddles then a score happened. Reset the ball after
        if (ballX < 10) 
        {
          noTone(1);
          tone(1,100,50);
        player2Score++;
        
        //reset ball

        ballX = tft.width() / 2;
        ballY = tft.height() / 2;
        currentBallSpeed = INITIAL_BALL_SPEED;

        // Start the ball in a random direction
        if(random(0,2) == 0)
        {
            ballDirX = -INITIAL_BALL_SPEED;
        }
        else
        {
            ballDirX = INITIAL_BALL_SPEED;
        }
        ballDirY = INITIAL_BALL_SPEED;
        }
        if (ballX > tft.width() - 10) 
        {
          noTone(1);
          tone(1,100,50);
        player1Score++;
        
        //reset ball

        ballX = tft.width() / 2;
        ballY = tft.height() / 2;
        currentBallSpeed = INITIAL_BALL_SPEED;

        // Start the ball in a random direction
        if(random(0,2) == 0)
        {
            ballDirX = -INITIAL_BALL_SPEED;
        }
        else
        {
            ballDirX = INITIAL_BALL_SPEED;
        }
        ballDirY = INITIAL_BALL_SPEED;
        }

        delay(20); // Frame rate control
    }
}