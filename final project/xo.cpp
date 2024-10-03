#include <Arduino.h>
#include "xo.h"
#include <TFT_eSPI.h>
#include "universal.h"
#include "xoMusic.h"

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
  tft.drawLine(60, 0, 60, 180, TFT_WHITE); // Vertical lines
  tft.drawLine(120, 0, 120, 180, TFT_WHITE);
  tft.drawLine(0, 60, 180, 60, TFT_WHITE); // Horizontal lines
  tft.drawLine(0, 120, 180, 120, TFT_WHITE);

  // Draw the current board state
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      tft.setCursor(j * 60 + 25, i * 60 + 20);
      tft.print(board[i][j]);
    }
  }

  // Highlight current cursor
  tft.drawRect(cursorX * 60, cursorY * 60, 60, 60, TFT_YELLOW);
}

// Function to handle player input
void playerInput(int up, int down, int left, int right, char marker) {
  if (!digitalRead(up) == HIGH) {
    cursorY = (cursorY + 2) % 3;  // Move cursor up
    drawBoard();
    delay(200); // Debounce delay
  }
  if (!digitalRead(down) == HIGH) {
    cursorY = (cursorY + 1) % 3;  // Move cursor down
    drawBoard();
    delay(200);
  }
  if (digitalRead(left) == HIGH) {
    cursorX = (cursorX + 2) % 3;  // Move cursor left
    drawBoard();
    delay(200);
  }
  if (digitalRead(right) == HIGH) {
    cursorX = (cursorX + 1) % 3;  // Move cursor right
    drawBoard();
    delay(200);
  }
  if (digitalRead(left) == HIGH && digitalRead(right) == HIGH) {
    if (board[cursorY][cursorX] == ' ') {  // Check if the space is empty
      board[cursorY][cursorX] = marker;    // Place the marker ('X' or 'O')
      drawBoard();

      if (checkWin(marker)) {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(20, 80);
        tft.setTextColor(TFT_WHITE);
        tft.print(marker == 'X' ? "Player 1 Wins!" : "Player 2 Wins!");
        while(1); // Game Over
      }

      if (checkTie()) {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(50, 80);
        tft.setTextColor(TFT_WHITE);
        tft.print("It's a Tie!");
        while(1); // Game Over
      }

      playerTurn = !playerTurn;  // Switch turns
    }
    delay(200);  // Debounce delay
  }
}

void xSetup() {
  // Initialize the display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);

  // Initialize buttons
  pinMode(BUTTON_UP_1, INPUT);
  pinMode(BUTTON_DOWN_1, INPUT);
  pinMode(BUTTON_LEFT_1, INPUT);
  pinMode(BUTTON_RIGHT_1, INPUT);
  //pinMode(BUTTON_CONFIRM_1, INPUT);

  // pinMode(BUTTON_UP_2, INPUT);
  // pinMode(BUTTON_DOWN_2, INPUT);
  // pinMode(BUTTON_LEFT_2, INPUT);
  // pinMode(BUTTON_RIGHT_2, INPUT);
  //pinMode(BUTTON_CONFIRM_2, INPUT);

  // Initialize board with empty spaces
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }

  drawBoard();
}

void xLoop() {
  playXOmusic();
  // Check for player input based on turn
  if (playerTurn) {
    playerInput(BUTTON_UP_1, BUTTON_DOWN_1, BUTTON_LEFT_1, BUTTON_RIGHT_1, 'X');
   } //else {
  //   playerInput(BUTTON_UP_2, BUTTON_DOWN_2, BUTTON_LEFT_2, BUTTON_RIGHT_2, 'O');
  // }
}