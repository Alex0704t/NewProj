//--------------------------------------------------------------
// File     : main.c
//--------------------------------------------------------------

#include "main.h"


int main()
{
  MCU_Init();
  Main_menu();

  while(1) {

      USB_Echo();

  }
}

void MCU_Init() {
  Clock_Init();
  //GPIO_Clock_En();
  Led_Init();
  Button_Init();
  Enc_Tim3_Init();
  Tim4_OC_Init();
  PCF8812_Init();
  RTC_Init();
  PCF8812_Test();
  UB_USB_CDC_Init();
  //SCB->CPACR |= 0x00F00000;//enable FPU
  //Audio_Init();
  //AnalogWave_Init(1000, 30);
}

void FPU_IRQ_Handler(void)
{
  LED_ON(blue);
}
