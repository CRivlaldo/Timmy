/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_istr.h"

#include "user_usb.h"

#include <stdio.h>

// $USBCONFIG - Тут нужно при необходимости добавить/удалить Callback-и Ендпойинтов
// См. также usb_conf.h

// Ендпойнт 1. 
void EP1_IN_Callback(void)
{
#ifdef _DEBUG_
    printf("EP: 01 in \r\n");
#endif
}

// Ендпойнт 2. 
void EP2_OUT_Callback(void)
{
    #ifndef STM32F10X_CL
		// Разрешаем ендпойнту получить следующую команду
        SetEPRxStatus(ENDP2, EP_RX_VALID);
    #else
        if (GetEPRxStatus(EP2_OUT) == EP_RX_STALL)
        {
			// Разрешаем ендпойнту получить следующую команду
            SetEPRxStatus(EP2_OUT, EP_RX_VALID);
        }
    #endif /* STM32F10X_CL */

#ifdef _DEBUG_
	printf("EP: 02 out \r\n");
#endif

	// Чтение и обработка пришедших данных
	usb_handle_command();
}

// Ендпойнт 3. 
void EP3_IN_Callback(void)
{
#ifdef _DEBUG_
  printf("EP: 03 in \r\n");
#endif
}

// Ендпойнт 4. Ничего не делает.
void EP4_OUT_Callback(void)
{
    #ifndef STM32F10X_CL
		// Разрешаем ендпойнту получить следующую команду
        SetEPRxStatus(ENDP4, EP_RX_VALID);
    #else
        if (GetEPRxStatus(EP4_OUT) == EP_RX_STALL)
        {
			// Разрешаем ендпойнту получить следующую команду
       		SetEPRxStatus(EP4_OUT, EP_RX_VALID);
        }
    #endif /* STM32F10X_CL */

#ifdef _DEBUG_
  printf("EP: 04 out \r\n");
#endif
}


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

