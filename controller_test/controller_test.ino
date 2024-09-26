#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

// Pins
#define pinA 16

#define upPin 21
#define downPin 18
#define leftPin 17
#define rightPin 43
#define pressPin 44


bool pinAPressed;

bool up;
bool left;
bool right;
bool down;
bool joyStickPress;

void setup() {
  // put your setup code here, to run once:

  tft.init();
  tft.fillScreen(TFT_WHITE);

  tft.setTextSize(4);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_BLACK);

  pinAPressed = false;

  pinMode(pinA, INPUT_PULLUP);

  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(pressPin, INPUT_PULLUP);

  up = false;
  left = false;
  right = false;
  down = false;
  joyStickPress = false;

}

void loop() {
  // put your main code here, to run repeatedly:
  pinAPressed = !digitalRead(pinA);
  
  up = digitalRead(upPin);
  down = digitalRead(downPin);
  left = digitalRead(leftPin);
  right = digitalRead(rightPin);
  joyStickPress = digitalRead(pressPin);


  if(pinAPressed == HIGH)
  {
      tft.fillScreen(TFT_BLACK);
  }else
  {
      tft.fillScreen(TFT_WHITE);

      if(up == HIGH)
      {
        tft.drawString("up",90,180);
      }
      else if(left == HIGH)
      {
        tft.drawString("left",90,180);
      }
      else if(right == HIGH)
      {
        tft.drawString("right",90,180);
      }
      else if(down == HIGH)
      {
        tft.drawString("down",90,180);
      }
      else if(joyStickPress == HIGH)
      {
        tft.drawString("joyStickPress",90,180);
      }
      else
      {
        tft.fillScreen(TFT_WHITE);
      }
  }

}
