#include <Arduino.h>
#include "xo.h"
// #include <TFT_eSPI.h>
#include "universal.h"
#include "xoMusic.h"

#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

uint8_t previous = 9999;

int playerXScore = 0;
int playerOScore = 0;

// Game variables
char board[3][3];  // The Tic-Tac-Toe board
int cursorX = 0, cursorY = 0; // Cursor for current player selection
bool playerTurn = true; // True = Player 1's turn, False = Player 2's turn

// Function to check if a player has won
bool checkWin(char marker) {
  // Check rows and columns
  for (int i = 0; i < 3; i++) {
    if (board[i][0] == marker && board[i][1] == marker && board[i][2] == marker) return true;
    if (board[0][i] == marker && board[1][i] == marker && board[2][i] == marker) return true;
  }

  // Check diagonals
  if (board[0][0] == marker && board[1][1] == marker && board[2][2] == marker) return true;
  if (board[0][2] == marker && board[1][1] == marker && board[2][0] == marker) return true;

  return false;
}

// Function to check if the board is full (tie)
bool checkTie() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ') return false;
    }
  }
  return true;
}

// Function to draw the Tic-Tac-Toe board
void drawBoard() {
  
  // tft.fillScreen(TFT_BLACK);
  vga.clear(vga.rgb(0, 0, 0));

  // Draw grid lines
  gfx.drawLine(mode.hRes / 3, 0, mode.hRes / 3, 3*mode.hRes / 3, (255,255,255)); // Vertical lines
  gfx.drawLine(2*mode.hRes / 3, 0, 2*mode.hRes / 3, 3*mode.hRes / 3, (255,255,255));
  gfx.drawLine(0, mode.hRes / 3, 3*mode.hRes / 3, mode.hRes / 3, (255,255,255)); // Horizontal lines
  gfx.drawLine(0, 2*mode.hRes / 3, 3*mode.hRes / 3, 2*mode.hRes / 3, (255,255,255));

  // Draw the current board state
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      gfx.setCursor(j * mode.hRes / 3 + (mode.hRes / 6) - 5, i *mode.hRes / 3 + (mode.hRes / 6) - 5);
      gfx.print(board[i][j]);
    }
  }

  // Highlight current cursor
  gfx.drawRect(cursorX * mode.hRes / 3, cursorY * mode.hRes / 3, mode.hRes / 3, mode.hRes / 3, (255,0,0));

  // tft.drawString(String("X : ") + String(playerXScore), 10, 3*mode.vRes / 4);
  // tft.drawString(String("O : ") + String(playerOScore), 100, 3*mode.vRes / 4);

  gfx.setCursor(10, 3*mode.vRes / 4);
  gfx.print(String("X : ") + String(playerXScore));

  gfx.setCursor(100, 3*mode.vRes / 4);
  gfx.print(String("O : ") + String(playerOScore));

  vga.show();
}

// Function to handle player input
void playerInput(uint8_t data, char marker) {
  if(previous != newData)
  {
    if (data == BUTTON_UP_1) {
      cursorY = (cursorY + 2) % 3;  // Move cursor up
      drawBoard();
    }
    if (data == BUTTON_DOWN_1) {
      cursorY = (cursorY + 1) % 3;  // Move cursor down
      drawBoard();
    }
    if (data == BUTTON_LEFT_1) {
      cursorX = (cursorX + 2) % 3;  // Move cursor left
      drawBoard();
    }
    if (data == BUTTON_RIGHT_1) {
      cursorX = (cursorX + 1) % 3;  // Move cursor right
      drawBoard();
    }
    if (data == BUTTON_CONFIRM_1) 
    {
      if (board[cursorY][cursorX] == ' ') 
      {  // Check if the space is empty
      board[cursorY][cursorX] = marker;    // Place the marker ('X' or 'O')
      drawBoard();

      if (checkWin(marker)) {

        // tft.fillScreen(TFT_BLACK);
        // tft.setCursor(20, 80);
        // tft.setTextColor(TFT_WHITE);

        vga.clear(vga.rgb(0, 0, 0));
        gfx.setCursor(20, 80);
        gfx.setTextColor((255,255,255));

        if(marker == 'X')
        {
            gfx.print("X Wins!");
            playerXScore += 1;
        }else
        {
          gfx.print("O Wins!");
          playerOScore += 1;
        }

        vga.show();

        delay(1500);
        // Initialize board with empty spaces
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
          }
        }

        drawBoard();
      }

      if (checkTie()) {
        vga.clear(vga.rgb(0, 0, 0));
        gfx.setCursor(50, 80);
        gfx.setTextColor((255,255,255));
        gfx.print("It's a Tie!");
        vga.show();
        delay(1500);
        // Initialize board with empty spaces
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
          }
        }

        drawBoard();
      }

      playerTurn = !playerTurn;  // Switch turns
    }
    }
  }
}

void xSetup() {
  // Initialize the display
  // tft.init();
  // tft.setRotation(4);
  // tft.fillScreen(TFT_BLACK);
  // tft.setTextSize(2);

  vga.clear(vga.rgb(0, 0, 0));
  gfx.setTextSize(2);

  vga.show();

  // Initialize buttons
  pinMode(0, INPUT_PULLUP);

  // Initialize board with empty spaces
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }

  drawBoard();
}

void xLoop() {
  // Check for player input based on turn
  if (playerTurn) {
      playerInput(newData, 'X');
   } 
   else {
      playerInput(newData, 'O');
  }

  previous = newData;

  playXOmusic();
}