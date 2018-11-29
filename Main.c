/*******************************************************************
 * Main.c
 * Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
 * Adapted by Thomas Royko, Gursel Serpen
 * Runs on TM4C123
 * 
 * Instructor: ***fill this in***
 * Student: ***fill this in***
 * Section: ***fill this in***
 * Date:    ***fill this in***
 *
 * 16-bit color, 128 wide by 160 high LCD
 *   Backlight (pin 10) connected to +3.3 V
 *   MISO (pin 9) unconnected 
 *   SCK (pin 8) connected to PA2 (SSI0Clk)
 *   MOSI (pin 7) connected to PA5 (SSI0Tx)
 *   TFT_CS (pin 6) connected to PA3 (SSI0Fss)
 *   CARD_CS (pin 5) unconnected
 *   Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
 *   RESET (pin 3) connected to PA7 (GPIO)
 *   VCC (pin 2) connected to +3.3 V
 *   Gnd (pin 1) connected to ground
 *******************************************************************/

#define GPIO_PORTA_DATA_R       (*((volatile uint32_t *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile uint32_t *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile uint32_t *)0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile uint32_t *)0x4000451C))
#define GPIO_PORTA_AMSEL_R      (*((volatile uint32_t *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile uint32_t *)0x4000452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"
#include "Images.h"
#include "../inc/tm4c123gh6pm.h"

typedef enum { false, true } bool;

extern void Delay1ms(uint32_t n);
void IO_Init(void);
void IO_HeartBeat(void);
void IO_Touch(void);
void portA_init(void);

int main(void){
	char *names="Will and Joy";
	char *phrase="Hello World!";
  PLL_Init(); // set system clock to 80 MHz
  IO_Init();
  
  // test DrawChar() and DrawCharS()
  ST7735_InitR(INITR_REDTAB);
	unsigned long input = 0x00000000;
	unsigned short flags = 0x6;
	unsigned int temp;
	unsigned short count=0;
  while(1){
		input=GPIO_PORTE_DATA_R;
		flags |=input&0x1;
		if (flags&0x1)																		//This whole code block handles breakpoint mode
		{
			GPIO_PORTE_DATA_R|=0x8;
			if (input&0x1)																	//wait for the breakpoint button to be pressed
			{
				ST7735_FillScreen(ST7735_BLACK);							//clear the screen
			ST7735_DrawString(1,2,"BREAKPOINT MODE",ST7735_RED);	//write "BREAKPOINT MODE" in the middle of the screen. for extended troubleshooting
			IO_Touch();																			//wait until the breakpoint button is realeased
			if ((input&0x2) && (count < 3))									//check the state of the first button
			{
				if (count==1)																	//write the names on the first press
				{
					ST7735_DrawString(1,1,names,ST7735_RED);
					count++;
				}
				else if (count==2)														//write the phrase on the second press
				{
					ST7735_DrawString(1,14,phrase,ST7735_RED);
					Delay1ms(20000);														//wait 2 seconds and get clear the catchphrase
					ST7735_DrawString(1,14,"                      ", ST7735_RED);
					if (!(input&0x4))														//check if we're done with BPM, and if so clear the status flags
					{
						flags &=0xE;
						count=0;
					}
				}
				
			}
			if (input&0x4)															//Check the second button
			{
				if (count==1 || count==3)									//Only draw the picture on pressed "2" and "4" (we've got it tracking at 1-n)
				{
					ST7735_DrawBitmap(35,115,parrot,49,75);	//draw the picture
					Delay1ms(20000);												//wait
					ST7735_DrawBitmap(35,115,blank,49,75);	//clear the picure
					flags &=0xE;														//clear the flags, it always breaks out of BPM after the picture
					count=0;
				}
			}
			if (count ==0) count++;											//advance the counter on the first iteration, as it can't do anything else
			if (count > 3) count=0;											//clear it just in case something escapes and we get an invalid value
		}
		}
		
		else{																					//This handles the normal behavior
			ST7735_DrawString(1,2,"               ",ST7735_RED);
			if (input&0x2 && !(flags&0x0002))							//FLAGS: ensures that the it'll only write a command to the screen if there was a change in state. 
		{																								//there's no reason to redraw the same thing however many times a second it updates.
			flags |=0x2;																	//This block checks the first button's state
			ST7735_DrawString(1,1,names, ST7735_RED);
			ST7735_DrawString(1,14,phrase,ST7735_RED);
		}
		if (!(input&0x2) && (flags&0x2))								// if the button was pressed and now is not, clear the names and phrase
		{
				flags &=0xFD;																//clear the flags to let the system know you're on the same page
			ST7735_DrawString(1,1,"            ", ST7735_RED);
			ST7735_DrawString(1,14,"                      ", ST7735_RED);
		}
		if (input&0x4 && !(flags&0x0004))								//if the button wasn't pressed and now it is
		{
			flags |=0x4;																	//set the flags to let the system know you're on the same page
			ST7735_DrawBitmap(35,125, parrot, 49,75);			//draw the picture
		}
		if (!(input&0x4) && (flags&0x4))								//if the button was pressed and now it's not
		{
				flags &=0xFB;																//clear the flags
			ST7735_DrawBitmap(35,125, blank, 49, 75);			//and get rid of the picture
		}
		IO_HeartBeat();																	//toggle the heartbeat
	}

	Delay1ms(1000);	
	}

}

void portA_init(void)																//initalizes port A just like we've been doing for this whole semester
{
	int delay;
	SYSCTL_RCGC2_R|=1;
	delay=SYSCTL_RCGC2_R;
	GPIO_PORTA_AFSEL_R=0;
	GPIO_PORTA_AMSEL_R=0;
	GPIO_PORTA_PCTL_R=0;
	GPIO_PORTA_DIR_R=0xEC;
	GPIO_PORTA_DEN_R=0xEC;
}
