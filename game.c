#include <stdio.h>
#include "main.h"
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h" 
#include "KBD.h"
#include <stdbool.h>
#include "game.h"
#include "bankRobberGT.h"

#define __FI        1                    
#define __USE_LCD   0	

bool isEmpty[3][3] = {
   {true, true, true},
   {true, true, true},
   {true, true, true}
};
bool hasX[3][3] = {
	{false, false, false},
	{false, false, false},
	{false, false, false}
};
bool hasO[3][3] = {
	{false, false, false},
	{false, false, false},
	{false, false, false}
};

void gameInteract(void);
void ticTacToe(void);
void playTicTacToe(void);
void updateDisplay(int vertical, int horizontal);
void checkX(void);
void checkO(void);
void refresh(void);

int32_t joy_direction;

void gameIntro(){
	GLCD_Clear(Blue);
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(1, 2, __FI, "Welcome to the");
	GLCD_DisplayString(2, 2, __FI, "GAME CENTER");
	GLCD_DisplayString(6, 1, __FI, "UP/DOWN to toggle");
	GLCD_DisplayString(7, 1, __FI, "SELECT to enter");
	GLCD_DisplayString(8, 1, __FI, "LEFT to exit");
	del(45);
	gameInteract();
}

bool looop;
int choice;

void gameInteract(){
	GLCD_Clear(Blue);
	GLCD_SetBackColor(Blue);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(1, 1, __FI, "Select game:");
	GLCD_DisplayString(3, 1, __FI, "(1) Tic Tac Toe   ");
	GLCD_DisplayString(4, 1, __FI, "(2) Bank Robber GT");
	KBD_Init();
	choice=1;
	looop=true;
	while (looop) {                                /* looop forever                  */
		
	joy_direction = get_button();
		
	if(joy_direction == KBD_SELECT){
		switch(choice){
			case 1:
				looop=false;
				ticTacToe();
				break;
			case 2:
				looop=false;
				bankRobber();
				break;
		}
	}
		else{
			if(joy_direction == KBD_UP){
				if(choice==1)
					choice=2;
				else
					--choice;
				delay(1);
			}
			else{
				if(joy_direction == KBD_RIGHT){
					//do nothing yet, need to do something soon
			  }
				else{
					if(joy_direction == KBD_DOWN){
						if(choice==2)
							choice=1;
						else
							++choice;
						delay(1);
					}
					else{
						if(joy_direction ==KBD_LEFT){
							delay(1);
							looop=false;
							interact();
						}
						else{
							switch (choice){
								case 1:
									#ifdef __USE_LCD
									GLCD_SetBackColor(Blue);
									GLCD_SetTextColor(White);
									GLCD_DisplayString(4, 1, __FI, "(2) Bank Robber GT");
									GLCD_SetBackColor(Green);
									GLCD_SetTextColor(Red);
									GLCD_DisplayString(3, 1, __FI, "(1) Tic Tac Toe   ");
									#endif
									break;
								case 2:
									#ifdef __USE_LCD
									GLCD_SetBackColor(Blue);
									GLCD_SetTextColor(White);
									GLCD_DisplayString(3, 1, __FI, "(1) Tic Tac Toe   ");
									GLCD_SetBackColor(Green);
									GLCD_SetTextColor(Red);
									GLCD_DisplayString(4, 1, __FI, "(2) Bank Robber GT");
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

void ticTacToe(){
	GLCD_Clear(White);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(1, 4, __FI, "Tic Tac Toe");
	GLCD_DisplayString(3, 7, __FI, "|   |");
	GLCD_DisplayString(4, 4, __FI, "-----------");
	GLCD_DisplayString(5, 7, __FI, "|   |");
	GLCD_DisplayString(6, 4, __FI, "-----------");
	GLCD_DisplayString(7, 7, __FI, "|   |");
	delay(10);
  refresh();
	playTicTacToe();
}

void refresh(){
	int i, j;
	for(i=0;i<3;++i){
		for(j=0;j<3;++j){
			hasX[i][j]=false;
			hasO[i][j]=false;
			isEmpty[i][j]=true;
		}
	}
}

bool xPlay, newInput;

void playTicTacToe(){
	int horizontal=4, vertical=3;
	KBD_Init();
	xPlay = true;
	looop=true;
	newInput=true;
	while(looop){
		joy_direction = get_button();
		
		if(joy_direction == KBD_RIGHT){
			if(horizontal==12)
				horizontal=4;
			else
				horizontal+=4;
			newInput=true;
			delay(1);
		}
		else{
			if(joy_direction == KBD_LEFT){
				if(horizontal==4)
					horizontal=12;
				else
					horizontal-=4;
				newInput=true;
				delay(1);
			}
			else{
				if(joy_direction == KBD_DOWN){
					if(vertical == 7)
						vertical=3;
					else
						vertical+=2;
					newInput=true;
					delay(1);
				}
				else{
					if(joy_direction == KBD_UP){
						if(vertical==3)
							vertical=7;
						else
							vertical-=2;
							newInput=true;
							delay(1);
					}
					else{
						if(joy_direction==KBD_SELECT){
							int y = (horizontal/4)-1;
							int x = ((vertical-1)/2)-1;
							if(xPlay && (isEmpty[x][y]==true)){
								hasX[x][y]=true;
								isEmpty[x][y]=false;
								newInput=true;
								xPlay=false;
								checkX();
							}
							else{
								if(!xPlay && (isEmpty[x][y]==true)){
									hasO[x][y]=true;
									isEmpty[x][y]=false;
									newInput=true;
									xPlay=true;
									checkO();
								}
							}
							del(1);
						}
						else{
							if(newInput){
								updateDisplay(vertical, horizontal);
								newInput=false;
							}
						}
					}
				}
			}
		}
	}
}

int row, i, j;

void updateDisplay(int vertical, int horizontal){
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(3, 4, __FI, "   |   |   ");
	GLCD_DisplayString(4, 4, __FI, "-----------");
	GLCD_DisplayString(5, 4, __FI, "   |   |   ");
	GLCD_DisplayString(6, 4, __FI, "-----------");
	GLCD_DisplayString(7, 4, __FI, "   |   |   ");		
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(White);
	row=0;
	for(i=3; i<8; i+=2){
		int column=0;
		for(j=4; j<13; j+=4){
			if(hasX[row][column]==true){
				GLCD_SetBackColor(White);
				GLCD_SetTextColor(Green);
				GLCD_DisplayString(i, j, __FI, " X ");
			}
			if(hasO[row][column]==true){
				GLCD_SetBackColor(White);
				GLCD_SetTextColor(Red);
				GLCD_DisplayString(i, j, __FI, " O ");
			}
			if(i==vertical && j==horizontal){
				GLCD_SetBackColor(Black);
				GLCD_SetTextColor(White);
				if(hasX[row][column]==true)
					GLCD_DisplayString(i, j, __FI, " X ");
				else{
					if(hasO[row][column]==true)
						GLCD_DisplayString(i, j, __FI, " O ");
					else
						GLCD_DisplayString(i, j, __FI, "   ");
				}
			}
			++column;
		}
		++row;
	}
}

void checkX(){
	int i;
	bool win=false;
	for(i=0; i<3; ++i){
		if(hasX[0][i]==true && hasX[1][i]==true && hasX[2][i]==true) win=true;
		if(hasX[i][0]==true && hasX[i][1]==true && hasX[i][2]==true) win=true;
	}
	if(hasX[0][0]==true && hasX[1][1]==true && hasX[2][2]==true) win=true;
	if(hasX[0][2]==true && hasX[1][1]==true && hasX[2][0]==true) win=true;
	
	if(win){
		delay(5);
		GLCD_Clear(White);
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(4, 6, __FI, "X Wins!");
		delay(5);
		interact();
	}
}

void checkO(){
	int i;
	bool win=false;
	for(i=0; i<3; ++i){
		if(hasO[0][i]==true && hasO[1][i]==true && hasO[2][i]==true) win=true;
		if(hasO[i][0]==true && hasO[i][1]==true && hasO[i][2]==true) win=true;
	}
	if(hasO[0][0]==true && hasO[1][1]==true && hasO[2][2]==true) win=true;
	if(hasO[0][2]==true && hasO[1][1]==true && hasO[2][0]==true) win=true;
	
	if(win){
		delay(5);
		GLCD_Clear(White);
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(4, 6, __FI, "O Wins!");
		delay(5);
		interact();
	}
}
