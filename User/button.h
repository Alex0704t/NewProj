#ifndef __BUTTON_H
#define __BUTTON_H

//#include "../Device/stm32f4xx.h"
#include "main.h"

#define BUTT_DELAY()        delay_ms(35)
#define PRESS_USER_BUTTON   ((GPIOA->IDR & GPIO_IDR_IDR_0) == GPIO_IDR_IDR_0)
#define PRESS_BUTTON_1      ((GPIOC->IDR & GPIO_IDR_IDR_8) == GPIO_IDR_IDR_8)
#define PRESS_BUTTON_2      ((GPIOC->IDR & GPIO_IDR_IDR_9) == GPIO_IDR_IDR_9)


#define CONTACT_BOUNCE_LIMIT  2
#define SHORT_PRESS_LIMIT     25
#define PAST_L_PRESS_DELAY    5

enum button
{user_button, button_1, button_2};

enum pressing
{no_press, short_press, long_press};



void Button_Init(void);
void ButtINT_Init(void);

typedef void (*p_func)(void);

typedef struct {
  uint8_t* name;
  p_func short_act;
  p_func long_act;
  uint8_t repeat;
  uint8_t state;
  uint8_t count;
}button_s;



uint8_t Get_Button(uint8_t button);
uint8_t Check_Button(uint8_t button);
void Butt_Count();

void Set_Button(uint8_t button, button_s* in);

#endif
