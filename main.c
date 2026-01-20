#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "LPC17xx.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "usbdmain.h"
#include "gallery.h"
#include "game.h"

#define __FI        1                      
#define __USE_LCD   0	

// Function declarations
void startScreen(void);
void delay(int);
void del(int);
void pause(int, int, int);
void printWelcome(void);
void mainPage(void);
void interact(void);
void mp3screen(void);

int selection=1;

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void){
	GLCD_Init();
  	LED_Init();                                
	KBD_Init();
	startScreen();
	delay(2);
	printWelcome();
	delay(10);
	interact();
}

void startScreen(){
	#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */
  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Red);
  GLCD_SetTextColor(White);
  GLCD_DisplayString(0, 0, __FI, "    Final Project   ");
  GLCD_DisplayString(1, 0, __FI, "    Carson Plett    ");
  GLCD_DisplayString(2, 0, __FI, "     #501042248     ");
	#endif
}

void delay(int uprBnd){
	long i, j, k;
	volatile long count;
	for(i=0; i<1000; ++i){
		for(j=0; j<1300; ++j){
			for(k=0; k<uprBnd; ++k)
				++count;
		}
	}
}

void del(int uprBnd){
	long i, j, k;
	volatile long count;
	for(i=0; i<500; ++i){
		for(j=0; j<1300; ++j){
			for(k=0; k<uprBnd; ++k)
				++count;
		}
	}
}

void pause(int uprBnd, int row, int column){
	#ifdef __USE_LCD
	int i;
	for(i=0; i<uprBnd; ++i){
		GLCD_DisplayString(row, column, __FI, " ");
		delay(1);
		GLCD_DisplayString(row, column, __FI, "_");
		delay(1);
		GLCD_DisplayString(row, column, __FI, " ");
	}
	#endif
}

void printWelcome(){
	#ifdef __USE_LCD
	delay(1);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(4, 3, __FI, "W_");
	delay(1);
	GLCD_DisplayString(4, 4, __FI, "e_");
	pause(3, 4, 5);
	GLCD_DisplayString(4, 5, __FI, "l_");
	delay(1);
	GLCD_DisplayString(4, 6, __FI, "c_");
	pause(1, 4, 7);
	GLCD_DisplayString(4, 7, __FI, "o_");
	delay(1);
	GLCD_DisplayString(4, 8, __FI, "m_");
	delay(1);
	GLCD_DisplayString(4, 9, __FI, "e_");
	pause(2, 4, 10);
	GLCD_DisplayString(4, 10, __FI, " _");
	delay(1);
	GLCD_DisplayString(4, 11, __FI, "t_");
	delay(2);
	GLCD_DisplayString(4, 12, __FI, "o_");
	delay(1);
	GLCD_DisplayString(4, 13, __FI, " ");
	GLCD_DisplayString(5, 3, __FI, "_");
	pause(3, 5, 3);
	GLCD_DisplayString(5, 3, __FI, "t_");
	delay(2);
	GLCD_DisplayString(5, 4, __FI, "h_");
	delay(1);
	GLCD_DisplayString(5, 5, __FI, "e_");
	pause(3, 5, 6);
	GLCD_DisplayString(5, 6, __FI, "._");
	delay(1);
	GLCD_DisplayString(5, 7, __FI, "._");
	delay(1);
	GLCD_DisplayString(5, 8, __FI, ".");
	delay(1);
	GLCD_DisplayString(7, 4, __FI, "M_");
	del(1);
	GLCD_DisplayString(7, 5, __FI, "E_");
	del(1);
	GLCD_DisplayString(7, 6, __FI, "D_");
	del(1);
	GLCD_DisplayString(7, 7, __FI, "I_");
	del(1);
	GLCD_DisplayString(7, 8, __FI, "A ");
	del(1);
	GLCD_DisplayString(7, 10, __FI, "C_");
	del(1);
	GLCD_DisplayString(7, 11, __FI, "E_");
	del(1);
	GLCD_DisplayString(7, 12, __FI, "N_");
	del(1);
	GLCD_DisplayString(7, 13, __FI, "T_");
	del(1);
	GLCD_DisplayString(7, 14, __FI, "E_");
	del(1);
	GLCD_DisplayString(7, 15, __FI, "R!");
	del(1);
	#endif
}

void mainPage(){
	#ifdef __USE_LCD
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Black);
  GLCD_DisplayString(1, 1, __FI, "(1) Photo Gallery");
  GLCD_DisplayString(2, 1, __FI, "(2) MP3 Player   ");
  GLCD_DisplayString(3, 1, __FI, "(3) Game Centre  ");
	GLCD_DisplayString(6, 1, __FI, "UP/DOWN to toggle");
	GLCD_DisplayString(7, 1, __FI, "SELECT to enter");
	#endif
}

int32_t current_direction;								 					
bool loop;

void interact(){
		GLCD_Clear(White);
		mainPage();
		KBD_Init();								 					
		loop=true;
		
		while (loop) {                        
		
			current_direction = get_button();
		
		if(current_direction == KBD_SELECT){
			switch(selection){
				case 1:
					delay(1);
					loop=false;
					photoIntro();
					break;
				case 2:
					mp3screen();
					delay(1);
					loop=false;
					mp3main();
					break;
				case 3:
					delay(1);
					loop=false;
					gameIntro();
					break;
			}
		}
		else{
			if(current_direction == KBD_UP){
				if(selection==1)
					selection=3;
				else
					--selection;
				delay(1);
			}
			else{
				if(current_direction == KBD_RIGHT){
					//do nothing yet, need to do something soon
			  }
				else{
					if(current_direction == KBD_DOWN){
						if(selection==3)
							selection=1;
						else
							++selection;
						delay(1);
					}
					else{
						if(current_direction ==KBD_LEFT){
							//do nothin, no exit needed
						}
						else{
							switch (selection){
								case 1:
									#ifdef __USE_LCD
									GLCD_SetBackColor(White);
									GLCD_SetTextColor(Black);
									GLCD_DisplayString(2, 1, __FI, "(2) MP3 Player   ");
									GLCD_DisplayString(3, 1, __FI, "(3) Game Centre  ");
									GLCD_SetBackColor(Black);
									GLCD_SetTextColor(White);
									GLCD_DisplayString(1, 1, __FI, "(1) Photo Gallery");
									#endif
									break;
								case 2:
									#ifdef __USE_LCD
									GLCD_SetBackColor(White);
									GLCD_SetTextColor(Black);
									GLCD_DisplayString(1, 1, __FI, "(1) Photo Gallery");
									GLCD_DisplayString(3, 1, __FI, "(3) Game Centre  ");
									GLCD_SetBackColor(Black);
									GLCD_SetTextColor(White);
									GLCD_DisplayString(2, 1, __FI, "(2) MP3 Player   ");
									#endif
									break;
								case 3:
									#ifdef __USE_LCD
									GLCD_SetBackColor(White);
									GLCD_SetTextColor(Black);
									GLCD_DisplayString(1, 1, __FI, "(1) Photo Gallery");
									GLCD_DisplayString(2, 1, __FI, "(2) MP3 Player   ");
									GLCD_SetBackColor(Black);
									GLCD_SetTextColor(White);
									GLCD_DisplayString(3, 1, __FI, "(3) Game Centre  ");
									#endif
									break;
							}
						}
					}
				} 
			}
		}
  }
}

void mp3screen(void){
	GLCD_Clear(Black);
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(3, 2, __FI, "Welcome to the");
	GLCD_DisplayString(4, 2, __FI, "mp3 Player");
	GLCD_DisplayString(5, 2, __FI, "LEFT to exit");
}

