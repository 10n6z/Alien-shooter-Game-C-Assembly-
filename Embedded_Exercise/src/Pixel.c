/*
 * Pixel.c
 *
 *  Created on: 19/11/2024
 *      Author: Pham Le Minh Long
 */

#include "Pixel.h"
#define RSTn (1 << 0)
#define LAT  (1 << 1)
#define SB   (1 << 2)
#define SCK  (1 << 3)
#define SDA  (1 << 4)

//Table for pixel dots.
//				 dots[X][Y][COLOR]
volatile uint8_t dots[8][8][3]={0};

// Here the setup operations for the LED matrix will be performed
void setup(){

	CONTROL = 0;
	CHANNEL = 0;

	// Resetting the screen at start is a MUST to operation (Set RSTn to 1).
	CONTROL &= ~RSTn;
	usleep(500);
	CONTROL |= RSTn;
	usleep(500);

	//Write code that sets 6-bit values in register of DM163 chip. Recommended that every bit in that register is set to 1. 6-bits and 24 "bytes", so some kind of loop structure could be nice.
	//24*6 bits needs to be transmitted
	CONTROL |= SDA;
	for(uint8_t i = 0;i < 24;i++){
		for(uint8_t j = 0;j < 6;j++){
			CONTROL &= ~SCK;
			CONTROL |= SCK;
		}
	}

	//Final thing in this function is to set SB-bit to 1 to enable transmission to 8-bit register.
	CONTROL |= SB;

}

//Change value of one pixel at led matrix. This function is only used for changing values of dots array
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b){

	//Hint: you can invert Y-axis quite easily with 7-y
	dots[x][y][0]=b;
	//Write rest of two lines of code required to make this function work properly (green and red colors to array).
	dots[x][y][1]=g;
	dots[x][y][2]=r;

}


//Put new data to led matrix. Hint: This function is supposed to send 24-bytes and parameter x is for channel x-coordinate.
void run(uint8_t x){

	CONTROL &= ~LAT;
	//Write code that writes data to led matrix driver (8-bit data). Use values from dots array
	//Hint: use nested loops (loops inside loops)
	//Hint2: loop iterations are 8,3,8 (pixels,color,8-bitdata)
	for(uint8_t y = 0;y < 8;y++){
		for(uint8_t color = 0;color < 3;color++){
			uint8_t cur = dots[x][y][color];
			for(uint8_t bit = 0;bit < 8;bit++){
				if (cur & (1 << (7 - bit))) CONTROL |= SDA;
				else CONTROL &= ~SDA;
				CONTROL &= ~SCK;
				CONTROL |= SCK;
			}
		}
	}
	latch();

}

//Latch signal. See colorsshield.pdf or DM163.pdf in project folder on how latching works
void latch(){
	CONTROL |= LAT;
	CONTROL &= ~LAT;

}


//Set one line (channel) as active, one at a time.
void open_line(uint8_t x){
	switch(x){
		case 0: CHANNEL |= (1 << 0); break;
		case 1: CHANNEL |= (1 << 1); break;
		case 2: CHANNEL |= (1 << 2); break;
		case 3: CHANNEL |= (1 << 3); break;
		case 4: CHANNEL |= (1 << 4); break;
		case 5: CHANNEL |= (1 << 5); break;
		case 6: CHANNEL |= (1 << 6); break;
		case 7: CHANNEL |= (1 << 7); break;
		default: CHANNEL = 0;
	}

}



