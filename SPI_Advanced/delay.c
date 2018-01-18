#include "delay.h"

#define MILLS_ROUNDS 5520
#define MICRO_ROUNDS 5
 
void delay(unsigned int uiMilisecs){
 
    unsigned int uiCounter;
 
    for (uiCounter=0; uiCounter<MILLS_ROUNDS * uiMilisecs; uiCounter++) {}
}

void delay_micro(unsigned int uiMilisecs){
 
    unsigned int uiCounter;
 
		for (uiCounter=0; uiCounter<MICRO_ROUNDS * uiMilisecs; uiCounter++) {}
			
}
