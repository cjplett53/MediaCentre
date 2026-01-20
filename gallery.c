#include <stdio.h>
#include "main.h"
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include <stdbool.h>
//#include "game.h"

#define __FI        1                    
#define __USE_LCD   0	

void photoMain(void);

int32_t joystick_direction;
int pic=0;

extern unsigned char seal1[];
extern unsigned char seal2[];
extern unsigned char polarbear[];

void photoIntro(){
	#ifdef __USE_LCD
	GLCD_Clear(White);
	GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(2, 1, __FI, "Welcome to the");
  GLCD_DisplayString(3, 1, __FI, "Photo Gallery");
	GLCD_DisplayString(6, 1, __FI, "SELECT toggles");
	GLCD_DisplayString(7, 1, __FI, "through photos");
	GLCD_DisplayString(8, 1, __FI, "LEFT to exit");
	delay(20);
	photoMain();
	#endif	
}

void photoMain(){
	#ifdef __USE_LCD
	GLCD_Clear(White);
	KBD_Init();
	for(;;){
		joystick_direction=get_button();
		if(joystick_direction==KBD_SELECT){
			if(pic++>2)pic=0;
			switch(pic){
				case 0:
					GLCD_Clear(White);
					GLCD_Bitmap (0, 27, 320, 186, seal1);
					break;
				case 1:
					GLCD_Clear(White);
					GLCD_Bitmap (0, 27, 320, 185, seal2);
					break;
				case 2:
					GLCD_Clear(White);
					GLCD_Bitmap (0, 20, 320, 201, polarbear);
					break;
			}
		}
		else{
			if(joystick_direction==KBD_LEFT){
				delay(1);
				interact();
			}
		}
	}
	#endif
}
