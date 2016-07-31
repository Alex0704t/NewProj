#include "button.h"

__IO uint8_t Butt_count = 0;
__IO uint8_t Butt_state = 0;
__IO uint8_t Butt_1 = open;
__IO uint8_t Butt_2 = open;
__IO uint8_t Butt_u = open;

void Button_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOCEN;//GPIOA & GPIOC clock enable
	GPIOA->MODER &= ~GPIO_MODER_MODER0;//Input PA0
	//GPIOA->OTYPER = 0x00;//Push-pull
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR0;//no pull-up & pull-down
	//GPIOA->OSPEEDR = 0x00;//Low speed
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

uint8_t Get_Button(uint8_t button)
{
  uint8_t state = 0;
  if(button == button_1)
  {
    if(Butt_1 == close)
    {
      state = 1;
      PCF8812_Butt_ind(button_1);
      PCF8812_On();
    }
    while(BUTTON_1 && state)
    {
      delay_ms(BUTT_DELAY);
    }
   Butt_1 = open;
   return state;
  }
  else if(button == button_2)
  {
    if(Butt_2 == close)
    {
      state = 1;
      PCF8812_Butt_ind(button_2);
      PCF8812_On();
    }
    while(BUTTON_2 && state)
    {
      delay_ms(BUTT_DELAY);
    }
   Butt_2 = open;
   return state;
  }
  else
    {
    if(Butt_u == close)
    {
      state = 1;
      PCF8812_Butt_ind(user_button);
      PCF8812_On();
    }
    while(USER_BUTTON && state)
    {
      delay_ms(BUTT_DELAY);
    }
   Butt_u = open;
   return state;
  }

}
/*
uint8_t Get_Button(uint8_t button)
{
	uint8_t state = 0;
	if(button == button_1)
	  {
	    if(BUTTON_1)
	      {
	        state = 1;
	        A55_Butt_ind(button_1);
	      }
      while(BUTTON_1 && state)
      {
        delay_ms(5);
      }
      return state;
	  }
	else if(button == button_2)
	    {
	      if(BUTTON_2)
	        {
	          state = 1;
	          A55_Butt_ind(button_2);
	        }
	      while(BUTTON_2 && state)
	      {
	        delay_ms(5);
	      }
	      return state;
	    }
	else
      {
        if(USER_BUTTON)
          {
            state = 1;
            A55_Butt_ind(user_button);
          }
        while(USER_BUTTON && state)
        {
          delay_ms(5);
        }
        return state;
      }

}
*/
void EXTI0_IRQHandler(void)
{
  uint32_t temp = 0;
	if((EXTI->PR & EXTI_PR_PR0) == EXTI_PR_PR0)
    {
      EXTI->PR |= EXTI_PR_PR0;//clear pending bit of set 1
      Butt_u = close;
    }
}

void EXTI9_5_IRQHandler(void)
{

  if((EXTI->PR & EXTI_PR_PR8) == EXTI_PR_PR8)
    {
      EXTI->PR |= EXTI_PR_PR8;//clear pending bit of set 1
      if(BUTTON_1)
        Butt_1 = close;
      if(BUTTON_2)
        Butt_2 = close;
    }
  /*
  if(((EXTI->PR & EXTI_PR_PR9) == EXTI_PR_PR9) || BUTTON)
    {
      EXTI->PR |= EXTI_PR_PR9;//clear pending bit of set 1
      Butt_2 = close;
    }
    */
}
