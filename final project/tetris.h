// TETRIS DEGINITIONS
//--------------------------------------------------------------------------

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

void tetrisSetup(void);
void tetrisLoop();
