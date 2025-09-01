/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 *
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

// Main program for exercise

//****************************************************
//By default, every output used in this exercise is 0
//****************************************************
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xgpiops.h"
#include "xttcps.h"
#include "xscugic.h"
#include "xparameters.h"
#include "Pixel.h"
#include "Interrupt_setup.h"

//********************************************************************
//***************TRY TO READ COMMENTS*********************************
//********************************************************************

//***Hint: Use sleep(x)  or usleep(x) if you want some delays.****
//***To call assembler code found in blinker.S, call it using: blinker();***


//Comment this if you want to disable all interrupts
#define enable_interrupts




/***************************************************************************************
Name: Pham Le Minh Long
Student number: 153176768

Tick boxes that you have coded

Led-matrix driver		Game		    Assembler
	[]					[]					[]

Brief description:

*****************************************************************************************/
#include <stdlib.h>
volatile uint8_t ship_x = 0;       // Ship's starting X position
volatile uint8_t ship_width = 3;  // Ship's width in pixels
volatile uint8_t alien_x = 0;     // Alien's X position
volatile uint8_t alien_y = 0;     // Alien's Y position
volatile uint8_t alien_alive = 1; // Is the alien alive?

volatile uint8_t bullet_x = 255;  // Bullet's X position (255 = inactive)
volatile uint8_t bullet_y = 255;  // Bullet's Y position (255 = inactive)
volatile uint8_t bullet_active = 0; // Is the bullet active?

volatile uint8_t current_channel = 0; // Current channel for 800 Hz refresh

volatile uint8_t game_over = 0;
volatile uint8_t points = 0;

volatile uint8_t run_game = 1;

void draw_init();
void FireBullet();


int main()
{
	//**DO NOT REMOVE THIS****
	    init_platform();
	//************************


#ifdef	enable_interrupts
	    init_interrupts();
#endif


	    //setup screen
	    setup();

	    //initial pixels to screen
	    draw_init();

	    Xil_ExceptionEnable();



	    //Try to avoid writing any code in the main loop.
		while(1){

		}


		cleanup_platform();
		return 0;
}

void Reset_Game(){
	run_game = 1;

	for(uint8_t i = 0;i < 8;i++){
		for(uint8_t j = 0;j < 8;j++){
			SetPixel(i, j ,0 ,0 ,0);
		}
	}

	ship_x = 0;       // Ship's starting X position
	ship_width = 3;  // Ship's width in pixels
	alien_x = 0;     // Alien's X position
	alien_y = 0;     // Alien's Y position
	alien_alive = 1; // Is the alien alive?

	bullet_x = 255;  // Bullet's X position (255 = inactive)
	bullet_y = 255;  // Bullet's Y position (255 = inactive)
	bullet_active = 0;

	points = 0;
	draw_init();
}

void draw_init(){
	for(uint8_t i = 0;i < ship_width;i++){
		if(i == 1) SetPixel(ship_x + i, 6, 0, 255, 0);
		SetPixel(ship_x + i, 7, 0, 255, 0);
	}
}

void Win(){

	for(uint8_t i = 0;i < 8;i++){
		for(uint8_t j = 0;j < 8;j++){
			SetPixel(i, j ,0 ,0 ,0);
		}
	}

	SetPixel(1, 1, 0, 255, 0);
	SetPixel(2, 1, 0, 255, 0);
	SetPixel(1, 2, 0, 255, 0);
	SetPixel(2, 2, 0, 255, 0);

	SetPixel(5, 1, 0, 255, 0);
	SetPixel(6, 1, 0, 255, 0);
	SetPixel(5, 2, 0, 255, 0);
	SetPixel(6, 2, 0, 255, 0);

	SetPixel(1, 5, 0, 255, 0);
	SetPixel(2, 6, 0, 255, 0);
	SetPixel(3, 6, 0, 255, 0);
	SetPixel(4, 6, 0, 255, 0);
	SetPixel(5, 6, 0, 255, 0);
	SetPixel(6, 5, 0, 255, 0);

}

