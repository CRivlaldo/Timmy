/******************** (C) COPYRIGHT 2011 Ezhik. ************
* File Name          : main.c
* Author             : Ezhik
* Date First Issued  : 09/07/2011
* Description        : Main
*******************************************************************************
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "platform_config.h"
#include "usb_pwr.h"

#include "user_usb.h"
#include "led.h"

#include <stdio.h>

void printboard();

// Main
int main(void)
{
	// Клоки и питание
    Set_System();

	// USART
    InitCOM();

	printf("\r\n\r\n\r\n");
	printf("Starting system\r\n");

	// Настройка SysTick - 1 мс. Например, для мигания светодиода (см led.c)
	SysTick_Config(SystemFrequency / 1000);	

	// Конфигурация индикатора
    Periphery_Config();

	printf("LED Count: %d\r\n", LEDS_NUM);
	printboard();
	LED_Off(LED1);

	// Конфигурация USB
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

	printf("USB startup...");

    while (bDeviceState != CONFIGURED);

	printf(" done!\r\n");

    LED_On(LED1);
  

  while (1)
  {
  }
}

// $BOARDSPECIFIC - Вывод названия платы
void printboard()
{
	#if defined STRIVE_BOARD
	printf("Strive Board\r\n");

	#elif defined EM_STM3210C_BOARD
	printf("EM-STM3210C Board\r\n");

	#elif defined TE_STM32F103_BOARD
	printf("TE-STM32F103 Board\r\n");

	#else	
	printf("Default  Board\r\n");

	#endif
}


#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************* (C) COPYRIGHT 2009 Wuhan R&D Center, Embest *****END OF FILE****/
