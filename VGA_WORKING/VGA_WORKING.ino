//This example shows how to use the GfxWrapper to be able to use the Adafruit GFX library with VGA
//bitluni

#include "ESP32S3VGA.h"
#include <GfxWrapper.h>
#include <Fonts/FreeMonoBoldOblique24pt7b.h>
#include <Fonts/FreeSerif24pt7b.h>

//                   r,r,r,r,r,  g,g, g, g, g, g,   b, b, b, b, b,   h,v
const PinConfig pins(-1,-1,-1,-1,3,  -1,-1,-1,-1,-1,10,  -1,-1,-1,-1,11,  1,2);

//VGA Device
VGA vga;
Mode mode = Mode::MODE_320x240x60;
GfxWrapper<VGA> gfx(vga, mode.hRes, mode.vRes);

//initial setup
void setup()
{
	vga.bufferCount = 2;
	if(!vga.init(pins, mode, 16)) while(1) delay(1);

	vga.start();
}

// 123 rgb 
// 5678 10 ground
// 13 14 h/v sync

//the loop is done every frame
void loop()
{
	static int x = 0;
	vga.clear(vga.rgb(0, 0x80, 0));
	//using adafruit gfx
	gfx.setFont(&FreeMonoBoldOblique24pt7b);
	gfx.setCursor(100 + x, 100);
	gfx.print("VGA");
	gfx.setFont(&FreeSerif24pt7b);
	gfx.setCursor(100, 200);
	gfx.print("Works!");
  // gfx.drawPixel(int16_t x, int16_t y, uint16_t color)
	vga.show();
	x = (x + 1) & 255;
}
