/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : Ezhik (thx to MCD Application Team)
* Version            : V1.0.0
* Date               : 09/06/2011
* Description        : Заголовки для дескрипторов USB-девайса
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

// $USBCONFIG - тут надо подправить размеры всех дескрипторов, в случае их изменения
#define Timmy_SIZ_DEVICE_DESC         	     18
#define Timmy_SIZ_CONFIG_DESC          	     46

#define Timmy_SIZ_STRING_LANGID       	     4
#define Timmy_SIZ_STRING_VENDOR       	     36
#define Timmy_SIZ_STRING_PRODUCT       		 12

#if defined STRIVE_BOARD
	#define Timmy_SIZ_STRING_SERIAL            26
#elif defined EM_STM3210C_BOARD
	#define Timmy_SIZ_STRING_SERIAL            26
#elif defined TE_STM32F103_BOARD
	#define Timmy_SIZ_STRING_SERIAL            26
#else
	#define Timmy_SIZ_STRING_SERIAL            26
#endif

#define Timmy_SIZ_STRING_INTERFACE        	 20

/* Exported functions ------------------------------------------------------- */
extern const uint8_t Timmy_DeviceDescriptor[Timmy_SIZ_DEVICE_DESC];
extern const uint8_t Timmy_ConfigDescriptor[Timmy_SIZ_CONFIG_DESC];

extern const uint8_t Timmy_StringLangID[Timmy_SIZ_STRING_LANGID];
extern const uint8_t Timmy_StringVendor[Timmy_SIZ_STRING_VENDOR];
extern const uint8_t Timmy_StringProduct[Timmy_SIZ_STRING_PRODUCT];
extern uint8_t Timmy_StringSerial[Timmy_SIZ_STRING_SERIAL];
extern const uint8_t Timmy_StringInterface[Timmy_SIZ_STRING_INTERFACE];

#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