//Timer interrupt handler for led matrix update. Frequency is 800 Hz
void TickHandler(void *CallBackRef){
	//Don't remove this
	uint32_t StatusEvent;

	// Exceptions must be disabled when updating screen
	Xil_ExceptionDisable();



	//****Write code here ****
	CHANNEL = 0;
	run(current_channel);
	open_line(current_channel);
	current_channel = (current_channel + 1)%8;



	//****END OF OWN CODE*****************

	//*********clear timer interrupt status. DO NOT REMOVE********
	StatusEvent = XTtcPs_GetInterruptStatus((XTtcPs *)CallBackRef);
	XTtcPs_ClearInterruptStatus((XTtcPs *)CallBackRef, StatusEvent);
	//*************************************************************
	//enable exceptions
	Xil_ExceptionEnable();
}


//Timer interrupt for moving alien, shooting... Frequency is 10 Hz by default
void TickHandler1(void *CallBackRef){

	//Don't remove this
	uint32_t StatusEvent;

	//****Write code here ****
	if (!run_game) Win();

    if (run_game && alien_alive) {
        SetPixel(alien_x, alien_y, 0, 0, 0); // Turn off alien pixel
    }

    // Update alien position
    if (run_game && alien_alive) {
        alien_x = (alien_x + 1) % 8; // Move right, wrap at edge
    }

    // Clear old bullet position
    if (run_game && bullet_active) {
        SetPixel(bullet_x, bullet_y, 0, 0, 0); // Turn off bullet pixel
        bullet_y--; // Move the bullet upward
        if (bullet_y > 7) { // If bullet goes out of bounds, deactivate it
            bullet_active = 0;
        }
    }

    // Detect collision (bullet hits alien)
    if (run_game && bullet_active && alien_alive &&
        bullet_x == alien_x && bullet_y == alien_y) {
        alien_alive = 0; // Alien is destroyed
        bullet_active = 0; // Bullet disappears

        // Respawn alien at a random position
        alien_y = rand() % 4; // Avoid bottom rows where the ship is
        alien_alive = 1;

        // Points update
        points++;
        SetPixel(0, 6 - points, 255, 255, 0);
    }

    // Draw the alien in its new position
    if (run_game && alien_alive) {
        SetPixel(alien_x, alien_y, 255, 0, 0); // Red alien
    }

    // Draw the bullet in its new position
    if (run_game && bullet_active) {
        SetPixel(bullet_x, bullet_y, 0, 0, 255); // Blue bullet
    }

    if (points == 2){
    	Reset_Game();
    	Win();
    	run_game = 0;
    }



	//****END OF OWN CODE*****************
	//clear timer interrupt status. DO NOT REMOVE
	StatusEvent = XTtcPs_GetInterruptStatus((XTtcPs *)CallBackRef);
	XTtcPs_ClearInterruptStatus((XTtcPs *)CallBackRef, StatusEvent);

}


//Interrupt handler for switches and buttons.
//Reading Status will tell which button or switch was used
//Bank information is useless in this exercise
void ButtonHandler(void *CallBackRef, u32 Bank, u32 Status){
	//****Write code here ****

	//Hint: Status==0x01 ->btn0, Status==0x02->btn1, Status==0x04->btn2, Status==0x08-> btn3, Status==0x10->SW0, Status==0x20 -> SW1

	//If true, btn0 was used to trigger interrupt
    if (run_game && Status == 0x02) {
        if (!bullet_active) { // Only fire if no bullet is active
            bullet_x = ship_x + ship_width / 2; // Center bullet on ship
            bullet_y = 5; // Start bullet from the ship's row
            bullet_active = 1;
        }
    }

    if (Status == 0x01){
    	Reset_Game();
    }

    // If btn2 is pressed, move ship right
    if (run_game && Status == 0x04) {
        if (ship_x + ship_width < 8) { // Prevent going out of bounds
            SetPixel(ship_x, 7, 0, 0, 0); // Clear first pixel
            ship_x++; // Move ship right
            SetPixel(ship_x, 6, 0, 0, 0);
            draw_init();
        }
    }

    // If btn3 is pressed, move ship left
    if (run_game && Status == 0x08) {
        if (ship_x > 0) { // Prevent going out of bounds
            SetPixel(ship_x + ship_width - 1, 7, 0, 0, 0); // Clear last pixel
            ship_x--; // Move ship left
            SetPixel(ship_x + ship_width - 1, 6, 0, 0, 0);
            draw_init();
        }
    }


	//****END OF OWN CODE*****************
}


