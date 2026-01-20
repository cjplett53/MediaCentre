#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "main.h"
#include "LPC17xx.h"                       
#include "GLCD.h"
#include "LED.h" 
#include "KBD.h"
#include "game.h"
#include "bankRobberGT.h"
//#include "cmsis_os.h"

#define __FI        1                    
#define __USE_LCD   0
#define start				5

typedef struct{
	int direction; 
	int row;
	int column;
} Police;
Police officers[start];
int policeCount=0;
bool blue=true;

int32_t cur_direction;

int playerRow=9;
int playerColumn=0;

int purse=0;
char cash[10];
bool moneyAvailable=false;
int moneyRow=0;
int moneyColumn=0;
int lasers[20];
int numLasers=0;
int maxPolice=5;

bool level1=true;
int level=1;

void playBankRobber(void);
void ref(void);
void display(void);
void antidisplay(void);
void dropMoney(void);
void checkMoney(void);
void shootLaser(void);
bool checkLaser(void);
void caughtByLaser(void);
void addCop(void);
void playWithCops(void);
void updateCop(void);
void moveCop(int);
void clearCop(int);
void displayCop(int);
void moneyRecover(int);
bool catchPlayer(void);
void caughtByPolice(void);
void changeD(void);

void bankRobber(){
	GLCD_Clear(Black);
	KBD_Init();
	GLCD_SetTextColor(White);
	GLCD_SetBackColor(Black);
	GLCD_DisplayString(4, 4, __FI, "Welcome to");
	delay(10);
	GLCD_DisplayString(5, 2, __FI, "Bank Robber GT");
	delay(10);
	playBankRobber();
}

bool loooop;
long count, count1, count2, count3;

void playBankRobber(){
	GLCD_Clear(Black);
	ref();
	display();
	loooop = true;
	count=0, count1=0, count2=0;
	while(loooop){
		cur_direction=get_button();
		if(cur_direction == KBD_UP){
			antidisplay();
			if(playerRow==0)
				playerRow=9;
			else
				--playerRow;
			display();
			del(1);
		}
		else{
			if(cur_direction == KBD_DOWN){
				antidisplay();
				if(playerRow==9)
					playerRow=0;
				else
					playerRow++;
				display();
				del(1);
			}
			else{
				if(cur_direction == KBD_LEFT){
					antidisplay();
					if(playerColumn==0)
						playerColumn=19;
					else
						playerColumn--;
					display();
					del(1);
				}
				else{
					if(cur_direction == KBD_RIGHT){
						antidisplay();
						if(playerColumn==19)
							playerColumn=0;
						else
							playerColumn++;
						display();
						del(1);
					}
				}
			}
		}//End of current_direction logic
		checkMoney();
		if(level1){
			if(checkLaser()){
				loooop=false;
				caughtByLaser();
			}
		}
		++count;
		if(count==100000){
			count=0;
			++count1;
			if(count1==10){
				dropMoney();
				count1=0;
				++count2;
				if(count2==2){
					if(level1){
						shootLaser();
						shootLaser();
					}
					else{
						addCop();
					}
					count2=0;
				}
			}
		}
	}
}

void ref(){
	playerRow=9;
	playerColumn=0;
	policeCount=0;
	numLasers=0;
	level1=true;
	moneyAvailable=false;
	purse=0;
	maxPolice=5;
	level=1;
}

void display(){
	GLCD_SetBackColor(White);
	GLCD_DisplayString(playerRow, playerColumn, __FI, " ");
	GLCD_SetTextColor(White);
	GLCD_SetBackColor(Black);
	sprintf(cash, "$%d", purse);
	GLCD_DisplayString(9, 15, __FI, (unsigned char *)cash);
}

void antidisplay(){
	GLCD_SetBackColor(Black);
	GLCD_DisplayString(playerRow, playerColumn, __FI, " ");
}

void dropMoney(){
	if(moneyAvailable)
		return;
	else{
		moneyRow=rand()%10;
		moneyColumn=rand()%20;
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(Yellow);
		GLCD_DisplayString(moneyRow, moneyColumn, __FI, "$");
		moneyAvailable=true;
	}
}

void checkMoney(){
	if(playerRow==moneyRow && playerColumn==moneyColumn){
		purse+=rand()%100;
		moneyRow=-1;
		moneyColumn=-1;
		moneyAvailable=false;
	}
}

void shootLaser(){
	int row = rand()%10;
	if(row==moneyRow)
		moneyAvailable=false;
	lasers[numLasers++]=row;
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(row, 0, __FI, (unsigned char*)"--------------------");
}

bool checkLaser(){
	bool caught = false;
	int i;
	for(i=0; i<numLasers; ++i){
		if(lasers[i]==playerRow){
			caught=true;
			break;
		}
	}
	if(caught){
		numLasers=0;
	}
	return caught;
}

