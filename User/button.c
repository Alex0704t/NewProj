#include "button.h"
#include "pcf8812.h"


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
  butt[button].state = button_released;
  //wait until button is pressed or count exceed short button_press threshold
  while(Check_Button(button) \
      && butt[button].count <= SHORT_PRESS_LIMIT);
  //detect short button_press
  if(butt[button].count >=  CONTACT_BOUNCE_LIMIT \
      && butt[button].count <= SHORT_PRESS_LIMIT) {
      //if delay past long button_press elapsed
      if(butt[button].rev_count == 0)
        butt[button].state = button_pressed;
      else {
        butt[button].state = button_released;
        butt[button].count = 0;//reset button counter
      }
  }
  //detect long button_press
  else if(butt[button].count > SHORT_PRESS_LIMIT) {
      butt[button].state = button_hold;
      butt[button].rev_count = PAST_HOLD_DELAY;//start delay past long button_press
  }
  if(butt[button].state) {
      PCF8812_On();//display on
      PCF8812_Butt_ind(button);//view indicator
      if(!Check_Button(button))
        butt[button].count = 0;//reset button counter
  }
  return butt[button].state;



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
  if(PRESS_USER_BUTTON) {
      if(butt[user_button].rev_count)
        butt[user_button].rev_count--;
      else
        butt[user_button].count++;
  }
  if(PRESS_BUTTON_1) {
      if(butt[button_1].rev_count)
        butt[button_1].rev_count--;
      else
        butt[button_1].count++;
  }
  if(PRESS_BUTTON_2) {
      if(butt[button_2].rev_count)
        butt[button_2].rev_count--;
      else
        butt[button_2].count++;
  }
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

