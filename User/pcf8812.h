/*
 * pcf8812.h
 *
 *  Created on: 26.07.2015
 *      Author: Aleksey
 */

#ifndef PCF8812_H_
#define PCF8812_H_

#include "../Device/stm32f4xx.h"
#include "../main.h"


#define PCF8812_ON()                GPIOD->BSRR = GPIO_BSRR_BS_8
#define PCF8812_OFF()               GPIOD->BSRR = GPIO_BSRR_BR_8
#define PCF8812_CMD()               GPIOD->BSRR = GPIO_BSRR_BR_9
#define PCF8812_DATA()              GPIOD->BSRR = GPIO_BSRR_BS_9
#define PCF8812_SEL()               GPIOD->BSRR = GPIO_BSRR_BR_10
#define PCF8812_UNSEL()             GPIOD->BSRR = GPIO_BSRR_BS_10
#define PCF8812_RES_ON()            GPIOD->BSRR = GPIO_BSRR_BR_11
#define PCF8812_RES_OFF()           GPIOD->BSRR = GPIO_BSRR_BS_11
#define PCF8812_LIGHT_ON()          GPIOB->BSRR = GPIO_BSRR_BS_14
#define PCF8812_LIGHT_OFF()         GPIOB->BSRR = GPIO_BSRR_BR_14
#define PCF8812_XSIZE               102//101 horizontal pixels + 1 invisible pixel
#define PCF8812_YSIZE               8//vertical banks(each contains 8 pixels) also line in text mode
#define PCF8812_BUFSIZ              PCF8812_XSIZE * PCF8812_YSIZE//LCD Data buffer size 816 bytes
#define PCF8812_FLAG                pcf8812_flag = SET//flag for display refresh function
#define PCF8812_DECODE_XY(x, y)     ((y / PCF8812_YSIZE) * PCF8812_XSIZE + x)//decode coordinates to buffer index
#define PCF8812_DECODE_LINE(line)   PCF8812_XSIZE * line //decode char strings to buffer index
#define PCF8812_CHAR_WIDE           6//font 5x8 + interval
#define PCF8812_DECODE_C(line, col) (PCF8812_DECODE_LINE(line) + PCF8812_CHAR_WIDE * col)//decode char line & columns to buffer index
#define PCF8812_LCD_LINE            17//columns in line
#define PCF8812_STR_SIZ             18//17chars + '\0'
#define EMPTY_STR                   "                 "
#define DIGITS_MAX                  5
#define PCF8812_F_RATE              25//LCD display framerate (frame/sec)
#define PCF8812_DELAY               delay_ms(1000/PCF8812_F_RATE)



/* Commands **************************/
#define FUNC_SET        0x20//function set 0 0 1 0 0 PD V H
#define FUNC_SET_PD     0x04//set for power-down mode (reset value 1)
#define FUNC_SET_V      0x02//set for vertical addressing
#define FUNC_SET_H      0x01//set for use extended instruction set
/* Base instructions ****************/
#define DISP_BLANK      0x08//display blank
#define DISP_ALL_ON     0x09//all display segments on
#define DISP_NORMAL     0x0C//normal mode
#define DISP_INVERSE    0x0D//inverse video mode
#define VOP_RANGE_LOW   0x10//set for V LCD programming range LOW(reset)
#define VOP_RANGE_HIGH  0x11//set for V LCD programming range HIGH
#define SET_Y           0x40// 0100YYYY (sets Y address of RAM 0 <= Y <= 8)
#define SET_X           0x80// 1XXXXXXX (sets X address part of RAM 0 <= X <= 101)
/* Extended instructions *************/
#define TEMP_CONT_0     0x04//V LCD temperature coefficient 0 (reset)
#define TEMP_CONT_1     0x05//V LCD temperature coefficient 1
#define TEMP_CONT_2     0x06//V LCD temperature coefficient 2
#define TEMP_CONT_3     0x07//V LCD temperature coefficient 3
#define VOLT_MUL_2      0x08//2x voltage multiplier (reset)
#define VOLT_MUL_3      0x09//3x voltage multiplier
#define VOLT_MUL_4      0x0A//4x voltage multiplier
#define VOLT_MUL_5      0x0B//5x voltage multiplier
#define BIAS_SET        0x10//bias system 0 0 0 1 0 BS2 BS1 BS0(reset 0 0 0)
#define VOP_SET         0x80//Vlcd programming 1 V6 V5 V4 V3 V2 V1 V0


