//--------------------------------------------------------------
// File     : usb_bsp.c
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usb_bsp.h"
#include "usbd_conf.h"


extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);



//--------------------------------------------------------------
void USB_OTG_BSP_Init()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  /* Configure  SOF VBUS ID DM DP Pins */
  GPIOA->MODER |= GPIO_MODER_MODER8_1|GPIO_MODER_MODER9_1|GPIO_MODER_MODER11_1\
      |GPIO_MODER_MODER12_1;//PA10-12 AF mode
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8_1|GPIO_OSPEEDER_OSPEEDR9_1\
      |GPIO_OSPEEDER_OSPEEDR10_1|GPIO_OSPEEDER_OSPEEDR11_1|GPIO_OSPEEDER_OSPEEDR12_1;
  GPIOA->AFR[1] |= 0x000AA0AA;
  GPIOA->OTYPER &=  ~(GPIO_OTYPER_OT_8|GPIO_OTYPER_OT_9\
      |GPIO_OTYPER_OT_11|GPIO_OTYPER_OT_12);
  GPIOA->OTYPER |= GPIO_OTYPER_OT_10;
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR8|GPIO_PUPDR_PUPDR9\
      |GPIO_PUPDR_PUPDR11|GPIO_PUPDR_PUPDR12);
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR10;

  RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
  /* enable the PWR clock */
  RCC->APB1RSTR |= RCC_APB1RSTR_PWRRST;
}

//--------------------------------------------------------------
void USB_OTG_BSP_EnableInterrupt()
{
  NVIC_SetPriority(OTG_FS_IRQn, 4);
  NVIC_EnableIRQ(OTG_FS_IRQn);//IRQ handler

}

//--------------------------------------------------------------
void USB_OTG_BSP_uDelay (const uint32_t usec)
{
  uint32_t count = 0;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}


//--------------------------------------------------------------
void USB_OTG_BSP_mDelay (const uint32_t msec)
{
  USB_OTG_BSP_uDelay(msec * 1000);   
}


//--------------------------------------------------------------
void OTG_FS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}




