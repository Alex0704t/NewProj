#ifndef __BUTTON_H
#define __BUTTON_H

//#include "../Device/stm32f4xx.h"
#include "main.h"


#define BUTT_DELAY 5
#define USER_BUTTON ((GPIOA->IDR & GPIO_IDR_IDR_0) == GPIO_IDR_IDR_0)
#define BUTTON_1    ((GPIOC->IDR & GPIO_IDR_IDR_8) == GPIO_IDR_IDR_8)
#define BUTTON_2    ((GPIOC->IDR & GPIO_IDR_IDR_9) == GPIO_IDR_IDR_9)

//#define USE_LONG_PRESS

enum button
{user_button, button_1, button_2};

enum curcuit
{no_press, short_press, long_press};

void Button_Init(void);
void ButtINT_Init(void);

uint8_t Get_Button(uint8_t button);

#endif