#define PCF8812_VIEW_VAR(var, line) {uint8_t s[PCF8812_STR_SIZ];\
                                snprintf(s, PCF8812_STR_SIZ, #var ":%d", var);\
                                LCD_Putline(s, line);}

uint8_t PCF8812_buff[PCF8812_BUFSIZ];
uint8_t PCF8812_buff_state;

enum PCF8812_buff_state
{
  PCF8812_BUFF_READY,
  PCF8812_BUFF_BUSY,
  PCF8812_BUFF_CHANGED
};

#define PCF8812_BUFF_SET_READY() PCF8812_buff_state = PCF8812_BUFF_READY

enum fill
{empty, filled};

enum view_mode
{view_all, view_time, view_date};


struct Par {
  uint8_t* name;
  uint8_t code;
};

typedef struct Par_list {
  uint8_t* name;
  uint8_t num;
  struct Par item[];
}Par_list;

static const char lcd_font[][5] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },  // sp
	{ 0x00, 0x00, 0x2f, 0x00, 0x00 },  // !
	{ 0x00, 0x07, 0x00, 0x07, 0x00 },  // "
	{ 0x14, 0x7f, 0x14, 0x7f, 0x14 },  // #
	{ 0x24, 0x2a, 0x7f, 0x2a, 0x12 },  // $
	{ 0x23, 0x13, 0x08, 0x64, 0x62 },  // %

	{ 0x36, 0x49, 0x55, 0x22, 0x50 },  // &
	{ 0x00, 0x05, 0x03, 0x00, 0x00 },  // '
	{ 0x00, 0x1c, 0x22, 0x41, 0x00 },  // (
	{ 0x00, 0x41, 0x22, 0x1c, 0x00 },  // )
	{ 0x14, 0x08, 0x3E, 0x08, 0x14 },  // *
	{ 0x08, 0x08, 0x3E, 0x08, 0x08 },  // +
	{ 0x00, 0x00, 0x50, 0x30, 0x00 },  // ,
	{ 0x08, 0x08, 0x08, 0x08, 0x08 },  // -
	{ 0x00, 0x60, 0x60, 0x00, 0x00 },  // .
	{ 0x20, 0x10, 0x08, 0x04, 0x02 },  // /
	{ 0x3E, 0x51, 0x49, 0x45, 0x3E },  // 0
	{ 0x00, 0x42, 0x7F, 0x40, 0x00 },  // 1
	{ 0x42, 0x61, 0x51, 0x49, 0x46 },  // 2
	{ 0x21, 0x41, 0x45, 0x4B, 0x31 },  // 3
	{ 0x18, 0x14, 0x12, 0x7F, 0x10 },  // 4
	{ 0x27, 0x45, 0x45, 0x45, 0x39 },  // 5
	{ 0x3C, 0x4A, 0x49, 0x49, 0x30 },  // 6
	{ 0x01, 0x71, 0x09, 0x05, 0x03 },  // 7
	{ 0x36, 0x49, 0x49, 0x49, 0x36 },  // 8
	{ 0x06, 0x49, 0x49, 0x29, 0x1E },  // 9
	{ 0x00, 0x36, 0x36, 0x00, 0x00 },  // :
	{ 0x00, 0x56, 0x36, 0x00, 0x00 },  // ;
	{ 0x08, 0x14, 0x22, 0x41, 0x00 },  // <
	{ 0x14, 0x14, 0x14, 0x14, 0x14 },  // =
	{ 0x00, 0x41, 0x22, 0x14, 0x08 },  // >
	{ 0x02, 0x01, 0x51, 0x09, 0x06 },  // ?
	{ 0x32, 0x49, 0x59, 0x51, 0x3E },  // @
	{ 0x7E, 0x11, 0x11, 0x11, 0x7E },  // A
	{ 0x7F, 0x49, 0x49, 0x49, 0x36 },  // B
	{ 0x3E, 0x41, 0x41, 0x41, 0x22 },  // C
	{ 0x7F, 0x41, 0x41, 0x22, 0x1C },  // D
	{ 0x7F, 0x49, 0x49, 0x49, 0x41 },  // E
	{ 0x7F, 0x09, 0x09, 0x09, 0x01 },  // F
	{ 0x3E, 0x41, 0x49, 0x49, 0x7A },  // G
	{ 0x7F, 0x08, 0x08, 0x08, 0x7F },  // H
	{ 0x00, 0x41, 0x7F, 0x41, 0x00 },  // I
	{ 0x20, 0x40, 0x41, 0x3F, 0x01 },  // J
	{ 0x7F, 0x08, 0x14, 0x22, 0x41 },  // K
	{ 0x7F, 0x40, 0x40, 0x40, 0x40 },  // L
	{ 0x7F, 0x02, 0x0C, 0x02, 0x7F },  // M
	{ 0x7F, 0x04, 0x08, 0x10, 0x7F },  // N
	{ 0x3E, 0x41, 0x41, 0x41, 0x3E },  // O
	{ 0x7F, 0x09, 0x09, 0x09, 0x06 },  // P
	{ 0x3E, 0x41, 0x51, 0x21, 0x5E },  // Q
	{ 0x7F, 0x09, 0x19, 0x29, 0x46 },  // R
	{ 0x46, 0x49, 0x49, 0x49, 0x31 },  // S
	{ 0x01, 0x01, 0x7F, 0x01, 0x01 },  // T
	{ 0x3F, 0x40, 0x40, 0x40, 0x3F },  // U
	{ 0x1F, 0x20, 0x40, 0x20, 0x1F },  // V
	{ 0x3F, 0x40, 0x38, 0x40, 0x3F },  // W
	{ 0x63, 0x14, 0x08, 0x14, 0x63 },  // X
	{ 0x07, 0x08, 0x70, 0x08, 0x07 },  // Y
	{ 0x61, 0x51, 0x49, 0x45, 0x43 },  // Z
	{ 0x00, 0x7F, 0x41, 0x41, 0x00 },  // [
	{ 0x55, 0x2A, 0x55, 0x2A, 0x55 },  // 55
	{ 0x00, 0x41, 0x41, 0x7F, 0x00 },  // ]
	{ 0x04, 0x02, 0x01, 0x02, 0x04 },  // ^
	{ 0x40, 0x40, 0x40, 0x40, 0x40 },  // _
	{ 0x00, 0x01, 0x02, 0x04, 0x00 },  // '
	{ 0x20, 0x54, 0x54, 0x54, 0x78 },  // a
	{ 0x7F, 0x48, 0x44, 0x44, 0x38 },  // b
	{ 0x38, 0x44, 0x44, 0x44, 0x20 },  // c
	{ 0x38, 0x44, 0x44, 0x48, 0x7F },  // d
	{ 0x38, 0x54, 0x54, 0x54, 0x18 },  // e
	{ 0x08, 0x7E, 0x09, 0x01, 0x02 },  // f
	{ 0x0C, 0x52, 0x52, 0x52, 0x3E },  // g
	{ 0x7F, 0x08, 0x04, 0x04, 0x78 },  // h
	{ 0x00, 0x44, 0x7D, 0x40, 0x00 },  // i
	{ 0x20, 0x40, 0x44, 0x3D, 0x00 },  // j
	{ 0x7F, 0x10, 0x28, 0x44, 0x00 },  // k
	{ 0x00, 0x41, 0x7F, 0x40, 0x00 },  // l
	{ 0x7C, 0x04, 0x18, 0x04, 0x78 },  // m
	{ 0x7C, 0x08, 0x04, 0x04, 0x78 },  // n
	{ 0x38, 0x44, 0x44, 0x44, 0x38 },  // o
	{ 0x7C, 0x14, 0x14, 0x14, 0x08 },  // p
	{ 0x08, 0x14, 0x14, 0x18, 0x7C },  // q
	{ 0x7C, 0x08, 0x04, 0x04, 0x08 },  // r
	{ 0x48, 0x54, 0x54, 0x54, 0x20 },  // s
	{ 0x04, 0x3F, 0x44, 0x40, 0x20 },  // t
	{ 0x3C, 0x40, 0x40, 0x20, 0x7C },  // u
	{ 0x1C, 0x20, 0x40, 0x20, 0x1C },  // v
	{ 0x3C, 0x40, 0x30, 0x40, 0x3C },  // w
	{ 0x44, 0x28, 0x10, 0x28, 0x44 },  // x
	{ 0x0C, 0x50, 0x50, 0x50, 0x3C },  // y
	{ 0x44, 0x64, 0x54, 0x4C, 0x44 },  // z

	{ 0x08, 0x08, 0x36, 0x41, 0x41 },  // {
	{ 0x00, 0x00, 0x7F, 0x00, 0x00 },  // |
	{ 0x41, 0x41, 0x36, 0x08, 0x08 },  // }
	{ 0x02, 0x01, 0x02, 0x02, 0x01 },  // ~

	{ 0x7E, 0x02, 0x02, 0x02, 0x03 },  // 165 Р СћРЎвЂ™
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x7C, 0x55, 0x54, 0x55, 0x44 },  // 168 Р В Р С“
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x3E, 0x49, 0x49, 0x49, 0x41 },  // 170 Р В РІР‚С›
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x45, 0x7C, 0x45, 0x00 },  // 175 Р В РІР‚РЋ
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x41, 0x7F, 0x41, 0x00 },  // 178 Р В РІР‚В 
  { 0x00, 0x44, 0x7D, 0x40, 0x00 },  // 179 Р РЋРІР‚вЂњ
  { 0x7C, 0x04, 0x04, 0x04, 0x06 },  // 180 Р СћРІР‚пїЅ
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x38, 0x55, 0x54, 0x55, 0x18 },  // 184 Р РЋРІР‚пїЅ
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x38, 0x54, 0x54, 0x54, 0x44 },  // 186 Р РЋРІР‚Сњ
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x00, 0x00, 0x00, 0x00 },
  { 0x00, 0x45, 0x7C, 0x41, 0x00 },  // 191 Р РЋРІР‚вЂќ


	{ 0x7E, 0x11, 0x11, 0x11, 0x7E },  // Р В РЎвЂ™ (192)
	{ 0x7F, 0x49, 0x49, 0x49, 0x33 },  // Р В РІР‚пїЅ
	{ 0x7F, 0x49, 0x49, 0x49, 0x36 },  // Р В РІР‚в„ў
	{ 0x7F, 0x01, 0x01, 0x01, 0x03 },  // Р В РІР‚Сљ
	{ 0xE0, 0x51, 0x4F, 0x41, 0xFF },  // Р В РІР‚Сњ
	{ 0x7F, 0x49, 0x49, 0x49, 0x49 },  // Р В РІР‚Сћ
	{ 0x77, 0x08, 0x7F, 0x08, 0x77 },  // Р В РІР‚вЂњ
	{ 0x49, 0x49, 0x49, 0x49, 0x36 },  // Р В РІР‚вЂќ
	{ 0x7F, 0x10, 0x08, 0x04, 0x7F },  // Р В РїС—Р…
	{ 0x7C, 0x21, 0x12, 0x09, 0x7C },  // Р В РІвЂћСћ
	{ 0x7F, 0x08, 0x14, 0x22, 0x41 },  // Р В РЎв„ў
	{ 0x20, 0x41, 0x3F, 0x01, 0x7F },  // Р В РІР‚С”
	{ 0x7F, 0x02, 0x0C, 0x02, 0x7F },  // Р В РЎС™
	{ 0x7F, 0x08, 0x08, 0x08, 0x7F },  // Р В РЎСљ
	{ 0x3E, 0x41, 0x41, 0x41, 0x3E },  // Р В РЎвЂє
	{ 0x7F, 0x01, 0x01, 0x01, 0x7F },  // Р В РЎСџ
	{ 0x7F, 0x09, 0x09, 0x09, 0x06 },  // Р В Р’В Р вЂ™Р’В 
	{ 0x3E, 0x41, 0x41, 0x41, 0x22 },  // Р В Р Р‹
	{ 0x01, 0x01, 0x7F, 0x01, 0x01 },  // Р В РЎС›
	{ 0x27, 0x48, 0x48, 0x48, 0x3F },  // Р В Р в‚¬
	{ 0x1C, 0x22, 0x7F, 0x22, 0x1C },  // Р В Р’В¤
	{ 0x63, 0x14, 0x08, 0x14, 0x63 },  // Р В РўС’
	{ 0x7F, 0x40, 0x40, 0x40, 0xFF },  // Р В Р’В¦
	{ 0x07, 0x08, 0x08, 0x08, 0x7F },  // Р В Р’В§
	{ 0x7F, 0x40, 0x7F, 0x40, 0x7F },  // Р В Р Рѓ
	{ 0x7F, 0x40, 0x7F, 0x40, 0xFF },  // Р В Р’В©
	{ 0x01, 0x7F, 0x48, 0x48, 0x30 },  // Р В Р вЂћ
	{ 0x7F, 0x48, 0x30, 0x00, 0x7F },  // Р В Р’В«
	{ 0x7F, 0x48, 0x48, 0x30, 0x00 },  // Р В Р’В¬
	{ 0x22, 0x41, 0x49, 0x49, 0x3E },  // Р В Р’В­
	{ 0x7F, 0x08, 0x3E, 0x41, 0x3E },  // Р В Р’В®
	{ 0x46, 0x29, 0x19, 0x09, 0x7F },  // Р В Р вЂЎ
	{ 0x20, 0x54, 0x54, 0x54, 0x78 },  // Р В Р’В°
	{ 0x3C, 0x4A, 0x4A, 0x49, 0x31 },  // Р В Р’В±
	{ 0x7C, 0x54, 0x54, 0x28, 0x00 },  // Р В Р вЂ 
	{ 0x7C, 0x04, 0x04, 0x04, 0x0C },  // Р В РЎвЂ“
	{ 0xE0, 0x54, 0x4C, 0x44, 0xFC },  // Р В РўвЂ�
	{ 0x38, 0x54, 0x54, 0x54, 0x08 },  // Р В Р’Вµ
	{ 0x6C, 0x10, 0x7C, 0x10, 0x6C },  // Р В Р’В¶
	{ 0x44, 0x44, 0x54, 0x54, 0x28 },  // Р В Р’В·
	{ 0x7C, 0x20, 0x10, 0x08, 0x7C },  // Р В РЎвЂ�
	{ 0x78, 0x42, 0x24, 0x12, 0x78 },  // Р В РІвЂћвЂ“
	{ 0x7C, 0x10, 0x28, 0x44, 0x00 },  // Р В РЎвЂќ
	{ 0x20, 0x44, 0x3C, 0x04, 0x7C },  // Р В Р’В»
	{ 0x7C, 0x08, 0x10, 0x08, 0x7C },  // Р В РЎпїЅ
	{ 0x7C, 0x10, 0x10, 0x10, 0x7C },  // Р В Р вЂ¦
	{ 0x38, 0x44, 0x44, 0x44, 0x38 },  // Р В РЎвЂў
	{ 0x7C, 0x04, 0x04, 0x04, 0x7C },  // Р В РЎвЂ”
	{ 0x7C, 0x14, 0x14, 0x14, 0x08 },  // Р РЋР вЂљ
	{ 0x38, 0x44, 0x44, 0x44, 0x44 },  // Р РЋР С“
	{ 0x04, 0x04, 0x7C, 0x04, 0x04 },  // Р РЋРІР‚С™
	{ 0x0C, 0x50, 0x50, 0x50, 0x3C },  // Р РЋРЎвЂњ
	{ 0x18, 0x24, 0x7E, 0x24, 0x18 },  // Р РЋРІР‚С›
	{ 0x44, 0x28, 0x10, 0x28, 0x44 },  // Р РЋРІР‚В¦
	{ 0x7C, 0x40, 0x40, 0x40, 0xFC },  // Р РЋРІР‚В 
	{ 0x0C, 0x10, 0x10, 0x10, 0x7C },  // Р РЋРІР‚РЋ
	{ 0x7C, 0x40, 0x7C, 0x40, 0x7C },  // Р РЋРІвЂљВ¬
	{ 0x7C, 0x40, 0x7C, 0x40, 0xFC },  // Р РЋРІР‚В°
	{ 0x04, 0x7C, 0x50, 0x50, 0x20 },  // Р РЋР вЂ°
	{ 0x7C, 0x50, 0x20, 0x00, 0x7C },  // Р РЋРІР‚в„–
	{ 0x7C, 0x50, 0x50, 0x20, 0x00 },  // Р РЋР Р‰
	{ 0x28, 0x44, 0x54, 0x54, 0x38 },  // Р РЋР РЉ
	{ 0x7C, 0x10, 0x38, 0x44, 0x38 },  // Р РЋР вЂ№
	{ 0x08, 0x54, 0x34, 0x14, 0x7C },  // Р РЋР РЏ (255)
};