void caughtByLaser(){
	GLCD_Clear(Black);
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(4, 4, __FI, "Argh! Those");
	GLCD_DisplayString(5, 1, __FI, "lasers are awful!");
	delay(20);
	GLCD_Clear(Black);
	GLCD_DisplayString(4, 3, __FI, "Uh oh! I think");
	GLCD_DisplayString(5, 2, __FI, "I heard a siren!");
	GLCD_DisplayString(6, 5, __FI, "The police");
	GLCD_DisplayString(7, 5, __FI, "are coming!");
	delay(20);
	level1=false;
	moneyAvailable=false;
	playWithCops();
}

Police p;

void addCop(){
	if(purse>500*level){
		maxPolice+=2;
		++level;
	}
	if(policeCount==maxPolice)
		return;
	p.direction=rand()%4;
	switch(p.direction){
		case 0:
			p.row=9;
			p.column=rand()%20;
			break;
		case 1:
			p.row=rand()%10;
			p.column=0;
			break;
		case 2:
			p.row=0;
			p.column=rand()%20;
			break;
		case 3:
			p.row=rand()%10;
			p.column=19;
			break;
		default:
			break;
	}
	officers[policeCount++]=p;
	updateCop();
}

int playerMoves;

void playWithCops(){
	GLCD_Clear(Black);
	moneyAvailable=false;
	display();
	loooop = true;
	count=0, count1=0, count2=0, count3=0;
	playerMoves=0;
	while(loooop){
		if(playerMoves<3){
			++playerMoves;
			cur_direction=get_button();
			if(cur_direction == KBD_UP){
				antidisplay();
				if(playerRow==0)
					playerRow=9;
				else
					--playerRow;
				display();
				del(1);
			}
			else{
				if(cur_direction == KBD_DOWN){
					antidisplay();
					if(playerRow==9)
						playerRow=0;
					else
						playerRow++;
					display();
					del(1);
				}
				else{
					if(cur_direction == KBD_LEFT){
						antidisplay();
						if(playerColumn==0)
							playerColumn=19;
						else
							playerColumn--;
						display();
						del(1);
					}
					else{
						if(cur_direction == KBD_RIGHT){
							antidisplay();
							if(playerColumn==19)
								playerColumn=0;
							else
								playerColumn++;
							display();
							del(1);
						}
					}
				}
			}//End of current_direction logic
		}
		checkMoney();
		++count;
		if(count==100000){
			count=0;
			++count1;
			if(count1==5){
				dropMoney();
				updateCop();
				if(catchPlayer()){
					loooop=false;
					caughtByPolice();
				}
				count1=0;
				++count2;
				if(count2==4){
					++count3;
					addCop();
					count2=0;
					if(count3==4){
						changeD();
						count3=0;
					}
				}
				playerMoves=0;
			}
		}
	}
}

void updateCop(){
	int i;
	for(i=0; i<policeCount; ++i){
		clearCop(i);
		moveCop(i);
		displayCop(i);
		moneyRecover(i);
	}
}

void clearCop(int i){
			GLCD_SetBackColor(Black);
			GLCD_DisplayString(officers[i].row, officers[i].column, __FI, (unsigned char *)" ");
}

void moveCop(int i){
		switch(officers[i].direction){
			case 0:
				if(officers[i].row==0){
					officers[i].row=0;
					officers[i].direction=2;
				}
				else
					officers[i].row--;
				break;
			case 1:
				if(officers[i].column==19){
					officers[i].column=19;
					officers[i].direction=3;
				}
				else
					officers[i].column++;
				break;
			case 2:
				if(officers[i].row==9){
					officers[i].row=9;
					officers[i].direction=0;
				}
				else
					officers[i].row++;
				break;
			case 3:
				if(officers[i].column==0){
					officers[i].column=0;
					officers[i].direction=1;
				}
				else
					officers[i].column--;
				break;
			default:
				break;
		}
}

void displayCop(int i){
	if(blue){
		GLCD_SetBackColor(Blue);
		blue=false;
	}
	else{
		GLCD_SetBackColor(Red);
		blue=true;
	}
	GLCD_DisplayString(officers[i].row, officers[i].column, __FI, (unsigned char *)" ");
}

void moneyRecover(int i){
	if((officers[i].row==moneyRow) && (officers[i].column==moneyColumn)){
		moneyAvailable=false;
		dropMoney();
	}
}

bool catchPlayer(){
	bool caught = false;
	int i;
	for(i=0; i<policeCount; ++i){
		if((officers[i].row==playerRow) && (officers[i].column==playerColumn)){
			caught=true;
			break;
		}
	}
	return caught;
}

void caughtByPolice(){
	GLCD_Clear(Black);
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(White);
	GLCD_DisplayString(4, 4, __FI, (unsigned char *)"Have fun in...");
	delay(10);
	GLCD_DisplayString(6, 6, __FI, (unsigned char *)"JAIL!!");
	delay(10);
	interact();
}

void changeD(){
	int i;
	for(i=0; i<policeCount; ++i){
		officers[i].direction=rand()%4;
	}
}
