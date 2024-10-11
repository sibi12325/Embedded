#include <TFT_eSPI.h>
#include <SPI.h>

#include "tet.h"

#include "tetris.h"
#include "xo.h"
#include "universal.h"
#include "pong.h"
#include "mainMusic.h"

#include <TFT_eSPI.h>
#include <esp_now.h>
#include <WiFi.h>

#define totalGames 3;

bool downPressed = false;
bool upPressed = false;

int currentSelection = 0;

uint8_t newData;

void setup() 
{

  tft.init();
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(2);

  // Initialize buttons
  // pinMode(BUTTON_CONFIRM_1, INPUT_PULLUP);
  // pinMode(BUTTON_UP_1, INPUT_PULLUP);
  // pinMode(BUTTON_DOWN_1, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(1, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("RX_1", "RX_1_Password", CHANNEL, 0);

  esp_now_init();
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  // Display the menu
  drawMenu();
  Serial.begin(115200);
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  // Serial.print("I just received -> ");
  // Serial.println(*data);
  memcpy(&newData, data, sizeof(newData));
}

void loop() 
{
  Serial.println(String(currentSelection));

  switch(newData)
  {
    case 0:
    if (!digitalRead(14) == HIGH) 
      {
        tone(1, 500, 200);
        selectGame(currentSelection);
        delay(500);
        noTone(1);
      }
    upPressed = false;
    downPressed = false;
    break;

    case BUTTON_UP_1:
      if(!upPressed)
      {
        tone(1, 262, 200);
        currentSelection = currentSelection - 1;
        if(currentSelection < 0)
        {
          currentSelection = 2;
        }
        drawMenu();
        // delay(200);
        noTone(1);
      }

      upPressed = true;
      downPressed = false;
      break;
    
    case BUTTON_DOWN_1:
      if (!downPressed)
      {
        tone(1, 262, 200);
        currentSelection = currentSelection + 1; 
        if(currentSelection > 2)
        {
          currentSelection = 0;
        }
        drawMenu();
        // delay(200);
        noTone(1);
      }

      downPressed = true;
      upPressed = false;
      break;

    case BUTTON_CONFIRM_1:
      tone(1, 500, 200);
      selectGame(currentSelection);
      // delay(500);
      noTone(1);
      break;

      default:
      if (!digitalRead(14) == HIGH) 
      {
        tone(1, 500, 200);
        selectGame(currentSelection);
        // delay(500);
        noTone(1);
      }


  }

  // Moving up the menu
  // if (!digitalRead(BUTTON_UP_1) == HIGH) 
  // {
  //   if(!upPressed)
  //   {
  //     tone(1, 262, 200);
  //     currentSelection = currentSelection - 1;
  //     if(currentSelection < 0)
  //     {
  //       currentSelection = totalGames - 1;
  //     }
  //     drawMenu();
  //     delay(200);
  //     noTone(1);
  //   }

  //   upPressed = true;

  // }else
  // {
  //   upPressed = false;
  // }

  // Moving down the menu
  // if (!digitalRead(BUTTON_DOWN_1) == HIGH) 
  // {
  //   if (!downPressed)
  //   {
  //     tone(1, 262, 200);
  //     currentSelection = currentSelection + 1; 
  //     if(currentSelection > 2)
  //     {
  //       currentSelection = 0;
  //     }
  //     drawMenu();
  //     delay(200);
  //     noTone(1);
  //   }

  //   downPressed = true;

  // }
  // else
  // {
  //   downPressed = false;
  // }

  // selecting a game
  if (!digitalRead(14) == HIGH) 
  {
    tone(1, 500, 200);
    selectGame(currentSelection);
    delay(500);
    noTone(1);
  }

  // playMainMusic();
}

void drawMenu() 
{
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.drawString("Select a Game:", 10, tft.height() / 5);

  if (currentSelection == 0) 
  {
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("Pong", 70, 2*tft.height() / 5);
  } 
  else 
  {
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Pong", 70, 2*tft.height() / 5);
  }

  if (currentSelection == 1) 
  {
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("Tetris", 60, 3*tft.height() / 5);
  } 
  else 
  {
    tft.setTextColor(TFT_WHITE);
    tft.drawString("Tetris", 60, 3*tft.height() / 5);
  }

  if (currentSelection == 2) 
  {
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("X and O", 60, 4*tft.height() / 5);
  } 
  else 
  {
    tft.setTextColor(TFT_WHITE);
    tft.drawString("X and O", 60, 4*tft.height() / 5);
  }
}

void selectGame(int selection) 
{
  switch (selection) 
  {
    case 0:
      runPong();
      break;

    case 1:
      runTetris();
      break;

    case 2:
    runXandO();
    break;
  }
}

void runXandO()
{
  xSetup();
  
  bool running = true;

  while(running)
  {
    xLoop();
  }
}

void runPong()
{
  pongSetup();
  bool pongRunning = true;

  while(pongRunning)
  {
    pongLoop();
  }

}
void runTetris()
{

    tetrisSetup();
    

    bool tetrisRunning = true;

    while(tetrisRunning)
    {
      tetrisLoop();
    }

}