typedef const uint8_t symb_ar[5];

static symb_ar arrow_up        = { 0x04, 0x02, 0x7F, 0x02, 0x04 };
static symb_ar arrow_down      = { 0x10, 0x20, 0x7F, 0x20, 0x10 };
static symb_ar arrow_right     = { 0x08, 0x49, 0x2A, 0x1C, 0x08 };
static symb_ar arrow_left      = { 0x08, 0x1C, 0x2A, 0x49, 0x08 };
static symb_ar corner_up       = { 0x40, 0x20, 0x10, 0x20, 0x40 };
static symb_ar corner_down     = { 0x01, 0x02, 0x04, 0x02, 0x01 };
static symb_ar line_up         = { 0x01, 0x01, 0x01, 0x01, 0x01 };
static symb_ar line_down       = { 0x40, 0x40, 0x40, 0x40, 0x40 };

void PCF8812_Port_Init(void);
void PCF8812_Command(uint8_t data);
void PCF8812_Data(uint8_t data);
void PCF8812_Init(void);
void PCF8812_Reset(void);
void PCF8812_Clear(void);
void PCF8812_Test(void);
void PCF8812_Toggle(void);
void PCF8812_Home(void);
void PCF8812_XY(uint8_t x, uint8_t y);
void PCF8812_Clr_Line(uint8_t line);
void PCF8812_Inv_Line(uint8_t line);
uint8_t PCF8812_Decode(uint8_t c);
void PCF8812_Set_Char(uint8_t c, uint8_t line, uint8_t col);
void PCF8812_Set_Symb(symb_ar symb, uint8_t line, uint8_t col);
void PCF8812_Inv_Char(uint8_t line, uint8_t col);
void PCF8812_Error(uint8_t *s);
void PCF8812_Message(uint8_t *s);
void PCF8812_Menu(uint8_t *s);
void PCF8812_Putline(uint8_t *s, uint8_t line);
void PCF8812_Putline_Centre(uint8_t* s, uint8_t line);
void PCF8812_Putline_Right(uint8_t* s, uint8_t line);
void PCF8812_SValue(uint8_t *name, int32_t value, uint8_t *unit, uint8_t line);
void PCF8812_UValue(uint8_t *name, uint32_t value, uint8_t *unit, uint8_t line);
void PCF8812_Hex_Value(uint8_t *name, int32_t value, uint8_t line);
void PCF8812_Float_Value(uint8_t *name, float value, uint8_t *unit, uint8_t line);
void PCF8812_Percent(uint8_t *name, int8_t value, uint8_t line);
void PCF8812_Set_bit(uint8_t x, uint8_t y);
void PCF8812_Clr_bit(uint8_t x, uint8_t y);
uint8_t PCF8812_Chk_bit(uint8_t x, uint8_t y);
inline void PCF8812_Set_byte(uint8_t line, uint8_t x, uint8_t value);
inline uint8_t PCF8812_Chk_byte(uint8_t line, uint8_t x);
inline void PCF8812_Inv_byte(uint8_t line, uint8_t x);
void PCF8812_Draw(void);
void PCF8812_Point(uint8_t x, uint8_t y);
void PCF8812_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void PCF8812_Rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill);
void PCF8812_Triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3);
void PCF8812_Time(uint8_t view_mode, uint8_t line);
void PCF8812_Option(uint8_t *option, uint8_t line);
void PCF8812_Cursor(uint8_t line);
void PCF8812_Button(uint8_t* butt_u, uint8_t* butt_1, uint8_t* butt_2);
void PCF8812_Butt_ind(uint8_t button);
uint32_t PCF8812_Input_Int(uint8_t* name, uint32_t min, uint32_t max);
void PCF8812_Input_Time();
uint32_t PCF8812_Set_Param(Par_list* list);


#endif /* PCF8812_H_ */
