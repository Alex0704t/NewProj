#ifndef __BUTTON_H
#define __BUTTON_H

//#include "../Device/stm32f4xx.h"
#include "main.h"


#define BUTT_DELAY 5
#define USER_BUTTON ((GPIOA->IDR & GPIO_IDR_IDR_0) == GPIO_IDR_IDR_0)
#define BUTTON_1    ((GPIOC->IDR & GPIO_IDR_IDR_8) == GPIO_IDR_IDR_8)
#define BUTTON_2    ((GPIOC->IDR & GPIO_IDR_IDR_9) == GPIO_IDR_IDR_9)

enum button
{clear_ind, button_1, button_2, user_button};

enum curcuit
{open, close};

void Button_Init(void);
void ButtINT_Init(void);

uint8_t Get_Button(uint8_t button);

#endif
