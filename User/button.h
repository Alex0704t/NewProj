#ifndef __BUTTON_H
#define __BUTTON_H

//#include "../Device/stm32f4xx.h"
#include "main.h"


#define BUTT_DELAY() delay_ms(35)
#define PRESS_USER_BUTTON ((GPIOA->IDR & GPIO_IDR_IDR_0) == GPIO_IDR_IDR_0)
#define PRESS_BUTTON_1    ((GPIOC->IDR & GPIO_IDR_IDR_8) == GPIO_IDR_IDR_8)
#define PRESS_BUTTON_2    ((GPIOC->IDR & GPIO_IDR_IDR_9) == GPIO_IDR_IDR_9)

#define USE_LONG_PRESS

#define CONT_BOUNCE_TIME_MS 25
#define SHORT_PRESS_TIME_MS 1500

enum button
{user_button, button_1, button_2};

enum pressing
{no_press, short_press, long_press};

void Button_Init(void);
void ButtINT_Init(void);

uint8_t Get_Button(uint8_t button);
void Butt_Count();

#endif
