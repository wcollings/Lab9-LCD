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

void Delay1ms(uint32_t n);
void IO_Init(void);
void IO_HeartBeat(void);
void IO_Touch(void);
void portA_init(void);

int main(void){
	char *intro="Hello";
  PLL_Init(); // set system clock to 80 MHz
  IO_Init();
  
  // test DrawChar() and DrawCharS()
  ST7735_InitR(INITR_REDTAB);
	ST7735_DrawBitmap(35,125,parrot, 49,75);
	//unsigned long in = 0x00000000;
	//bool breakEnabled = false;
	
  while(1){
		//Input and display code goes here
		//See assignment for psuedo code
		
  }
}

void portA_init(void)
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
