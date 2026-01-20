/*----------------------------------------------------------------------------

 * Name:    LED.c
 * Purpose: low level LED functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "LPC17xx.h"                         /* LPC17xx definitions           */
#include "LED.h"

const unsigned long led_mask[] = { 1UL<<28, 1UL<<29, 1UL<<31, 1UL<< 2,
                                   1UL<< 3, 1UL<< 4, 1UL<< 5, 1UL<< 6 };

volatile unsigned long * bit;
																	 
/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/

void LED_Init (void) {

  LPC_SC->PCONP     |= (1 << 15);            /* enable power to GPIO & IOCON  */

  LPC_GPIO1->FIODIR |= 0xB0000000;           /* LEDs on PORT1 are output      */
  LPC_GPIO2->FIODIR |= 0x0000007C;           /* LEDs on PORT2 are output      */
}

/*----------------------------------------------------------------------------
  Function that turns on requested LED
 *----------------------------------------------------------------------------*/
void LED_On (unsigned int num) {

  if (num < 3) LPC_GPIO1->FIOPIN |=  led_mask[num];
  else         LPC_GPIO2->FIOPIN |=  led_mask[num];
}

/*----------------------------------------------------------------------------
  Function that turns off requested LED
 *----------------------------------------------------------------------------*/
void LED_Off (unsigned int num) {

  if (num < 3) LPC_GPIO1->FIOPIN &= ~led_mask[num];
  else         LPC_GPIO2->FIOPIN &= ~led_mask[num];
}

/*----------------------------------------------------------------------------
  Function that outputs value to LEDs
 *----------------------------------------------------------------------------*/
void LED_Out(unsigned int value) {
  int i;

  for (i = 0; i < LED_NUM; i++) {
    if (value & (1<<i)) {
      LED_On (i);
    } else {
      LED_Off(i);
    }
  }
}

/*----------------------------------------------------------------------------
  illuminate LEDs via MASK method
 *----------------------------------------------------------------------------*/

void mask_LED(){
	LED_Out(0);	//Turns other LEDs off
	LPC_GPIO1->FIOPIN |= (9UL<<28);
	LPC_GPIO2->FIOPIN |= (1UL<<3);
}

/*----------------------------------------------------------------------------
  illuminate LEDs via FUNCTION method
 *----------------------------------------------------------------------------*/

void function_LED(){
	LED_Out(0); //Turns other LEDs off
	bit = &BitBand(&LPC_GPIO1->FIOPIN, 29);		
 *bit = 1;
	bit = &BitBand(&LPC_GPIO2->FIOPIN, 2);
 *bit = 1;
	bit = &BitBand(&LPC_GPIO2->FIOPIN, 4);
 *bit = 1;
}

/*----------------------------------------------------------------------------
  illuminate LEDs via BITBAND method
 *----------------------------------------------------------------------------*/

void bitband_LED(){
	LED_Out(0);
	FIOPIN1=1;
	FIOPIN2=1;
	FIOPIN3=1;
}
