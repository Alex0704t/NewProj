//--------------------------------------------------------------
// File     : usb_bsp.h
//--------------------------------------------------------------


#ifndef __USB_BSP__H__
#define __USB_BSP__H__

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "usb_core.h"

void BSP_Init(void);

void USB_OTG_BSP_Init ();
void USB_OTG_BSP_uDelay (const uint32_t usec);
void USB_OTG_BSP_mDelay (const uint32_t msec);
void USB_OTG_BSP_EnableInterrupt ();

#endif



