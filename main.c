//--------------------------------------------------------------
// File     : main.c
//--------------------------------------------------------------

#include "main.h"


int main()
{
  MCU_Init();





  while(1)
  {

  Main_menu();


  }
}

void MCU_Init()
{
  Clock_Init();
  //GPIO_Clock_En();
  Led_Init();
  ButtINT_Init();
  Enc_Tim3_Init();
  Tim4_OC_Init();
  PCF8812_Init();
  RTC_Init();
  PCF8812_Test();
  SCB->CPACR |= 0x00F00000;//enable FPU
  //Audio_Init();
  //AnalogWave_Init(1000, 30);
}

void FPU_IRQ_Handler(void)
{
  LED_ON(blue);
}
