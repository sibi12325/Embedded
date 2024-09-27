#include <SPI.h>
#include <TFT_eSPI.h>
#include "tet.h"


TFT_eSPI tft = TFT_eSPI(); 

// Block declaration 3d array of type [block colour][pixel length][pixel width]
uint16_t BlockImage[8][12][12];

// The background
uint16_t backBuffer[220][110];

// To store the coordinates of the blocks
struct Point 
{
  int X;
  int Y;
};

// The blocks themselves (their shape, total number of rotations, and colour)
struct Block 
{
  Point square[4][4]; 
  int numRotate; 
  int colour;
};

// The current block that is falling and its position
Point pos; 
Block block;

int rot, fall_cnt = 0;

bool started, gameover = false;

bool but_ROTATE, but_LEFT, but_RIGHT, but_RESET = false;

int game_speed = 20;

// Every shape and orientation
#define UP_Z {{0,-1},{0,0},{-1,0},{-1,1}}
#define NORMAL_Z { {-1,0},{0,0},{0,1},{1,1} }

#define REVERSED_NORMAL_Z {{-1,1},{0,1},{0,0},{1,0}}
#define REVERSED_UP_Z {{0,-1},{0,0},{1,0},{1,1}}

#define RIGHT_L { {-1,-1},{-1,0},{0,0},{1,0} }
#define UPSIDEDOWN_L { {-1,1},{0,1},{0,0},{0,-1} }
#define LEFT_L { {-1,0},{0,0},{1,0},{1,1} }
#define NORMAL_L { {1,-1},{0,-1},{0,0},{0,1} }

#define REVERSED_LEFT_L {{-1,0},{0,0},{1,0},{1,-1}}
#define REVERSED_NORMAL_L {{-1,-1},{0,-1},{0,0},{0,1}}
#define REVERSED_RIGHT_L {{-1,1},{-1,0},{0,0},{1,0}}
#define REVERSED_UPSIDEDOWN_L {{0,-1},{0,0},{0,1},{1,1}}

#define UPSIDEDOWN_WASD {{-1,0},{0,0},{1,0},{0,-1}}
#define LEFT_WASD {{0,-1},{0,0},{0,1},{-1,0}}
#define NORMAL_WASD {{-1,0},{0,0},{1,0},{0,1}}
#define RIGHT_WASD {{0,-1},{0,0},{0,1},{1,0}}


#define SQUARE { {0,-1},{1,-1},{0,0},{1,0} }

#define HORIZONTAL_LINE { {-1,0},{0,0},{1,0},{2,0} }
#define VERTICAL_LINE { {0,-1},{0,0},{0,1},{0,2} }

#define EMPTY { {0,0},{0,0},{0,0},{0,0} }

// Array storing all the shapes and orientation
Block blocks[7] = {
  {
    {  HORIZONTAL_LINE, VERTICAL_LINE,
    EMPTY, EMPTY  }, 
    2, 
    1
  },

  {  
    {  SQUARE, EMPTY,
    EMPTY, EMPTY  },
    1,
    2
  },

  {
    {  RIGHT_L,UPSIDEDOWN_L,
    LEFT_L,NORMAL_L  },
    4,
    3
  },

  {
    { NORMAL_Z,UP_Z,
    EMPTY, EMPTY },
    2,
    4
  },

  {
    { REVERSED_LEFT_L,REVERSED_NORMAL_L,
    REVERSED_RIGHT_L,REVERSED_UPSIDEDOWN_L },
    4,
    5
  },

  {
    { REVERSED_NORMAL_Z,REVERSED_UP_Z,
    EMPTY, EMPTY },
    2,
    6
  },

  {
    { UPSIDEDOWN_WASD,LEFT_WASD,
    NORMAL_WASD,RIGHT_WASD },
    4,
    7
  }
};

extern uint8_t tetris_img[];

// Initial conditions
bool leftButtonPressed = false;
bool rightButtonPressed = false;
bool rotateButtonPressed = false;
bool resetButtonPressed = false;

bool resetted = false;

int score=0;
int lvl=1;

int leftButton=16;
int rightButton=17;
int rotateButton = 0;
int resetButton = 14;

//  Define the colours to be used
#define GREY 0x5AEB
#define RED 0x00F0
#define PURPLE 0xFBE4
#define BLUE 0xFF00
#define GREEN 0xFF87
#define YELLOW 0x87FF
#define LIGHT_GREEN 0xF00F
#define PINK 0xF8FC

// the number of pixels for a side of a block
#define Length 11

// the number of horizontal blocks
#define Width 10

// the number of vertical blocks
#define Height 20

// Initialise the screen. it shows colour-numbers of all positions
int screen[Width][Height] = {0};

