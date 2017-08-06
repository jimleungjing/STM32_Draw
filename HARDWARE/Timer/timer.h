#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f4xx.h"


extern double frequency;
extern double duty;
extern double amplitude;
extern double risingedge;

/*------FUNCTION-----*/

void TIM2_Init(uint16_t arr,uint16_t psc);

#endif
