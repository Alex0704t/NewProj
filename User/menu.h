/*
 * menu.h
 *
 *  Created on: 04.09.2015
 *      Author: Aleksey Totkal
 */

#ifndef USER_MENU_H_
#define USER_MENU_H_


//#include "../Device/stm32f4xx.h"
#include "main.h"
//#include "usb.h"

#define MAX_OPT         6
#define OPT_LEN         16
#define BUTT_NUM        3
#define USB_SEND_PERIOD 500

#define BREAK_OUT       if(brk_flag) \
                        {            \
                        brk_flag = 0;\
                        break;       \
                        }            \


typedef void (*act_fp)(void);

typedef struct {
  act_fp action;//button function
  uint8_t name[OPT_LEN];//button name
} butt_s;//button structure

typedef struct menu{
  uint8_t* name;//menu name
  uint8_t num;//options num
  uint8_t *option[MAX_OPT];//options names
  struct menu* next_menu[MAX_OPT];//next menu pointers
  act_fp action[MAX_OPT];//option functions pointers
  act_fp Init;//initialisation function pointer
  act_fp DeInit;//deinitialisation function pointer
  uint8_t init_flag;//initialisation flag
  butt_s* butt[BUTT_NUM];//buttons functions
}menu_s;//menu structure



void Enter_menu(menu_s *menu);
void Back_menu(void);
void No_op(void);
void Next_item(void);
void Prev_item(void);
void Main_menu(void);
/*********************************************************************/
void LED0_Toggle(void);
void LED1_Toggle(void);
void LED2_Toggle(void);
void LED3_Toggle(void);
void LED_all_Toggle(void);
void LED0_Bright(void);
void LED1_Bright(void);
void LED2_Bright(void);
void LED3_Bright(void);
void LED_all_Bright(void);
void LED0_Blink(void);
void LED1_Blink(void);
void LED2_Blink(void);
void LED3_Blink(void);
void LED_all_Blink(void);
/*********************************************************************/
void Send_Count(void);
/*********************************************************************/
void Sine_Init();
void Sine_Set();
void Beep_Start();
void Audio_Init();
/*********************************************************************/

#endif /* USER_MENU_H_ */
