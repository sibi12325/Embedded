#include <Arduino.h>
#include "xo.h"
#include <TFT_eSPI.h>
#include "universal.h"
#include "xoMusic.h"

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
  tft.fillScreen(TFT_BLACK);

  // Draw grid lines
  tft.drawLine(tft.width() / 3, 0, tft.width() / 3, 3*tft.width() / 3, TFT_WHITE); // Vertical lines
  tft.drawLine(2*tft.width() / 3, 0, 2*tft.width() / 3, 3*tft.width() / 3, TFT_WHITE);
  tft.drawLine(0, tft.width() / 3, 3*tft.width() / 3, tft.width() / 3, TFT_WHITE); // Horizontal lines
  tft.drawLine(0, 2*tft.width() / 3, 3*tft.width() / 3, 2*tft.width() / 3, TFT_WHITE);

  // Draw the current board state
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      tft.setCursor(j * tft.width() / 3 + (tft.width() / 6) - 5, i *tft.width() / 3 + (tft.width() / 6) - 5);
      tft.print(board[i][j]);
    }
  }

  // Highlight current cursor
  tft.drawRect(cursorX * tft.width() / 3, cursorY * tft.width() / 3, tft.width() / 3, tft.width() / 3, TFT_YELLOW);

  tft.drawString(String("X : ") + String(playerXScore), 10, 3*tft.height() / 4);
  tft.drawString(String("O : ") + String(playerOScore), 100, 3*tft.height() / 4);
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
  }
  if (!digitalRead(0) == HIGH) {
    if (board[cursorY][cursorX] == ' ') {  // Check if the space is empty
      board[cursorY][cursorX] = marker;    // Place the marker ('X' or 'O')
      drawBoard();

      if (checkWin(marker)) {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(20, 80);
        tft.setTextColor(TFT_WHITE);
        if(marker == 'X')
        {
            tft.print("X Wins!");
            playerXScore += 1;
        }else
        {
          tft.print("O Wins!");
          playerOScore += 1;
        }
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
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(50, 80);
        tft.setTextColor(TFT_WHITE);
        tft.print("It's a Tie!");
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

void xSetup() {
  // Initialize the display
  tft.init();
  tft.setRotation(4);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);

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