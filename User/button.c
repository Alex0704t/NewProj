#include "button.h"


__IO uint8_t button_state[3] = {0};

#ifdef NEED_LONG_PRESS
__IO uint32_t button_count[3] = {0};
#endif

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
	EXTI->IMR |= EXTI_IMR_MR0|EXTI_IMR_MR8/*|EXTI_IMR_MR9*/;//no mask interrupt EXTI0, 8, 9
	EXTI->RTSR |= EXTI_RTSR_TR0|EXTI_RTSR_TR8/*|EXTI_RTSR_TR9*/;//rising trigger event EXTI0, 8, 9
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;//EXTI0 source PA0
	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC/*|SYSCFG_EXTICR3_EXTI9_PC*/;//EXTI8, 9 source PC8, PC9
	NVIC_SetPriorityGrouping(4);//4 field for priority group
	NVIC_SetPriority(EXTI0_IRQn, 4);
	NVIC_EnableIRQ(EXTI0_IRQn);//IRQ handler
	NVIC_SetPriority(EXTI9_5_IRQn, 4);
	NVIC_EnableIRQ(EXTI9_5_IRQn);//IRQ handler
}


uint8_t Get_Button(uint8_t button) {
#ifndef NEED_LONG_PRESS
  uint8_t ret = 0;
  if(button_state[button]) {//if button pressed
      ret = 1;//return value 1
      PCF8812_Butt_ind(button);//view indicator
      PCF8812_On();//display on
  }
  delay_ms(BUTT_DELAY);//delay to eliminate contact bounce
  button_state[button] = 0;//reset button state
  return ret;
#else



#endif
}

void EXTI0_IRQHandler(void) {
  uint32_t temp = 0;
	if((EXTI->PR & EXTI_PR_PR0) == EXTI_PR_PR0) {
      EXTI->PR |= EXTI_PR_PR0;//clear pending bit of set 1
      button_state[user_button] = 1;
    }
}

void EXTI9_5_IRQHandler(void) {
  if((EXTI->PR & EXTI_PR_PR8) == EXTI_PR_PR8)
    {
      EXTI->PR |= EXTI_PR_PR8;//clear pending bit of set 1
      if(BUTTON_1)
        button_state[button_1] = 1;
      if(BUTTON_2)
        button_state[button_2] = 1;
    }
  /*
  if(((EXTI->PR & EXTI_PR_PR9) == EXTI_PR_PR9) || BUTTON)
    {
      EXTI->PR |= EXTI_PR_PR9;//clear pending bit of set 1
      button_state[button_2] = 1;
    }
    */
}