// Function to set up the screen for play
void screenInitialise(void)
{
  // The background colour
  tft.fillScreen(TFT_BLACK);  

  // The border lines
  tft.drawLine(11,19,122,19,GREY);
  tft.drawLine(11,19,11,240,GREY);
  tft.drawLine(122,19,122,240,GREY);
  tft.drawLine(11,240,122,240,GREY);
  
  // The score and level texts
  tft.drawString("SCORE:"+String(score),14,8,1);
  tft.drawString("LVL:"+String(lvl),88,8,1);

  // Make each block
  make_block( 0, TFT_BLACK);
  make_block( 1, RED);
  make_block( 2, PURPLE);
  make_block( 3, BLUE);
  make_block( 4, GREEN);
  make_block( 5, YELLOW);
  make_block( 6, LIGHT_GREEN);
  make_block( 7, PINK);


  // Start Position
  PutStartPos();

  // Initialise location of the first block
  for (int i = 0; i < 4; ++i) 
  {
    screen[pos.X + block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = block.colour;
  }

  // Draw the blocks
  Draw();
}

void setup(void) 
{
  
  pinMode(leftButton,INPUT_PULLUP);
  pinMode(rightButton,INPUT_PULLUP);
  pinMode(rotateButton,INPUT_PULLUP);
  pinMode(resetButton,INPUT_PULLUP);

  tft.init();
  tft.setRotation(0);
  tft.setSwapBytes(true);

  tft.pushImage(0, 0, 135, 240,tet);
  delay(3000);

  screenInitialise();
}
//========================================================================
void loop() 
{
  if (gameover)
  { 
    if(digitalRead(leftButton)==0)
    {
      screenInitialise();

      for (int j = 0; j < Height; ++j)
      {
        for (int i = 0; i < Width; ++i)
        {
          screen[i][j] = 0;
        }
      }

      gameover=false;
      score=0;
      game_speed=20;
      lvl=1;

      PutStartPos();                             // Start Position
      for (int i = 0; i < 4; ++i) 
      {
        screen[pos.X + block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = block.colour;
      }

      tft.drawString("SCORE:"+String(score),14,8,1);
      tft.drawString("LVL:"+String(lvl),88,8,1);
      Draw();  
    }

    else
    {
      if(resetted)
      {
        tft.fillScreen(TFT_BLACK);
        char *resetText = "PRESS BUTTON TO START GAME";

        // halfway point is at half the ttgo screen size - half the size of the text (to center text at center of ttgo)
        int xPosition = tft.width() / 2 - (strlen(resetText) / 2) * Length/2;
        tft.drawString(resetText, xPosition,180,1);

        resetted = false;
      }
    }

    return;
  }

  if(!gameover)
  {
    Point next_pos;
    int next_rot = rot;
    GetNextPosRot(&next_pos, &next_rot);
    ReviseScreen(next_pos, next_rot);
    delay(game_speed);                        // SPEED ADJUST
  }      

}
//========================================================================
void Draw()                                  // Draw 120x240 in the center
{                               
  for (int i = 0; i < Width; ++i) 
  {
    for (int j = 0; j < Height; ++j)
    {
      for (int k = 0; k < Length; ++k) 
      {
        for (int l = 0; l < Length; ++l)
        {
          backBuffer[j * Length + l][i * Length + k] = BlockImage[screen[i][j]][k][l];
        }
      }
    }
  }
  
  tft.pushImage(12, 20, 110, 220,*backBuffer);
}
//========================================================================
void PutStartPos() 
{
  game_speed=20;
  pos.X = 4; 
  pos.Y = 1;
  block = blocks[random(7)];
  rot = random(block.numRotate);
}
//========================================================================
bool GetSquares(Block block, Point pos, int rot, Point* squares) 
{
  bool overlap = false;
  for (int i = 0; i < 4; ++i) 
  {
    Point p;
    p.X = pos.X + block.square[rot][i].X;
    p.Y = pos.Y + block.square[rot][i].Y;
    // bitwise exclusive or
    overlap |= (p.X < 0 || p.X >= Width || p.Y < 0 || p.Y >= Height || screen[p.X][p.Y] != 0);
    squares[i] = p;
  }
  return !overlap;
}
//========================================================================
void GameOver() 
{
  for (int i = 0; i < Width; ++i) for (int j = 0; j < Height; ++j)
  {
    if (screen[i][j] != 0) 
    {
      screen[i][j] = 4;
    }
    gameover = true;
  }
 
}
//========================================================================
void ClearKeys() 
{ 
  but_ROTATE=false; 
  but_LEFT=false; 
  but_RIGHT=false; 
  but_RESET = false;
}
//========================================================================

void KeyPadLoop()
{
  // If the left button was pressed
  if(!digitalRead(leftButton))
  {
    // toggle so that you cant hold the button down
    if(!leftButtonPressed)
    {
      leftButtonPressed = true;
      ClearKeys();
      but_LEFT =true;
    }
  }
  // button not pressed
  else 
  {
    leftButtonPressed = false;
  }
  
  // If the right button was pressed
  if(!digitalRead(rightButton))
  {
    // toggle so that you cant hold the button down
    if(!rightButtonPressed)
    {
      rightButtonPressed = true;
      ClearKeys();
      but_RIGHT=true;
    }
  }
  // button not pressed
  else 
  {
    rightButtonPressed = false;
  }

  // If the rotate button was pressed
  if(!digitalRead(rotateButton))
  {
    // toggle so that you cant hold the button down
    if(!rotateButtonPressed)
    {
      rotateButtonPressed = true;
      ClearKeys();
      but_ROTATE =true;
    }
  }
  // button not pressed
  else 
  {
    rotateButtonPressed = false;
  }

  // If the reset button was pressed
  if(!digitalRead(resetButton))
  {
    // toggle so that you cant hold the button down
    if(!resetButtonPressed)
    {
        resetButtonPressed = true;
        ClearKeys();
        but_RESET =true;
    }
  }
  // button not pressed
  else 
  {
      resetButtonPressed = false;
  }
}
//========================================================================
void GetNextPosRot(Point* pnext_pos, int* pnext_rot) 
{
  KeyPadLoop();

  if (but_LEFT) 
  {
    started = true;
  }

  if (!started) 
  {
    return;
  }

  pnext_pos->X = pos.X;
  pnext_pos->Y = pos.Y;

  if ((fall_cnt = (fall_cnt + 1) % 10) == 0) 
  {
    pnext_pos->Y += 1;
  }
  else
  {
    if (but_LEFT) 
    { 
      but_LEFT = false; 
      pnext_pos->X -= 1;
    }
    else if (but_RIGHT) 
    { 
      but_RIGHT = false; 
      pnext_pos->X += 1;
    }
    else if (but_ROTATE) 
    { 
      but_ROTATE = false;
      *pnext_rot = (*pnext_rot + block.numRotate - 1) % block.numRotate; 
    }
    else if(but_RESET) 
    {
      but_RESET = false;
      resetted = true;
      GameOver();
      tft.pushImage(0, 0, 135, 240,tet);
      delay(2000);
      screenInitialise();
    }
  }
}
//========================================================================
void DeleteLine() 
{
  for (int j = 0; j < Height; ++j) 
  {
    bool Delete = true;
    for (int i = 0; i < Width; ++i) 
    {
      if (screen[i][j] == 0) 
      {
        Delete = false;
      }
    }

    if (Delete)
    {
      score++;
      if(score % 5 == 0)
      {
        lvl++;
        game_speed=game_speed-4;
        tft.drawString("LVL:"+String(lvl),88,8,1);
      }
      tft.drawString("SCORE:"+String(score),14,8,1);
      for (int k = j; k >= 1; --k) 
      {
        for (int i = 0; i < Width; ++i)
        {
          screen[i][k] = screen[i][k - 1];
        }
      }
    }
  }
}
//========================================================================
void ReviseScreen(Point next_pos, int next_rot) 
{
  if (!started) 
  {
    return;
  }

  Point next_squares[4];

  for (int i = 0; i < 4; ++i) 
  {
    screen[pos.X + block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = 0;
  }

  if (GetSquares(block, next_pos, next_rot, next_squares)) 
  {

   for (int i = 0; i < 4; ++i)
   {
     screen[next_squares[i].X][next_squares[i].Y] = block.colour;
   }
   
   pos = next_pos; rot = next_rot;

  }
  else 
  {
    for (int i = 0; i < 4; ++i) 
    {
      screen[pos.X + block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = block.colour;
    }

    if (next_pos.Y == pos.Y + 1) 
    {
      DeleteLine(); PutStartPos();
      if (!GetSquares(block, pos, rot, next_squares)) 
      {
        for (int i = 0; i < 4; ++i) 
        {
          screen[pos.X + block.square[rot][i].X][pos.Y + block.square[rot][i].Y] = block.colour;
        }

      GameOver();
      resetted = true;
      screenInitialise();
      }
    }
  }
  
  Draw();
}

//========================================================================
void make_block( int n , uint16_t colour )           // Make Block colour 
{                  
  for ( int i =0 ; i < 12; i++ ) 
  {
    for ( int j =0 ; j < 12; j++ )
    {
      BlockImage[n][i][j] = colour;                           // Block colour
      if ( i == 0 || j == 0 ) 
      {
        BlockImage[n][i][j] = 0;       // TFT_BLACK Line
      }
    } 
  }
}
//========================================================================
