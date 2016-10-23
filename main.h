//--------------------------------------------------------------
// File     : main.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_MAIN_H
#define __STM32F4_MAIN_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <newlib.h>
#include <wchar.h>
#include <stdbool.h>
//#include <fenv.h>
//#include <time.h>

#include "../Device/stm32f4xx.h"
__IO int32_t ITM_RxBuffer;

#include "user.h"

#ifndef NULL
#define NULL  (void*)0
#endif

#define SWAP(type, a, b) {type temp; temp = a; a = b; b = temp;}


//typedef void (*p_func)(void);

void MCU_Init();


#endif // __STM32F4_MAIN_H
