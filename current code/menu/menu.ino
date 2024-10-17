// #include <TFT_eSPI.h>
#include <SPI.h>

#include "tet.h"

#include "tetris.h"
#include "xo.h"
#include "universal.h"
#include "pong.h"
#include "snake.h"
#include "platform.h"
#include "spaceship.h"

#include <esp_now.h>
#include <WiFi.h>

//This example shows how to use the GfxWrapper to be able to use the Adafruit GFX library with VGA
//bitluni

#include "ESP32S3VGA.h"
#include <GfxWrapper.h>

//                   r,r,r,r,r,  g,g, g, g, g, g,   b, b, b, b, b,   h,v
const PinConfig pins(-1,-1,-1,-1,12,  -1,-1,-1,-1,-1,10,  -1,-1,-1,-1,11,  1,2);

//VGA Device
VGA vga;
Mode mode = Mode::MODE_320x240x60;
GfxWrapper<VGA> gfx(vga, mode.hRes, mode.vRes);

bool downPressed = false;
bool upPressed = false;

int currentSelection = 0;

uint8_t newData;

void setup() 
{
  // setupVolume();

  Serial.begin(115200);
	vga.bufferCount = 2;
	if(!vga.init(pins, mode, 16)) 
  {
    while(1)
    {
      Serial.println("FAILED TO INITIALISE VGA");
      delay(1);
    }
  } 

	vga.start();
  Serial.println("VGA STARTED");

  // tft.init();
  // tft.fillScreen(TFT_BLACK);

  // tft.setTextSize(2);

  pinMode(13, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("RX_1", "RX_1_Password", CHANNEL, 0);

  esp_now_init();
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  drawMenu();
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  memcpy(&newData, data, sizeof(newData));
}

void loop() 
{
  // Serial.println(String(currentSelection));

  switch(newData)
  {
    case 0:
      upPressed = false;
      downPressed = false;
      break;
    case BUTTON_UP_1:
      if(!upPressed)
      {
        tone(13, 262, 200);
        currentSelection = currentSelection - 1;
        if(currentSelection < 0)
        {
          currentSelection = 5;
        }
        drawMenu();
        noTone(13);
      }

      upPressed = true;
      downPressed = false;
      break;
    
    case BUTTON_DOWN_1:
      if (!downPressed)
      {
        tone(13, 262, 200);
        currentSelection = currentSelection + 1; 
        if(currentSelection > 5)
        {
          currentSelection = 0;
        }
        drawMenu();
        noTone(13);
      }

      downPressed = true;
      upPressed = false;
      break;

    case BUTTON_CONFIRM_1:
      tone(13, 500, 200);
      selectGame(currentSelection);
      noTone(13);
      break;
  }

  Serial.println(String(newData));
}

void drawMenu() 
{
  vga.clear(vga.rgb(0, 0, 0));
  
  gfx.setTextColor(vga.rgb(255, 255, 255));
  gfx.setCursor(mode.hRes / 3 + 10, mode.vRes / 8);
	gfx.print("Select a Game:");

  if (currentSelection == 0) 
  {
    gfx.setTextColor(vga.rgb(255, 0, 0));
    gfx.setCursor(mode.hRes / 3 + 30, 2*mode.vRes / 8);
	  gfx.print("Pong");
  } 
  else 
  {
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.setCursor(mode.hRes / 3 + 30, 2*mode.vRes / 8);
	  gfx.print("Pong");
  }

  if (currentSelection == 1) 
  {
    gfx.setTextColor(vga.rgb(255, 0, 0));
    gfx.setCursor(mode.hRes / 3 + 30, 3*mode.vRes / 8);
	  gfx.print("Tetris");
  } 
  else 
  {
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.setCursor(mode.hRes / 3 + 30, 3*mode.vRes / 8);
	  gfx.print("Tetris");
  }

  if (currentSelection == 2) 
  {
    gfx.setTextColor(vga.rgb(255, 0, 0));
    gfx.setCursor(mode.hRes / 3 + 30, 4*mode.vRes / 8);
	  gfx.print("X and O");
  } 
  else 
  {
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.setCursor(mode.hRes / 3 + 30, 4*mode.vRes / 8);
	  gfx.print("X and O");
  }

  if (currentSelection == 3) 
  {
    gfx.setTextColor(vga.rgb(255, 0, 0));
    gfx.setCursor(mode.hRes / 3 + 30, 5*mode.vRes / 8);
	  gfx.print("Snake");
  } 
  else 
  {
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.setCursor(mode.hRes / 3 + 30, 5*mode.vRes / 8);
	  gfx.print("Snake");
  }

  if (currentSelection == 4) 
  {
    gfx.setTextColor(vga.rgb(255, 0, 0));
    gfx.setCursor(mode.hRes / 3 + 30, 6*mode.vRes / 8);
	  gfx.print("Platform");
  } 
  else 
  {
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.setCursor(mode.hRes / 3 + 30, 6*mode.vRes / 8);
	  gfx.print("Platform");
  }

  if (currentSelection == 5) 
  {
    gfx.setTextColor(vga.rgb(255, 0, 0));
    gfx.setCursor(mode.hRes / 3 + 30, 7*mode.vRes / 8);
	  gfx.print("SpaceShip");
  } 
  else 
  {
    gfx.setTextColor(vga.rgb(255, 255, 255));
    gfx.setCursor(mode.hRes / 3 + 30, 7*mode.vRes / 8);
	  gfx.print("SpaceShip");
  }

  vga.show();
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

    case 3:
      runSnake();
      break;
    
    case 4:
      runPlatform();
      break;
    
    case 5:
      runSpaceship();
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
    // updateVolume();
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

void runSnake()
{
  snakeSetup();
  bool snakeRunning = true;

  while(snakeRunning)
  {
    snakeLoop();
  }

}

void runPlatform()
{
  platformSetup();
  bool platformRunning = true;

  while(platformRunning)
  {
    platformLoop();
  }

}


void runSpaceship()
{
  spaceshipSetup();
  bool platformRunning = true;

  while(platformRunning)
  {
    spaceshipLoop();
  }

}