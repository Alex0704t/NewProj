//--------------------------------------------------------------
// File     : usb.c
// CPU      : STM32F4
//--------------------------------------------------------------

#include "usb.h"

void USB_Echo(void)
{
  char buf[APP_TX_BUF_SIZE];//data receive buffer
  char str[APP_TX_BUF_SIZE] = "";
  USB_CDC_RXSTATUS_t check = RX_EMPTY;
  while(1)
  {
    PCF8812_Clear();
    PCF8812_Title("USB ECHO");
    PCF8812_Button("EXIT", "", "");
    if(UB_USB_CDC_GetStatus() == USB_CDC_CONNECTED)//check USB plug to PC
    {
		  check = UB_USB_CDC_ReceiveString(buf);//check data received via USB
		  if(check == RX_READY)
		  {
		    PCF8812_On();
		    strcpy(str, buf);
		    UB_USB_CDC_SendString(buf, CR);//send back received data
		                                   //with moving to new line
		  }
	  }
	  if(Get_Button(user_button))
	      break;
	  PCF8812_Putline(str, 2);
	  PCF8812_DELAY;
  }
}

void USB_Count(uint32_t period)
{
  USB_CDC_RXSTATUS_t check = RX_EMPTY;
  uint32_t i = 0;
  while(1)
  {
    PCF8812_Clear();
    PCF8812_Title("USB COUNT");
    PCF8812_UValue("count", i, "", 2);
    PCF8812_Button("END", "PAUSE", "PAUSE");
    if(!Check_delay_ms(period)) {
        USB_Send_Int(i);
        PCF8812_On();
        i++;
    }
    PCF8812_DELAY;
    //delay_ms(period);
    if(Get_Button(user_button))
            break;
    if(Get_Button(button_1))
      while(1)
      {
        PCF8812_Button("END", "RESUME", "RESUME");
        //PCF8812_Butt_ind(clear_ind);
       if(Get_Button(button_1))
               break;
       if(Get_Button(user_button))
               break;
      }
  }
}

void USB_Send_Int(uint32_t value)
{
  char str[APP_TX_BUF_SIZE] = "";
  sprintf(str, "%d", value);
  if(UB_USB_CDC_GetStatus() == USB_CDC_CONNECTED)//check USB plug to PC
    {
     UB_USB_CDC_SendString(str, CR);
    }
}
/*
void Logview_Send(uint8_t value)
{
  char buf[10];
  uint8_t n;

  //strcpy(buf,"$1;1;;");

    sprintf(buf,"$1;1;;%4d;0", value);

  //strcpy(buf,"0");

    if(UB_USB_CDC_GetStatus() == USB_CDC_CONNECTED)//Проверка работы USB-подключения к ПК
        {
         UB_USB_CDC_SendString(buf, CR);
        }
}
*/
