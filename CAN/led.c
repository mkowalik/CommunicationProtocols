#include <LPC21xx.H>
#include "led.h"


void Led_Init(){
	IO1DIR |= 0xFF << 16;
}

void Led_ShowByte(unsigned int value){
	IO1CLR = (0xFF << 16);
	IO1SET = ((value & 0xFF) << 16);
}
