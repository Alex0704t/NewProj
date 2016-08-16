#include "button.h"



__IO uint8_t button_state[3] = {0};
__IO uint8_t button_count[3] = {0};
__IO uint8_t button_rev_count[3] = {0};
button_s butt[3] = {0};

void Button_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOCEN;//GPIOA & GPIOC clock enable
	GPIOA->MODER &= ~GPIO_MODER_MODER0;//Input PA0
	GPIOA->OTYPER = 0x00;//Push-pull
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0;//no pull-up & pull-down
	GPIOC->MODER &= ~(GPIO_MODER_MODER8|GPIO_MODER_MODER9);//Input PC8, PC9
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8|GPIO_PUPDR_PUPDR9);//no pull-up & pull-down PC8, PC9
}

void ButtINT_Init(void)
{
	Button_Init();
	// set EXTI registers bit 8 has effect to both 8 and 9 lines
	// bit 9 has no effect
	EXTI->IMR |= EXTI_IMR_MR0|EXTI_IMR_MR8;//no mask interrupt EXTI0, 8, 9
	EXTI->RTSR |= EXTI_RTSR_TR0|EXTI_RTSR_TR8;//rising trigger event EXTI0, 8, 9
  //EXTI->FTSR |= EXTI_FTSR_TR0|EXTI_FTSR_TR8;//falling trigger event EXTI0, 8, 9
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;//EXTI0 source PA0
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC;//EXTI8, 9 source PC8, PC9
	NVIC_SetPriorityGrouping(4);//4 field for priority group
	NVIC_SetPriority(EXTI0_IRQn, 4);
	NVIC_EnableIRQ(EXTI0_IRQn);//IRQ handler
	NVIC_SetPriority(EXTI9_5_IRQn, 4);
	NVIC_EnableIRQ(EXTI9_5_IRQn);//IRQ handler
}


uint8_t Get_Button(uint8_t button) {
  uint8_t ret = no_press;
  //wait until button is pressed or count exceed short press threshold
  while(Check_Button(button)\
      && button_count[button] <= SHORT_PRESS_LIMIT);
  //detect short press
  if(button_count[button] >=  CONTACT_BOUNCE_LIMIT\
      && button_count[button] <= SHORT_PRESS_LIMIT) {
      //if delay past long press elapsed
      if(button_rev_count[button] == 0)
        ret = short_press;
      else {
        ret = no_press;
        button_count[button] = 0;//reset button counter
      }

  }
  //detect long press
  else if(button_count[button] > SHORT_PRESS_LIMIT) {
      ret = long_press;
      button_rev_count[button] = PAST_L_PRESS_DELAY;//start delay past long press
  }
  if(ret) {
      PCF8812_On();//display on
      PCF8812_Butt_ind(button);//view indicator
      if(!Check_Button(button))
        button_count[button] = 0;//reset button counter
  }
  return ret;
}

void EXTI0_IRQHandler(void) {
	if((EXTI->PR & EXTI_PR_PR0) == EXTI_PR_PR0) {
      EXTI->PR |= EXTI_PR_PR0;//clear pending bit of set 1

    }
}

void EXTI9_5_IRQHandler(void) {
  if((EXTI->PR & EXTI_PR_PR8) == EXTI_PR_PR8) {
      EXTI->PR |= EXTI_PR_PR8;//clear pending bit of set 1

    }
  }


void Butt_Count() {
  if(PRESS_USER_BUTTON)
      button_count[user_button]++;
  if(PRESS_BUTTON_1)
      button_count[button_1]++;
  if(PRESS_BUTTON_2)
      button_count[button_2]++;
  if(button_rev_count[user_button] && PRESS_USER_BUTTON == 0)
      button_rev_count[user_button]--;
  if(button_rev_count[button_1] && PRESS_BUTTON_1 == 0)
      button_rev_count[button_1]--;
  if(button_rev_count[button_2] && PRESS_BUTTON_2 == 0)
      button_rev_count[button_2]--;
}

void Set_Button(uint8_t button, button_s* in) {
  butt[button] = (button_s){0};
  butt[button] = *in;
}

uint8_t Check_Button(uint8_t button) {
  return (button == user_button)  ? PRESS_USER_BUTTON :
         (button == button_1)     ? PRESS_BUTTON_1    :
         (button == button_2)     ? PRESS_BUTTON_2    : 0;
}

