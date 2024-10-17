#include "universal.h"
#include "pong.h"
#include <Arduino.h>
// #include <TFT_eSPI.h>
#include "pongMusic.h"

#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

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
  // tft.init();
  // tft.fillScreen(TFT_BLACK);
  // tft.setRotation(1); // puts in landscape mode

  player1Y = (mode.vRes - PADDLE_HEIGHT) / 2;            
  player2Y = (mode.vRes - PADDLE_HEIGHT) / 2;
  ballX = mode.hRes / 2;
  ballY = mode.vRes / 2;

  // set up pins
  // pinMode(PLAYER1_UP, INPUT_PULLUP);
  // pinMode(PLAYER1_DOWN, INPUT_PULLUP);
  pinMode(PLAYER2_UP, INPUT_PULLUP);
  pinMode(PLAYER2_DOWN, INPUT_PULLUP);

  vga.clear(vga.rgb(0, 0, 0));
  vga.show();

  // Show waiting screen
  // tft.setTextSize(2);
  // tft.setTextColor(TFT_WHITE);
  // String text = "Waiting for players...";
  // tft.drawString(text, mode.hRes / 2 - 5*text.length(), mode.vRes / 2);

  String text = "Waiting for players...";
  gfx.setTextSize(2);
  gfx.setTextColor(vga.rgb(255, 255, 255));
  gfx.setCursor(mode.hRes / 2 - 5*text.length(), mode.vRes / 2);
  gfx.print(text);
  vga.show();
}

void pongLoop()
{
    if (!gameStarted) 
    {
        // check that both players are ready

        // Check if player 1 is ready
        if (!player1Ready && (newData == 1 || newData == 2)) 
        {
        tone(13, 200, 200);

        vga.clear(vga.rgb(0, 0, 0));

        // tft.fillScreen(TFT_BLACK);
        player1Ready = true;
        String text = "Player 1 is ready";
        // tft.drawString(text,mode.hRes / 2 - 5*text.length(),mode.vRes / 3);
        gfx.setCursor(mode.hRes / 2 - 5*text.length(),mode.vRes / 3);
        gfx.print(text);
        vga.show();
        }

        // Check if player 2 is ready
        if (!player2Ready && (!digitalRead(PLAYER2_UP) == HIGH || !digitalRead(PLAYER2_DOWN) == HIGH)) 
        {
        tone(13,200,200);
        player2Ready = true;
        String text = "Player 2 is ready";
        // tft.drawString(text,mode.hRes / 2 - 5*text.length(),2*mode.vRes / 3);

        gfx.setCursor(mode.hRes / 2 - 5*text.length(),2*mode.vRes / 3);
        gfx.print(text);
        vga.show();
        }

        // wait half a second to look nice
        delay(100);
        
        // If both players are ready, start a countdown
        if (player1Ready && player2Ready) 
        {
        for (int i = 3; i > 0; i--) 
        {
            tone(13,400,400);

            // tft.fillScreen(TFT_BLACK);
            // tft.drawString(String(i),mode.hRes / 2, mode.vRes / 2);

            vga.clear(vga.rgb(0, 0, 0));
            gfx.setCursor(mode.hRes / 2, mode.vRes / 2);
            gfx.print(String(i));
            vga.show();
            delay(1000);  // 1 second delay for countdown
        }

        // tft.fillScreen(TFT_BLACK);  // Clear the screen after countdown
        vga.clear(vga.rgb(0, 0, 0));
        vga.show();
        gameStarted = true;
        }
    } 
    else 
    {
        playPongMusic();
        // tft.fillScreen(TFT_BLACK);
        vga.clear(vga.rgb(0, 0, 0));

        // Draw paddles
        gfx.fillRect(10, player1Y, PADDLE_WIDTH, PADDLE_HEIGHT, (255,255,255));
        gfx.fillRect(mode.hRes - 20, player2Y, PADDLE_WIDTH, PADDLE_HEIGHT, (255,255,255));

        // Draw ball
        gfx.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, (255,255,255));

        // Draw scores
        // tft.drawString(String(player1Score), mode.hRes / 3, 10);
        // tft.drawString(String(player2Score), 2*mode.hRes / 3, 10);

        gfx.setCursor(mode.hRes / 3, 10);
        gfx.print(String(player1Score));

        gfx.setCursor(2*mode.hRes / 3, 10);
        gfx.print(String(player2Score));

        vga.show();

        // Player 1 controls
        if (newData == 1 && player1Y > 0) 
        {
        player1Y -= PADDLE_SPEED;
        }
        if (newData == 2 && player1Y < mode.vRes - PADDLE_HEIGHT) 
        {
        player1Y += PADDLE_SPEED;
        }

        // Player 2 controls
        if (!digitalRead(PLAYER2_UP) == HIGH && player2Y > 0) 
        {
        player2Y -= PADDLE_SPEED;
        }
        if (!digitalRead(PLAYER2_DOWN) == HIGH && player2Y < mode.vRes - PADDLE_HEIGHT) 
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
        if (ballY <= 0 || ballY >= mode.vRes - BALL_SIZE) 
        {
        ballDirY = -ballDirY;
        }

        // If it hits player 1s paddle, then flip x direciton
        if (ballX <= 20 && ballX >= 10 && ballY + BALL_SIZE >= player1Y && ballY <= player1Y + PADDLE_HEIGHT) 
        {
          noTone(13);
          tone(13,50,50);
        ballDirX = currentBallSpeed;
        ballDirY = (ballY - (player1Y + PADDLE_HEIGHT / 2)) / 4;  // Add some angle to the bounce
        }

        // If it hits player 2s paddle, then flip x direciton
        if (ballX >= mode.hRes - 30 && ballX <= mode.hRes - 20 && ballY + BALL_SIZE >= player2Y && ballY <= player2Y + PADDLE_HEIGHT) 
        {
          noTone(13);
          tone(13,50,50);
        ballDirX = -currentBallSpeed;
        ballDirY = (ballY - (player2Y + PADDLE_HEIGHT / 2)) / 4;  // Add some angle to the bounce
        }

        // IF ball goes behind the paddles then a score happened. Reset the ball after
        if (ballX < 10) 
        {
          noTone(13);
          tone(13,100,50);
        player2Score++;
        
        //reset ball

        ballX = mode.hRes / 2;
        ballY = mode.vRes / 2;
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
        if (ballX > mode.hRes - 10) 
        {
          noTone(13);
          tone(13,100,50);
        player1Score++;
        
        //reset ball

        ballX = mode.hRes / 2;
        ballY = mode.vRes / 2;
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