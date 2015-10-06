/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : platform_config.h
* Author             : Ezhik (thx to MCD Application Team)
* Version            : V1.0.0
* Date               : 09/07/2009
* Description        : Конфигурация платы
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

// $BOARDSPECIFIC - тут необходимо задать конфигурацию платы
// сделано 3 конфига под различные платы
// Последний конфиг - дефолтный, его можно изменять, или на его примере сделать свой.
// Число светодиодов должно быть не больше 4 и они должны идти по порядку начиная с 1


// Варианты конфигурации USART

// USE_USART1_DEFAULT_PA9_PA10
// USE_USART1_REMAP_PB6_PB7

// USE_USART2_DEFAULT_PA2_PA3
// USE_USART2_REMAP_PD5_PD6

// USE_USART3_DEFAULT_PB10_PB11
// USE_USART3_REMAP1_PC10_PC11
// USE_USART3_REMAP2_PD8_PD9


// $USBCONFIG - тут также задается pin, отвечающий за программное подключение/отключение USB-устройства


// STM32 Development Board MINI (512K Flash 64K SRAM) 2.4-inch QVGA TFT module
// http://www.ebay.com/itm/Mini-STM32-Board-STM32F103VET6-2-4-LCD-Touch-Panel-JTAG-/200575289157
// http://www.mcueasy.com/product_detail.php?product_id=55
#if defined STRIVE_BOARD

	#define USB_DISCONNECT                      GPIOC  
	#define USB_DISCONNECT_PIN                  GPIO_Pin_13
	#define USB_DISCONNECT_LOG1					DISABLE
	#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOC

	#define LEDS_NUM							1
									
	#define LED1								0
	#define LED1_GPIO_PORT       	            GPIOB
	#define LED1_GPIO_CLK              		    RCC_APB2Periph_GPIOB  
	#define LED1_GPIO_PIN         		        GPIO_Pin_5

	#define USE_USART1_DEFAULT_PA9_PA10

// EMBEST EM-STM32C
// http://www.terraelectronica.ru/catalog_info.php?ID=1001&CODE=557292
#elif defined EM_STM3210C_BOARD
	#define LED1								0
	#define LED1_GPIO_PORT              	    GPIOE
	#define LED1_GPIO_CLK               	    RCC_APB2Periph_GPIOE  
	#define LED1_GPIO_PIN               	    GPIO_Pin_2

	#define LED2								1
	#define LED2_GPIO_PORT       	            GPIOE
	#define LED2_GPIO_CLK              		    RCC_APB2Periph_GPIOE  
	#define LED2_GPIO_PIN         		        GPIO_Pin_3

	#define LED3								2
	#define LED3_GPIO_PORT       	            GPIOE
	#define LED3_GPIO_CLK              		    RCC_APB2Periph_GPIOE  
	#define LED3_GPIO_PIN         		        GPIO_Pin_4

	#define LED4								3
	#define LED4_GPIO_PORT       	            GPIOE
	#define LED4_GPIO_CLK              		    RCC_APB2Periph_GPIOE  
	#define LED4_GPIO_PIN         		        GPIO_Pin_5

	#define USE_USART2_REMAP_PD5_PD6

// TE-STM32F103 - «Махаон»
// http://www.terraelectronica.ru/catalog_info.php?ID=1001&CODE=333824
#elif defined TE_STM32F103_BOARD

	#define USB_DISCONNECT                      GPIOB  
	#define USB_DISCONNECT_PIN                  GPIO_Pin_5
	#define USB_DISCONNECT_LOG1					DISABLE
	#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOB

	#define LED1								0
	#define LED1_GPIO_PORT              	    GPIOA
	#define LED1_GPIO_CLK               	    RCC_APB2Periph_GPIOA  
	#define LED1_GPIO_PIN               	    GPIO_Pin_4

	#define LED2								1
	#define LED2_GPIO_PORT       	            GPIOA
	#define LED2_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define LED2_GPIO_PIN         		        GPIO_Pin_5

	#define LED3								2
	#define LED3_GPIO_PORT       	            GPIOA
	#define LED3_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define LED3_GPIO_PIN         		        GPIO_Pin_6
	
	#define SERVOS_NUM						6

	#define SERVO1								0
	#define SERVO1_GPIO_PORT       	            GPIOA
	#define SERVO1_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define SERVO1_GPIO_PIN         		        GPIO_Pin_0
	
	#define SERVO2								1
	#define SERVO2_GPIO_PORT       	            GPIOA
	#define SERVO2_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define SERVO2_GPIO_PIN         		        GPIO_Pin_1
	
	#define SERVO3								2
	#define SERVO3_GPIO_PORT       	            GPIOA
	#define SERVO3_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define SERVO3_GPIO_PIN         		        GPIO_Pin_2
	
	#define SERVO4								3
	#define SERVO4_GPIO_PORT       	            GPIOA
	#define SERVO4_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define SERVO4_GPIO_PIN         		        GPIO_Pin_3

	#define SERVO5								4
	#define SERVO5_GPIO_PORT       	            GPIOA
	#define SERVO5_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define SERVO5_GPIO_PIN         		        GPIO_Pin_6
	
	#define SERVO6								5
	#define SERVO6_GPIO_PORT       	            GPIOA
	#define SERVO6_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define SERVO6_GPIO_PIN         		        GPIO_Pin_7

	#define USE_USART1_DEFAULT_PA9_PA10

#else// Дефолтная конфигурация - сделано под TE-STM32F103_BOARD

	#define USB_DISCONNECT                      GPIOB  
	#define USB_DISCONNECT_PIN                  GPIO_Pin_5
	#define USB_DISCONNECT_LOG1					DISABLE
	#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOB

	#define LED1								0
	#define LED1_GPIO_PORT              	    GPIOA
	#define LED1_GPIO_CLK               	    RCC_APB2Periph_GPIOA  
	#define LED1_GPIO_PIN               	    GPIO_Pin_4

	#define LED2								1
	#define LED2_GPIO_PORT       	            GPIOA
	#define LED2_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define LED2_GPIO_PIN         		        GPIO_Pin_5

	#define LED3								2
	#define LED3_GPIO_PORT       	            GPIOA
	#define LED3_GPIO_CLK              		    RCC_APB2Periph_GPIOA  
	#define LED3_GPIO_PIN         		        GPIO_Pin_6

	#define USE_USART1_DEFAULT_PA9_PA10

#endif

// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------

// Дальше ничего редактировать не нужно

// Собранные в одном месте все возможные конфигурации для USART
// USART 1

#ifdef USE_USART1_DEFAULT_PA9_PA10
	#define USART1_USE_INTERRUPT_HANDLER
	#define COM1       	    	      		    USART1
	#define COM1_IRQ  	    	      		    USART1_IRQn
	#define COM1_GPIO   	          		    GPIOA
	#define COM1_CLK_APB              		    2
	#define COM1_CLK               	 		    RCC_APB2Periph_USART1
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOA
	#define COM1_GPIO_REMAP						0
	#define COM1_TxPin                  	    GPIO_Pin_9
	#define COM1_RxPin                  	    GPIO_Pin_10  
#endif

#ifdef USE_USART1_REMAP_PB6_PB7
	#define USART1_USE_INTERRUPT_HANDLER
	#define COM1       	    	      		    USART1
	#define COM1_IRQ  	    	      		    USART1_IRQn
	#define COM1_GPIO   	          		    GPIOB
	#define COM1_CLK_APB              		    2
	#define COM1_CLK               	 		    RCC_APB2Periph_USART1
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOB
	#define COM1_GPIO_REMAP						GPIO_Remap_USART1
	#define COM1_TxPin                  	    GPIO_Pin_6
	#define COM1_RxPin                  	    GPIO_Pin_7  
#endif

// USART 2

#ifdef USE_USART2_DEFAULT_PA2_PA3
	#define USART2_USE_INTERRUPT_HANDLER
	#define COM1       	    	        	    USART2
	#define COM1_IRQ  	    	       		    USART2_IRQn
	#define COM1_GPIO   	            	  	GPIOA
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART2
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOA
	#define COM1_GPIO_REMAP						0
	#define COM1_TxPin		                    GPIO_Pin_2
	#define COM1_RxPin      		            GPIO_Pin_3
#endif

#ifdef USE_USART2_REMAP_PD5_PD6
	#define USART2_USE_INTERRUPT_HANDLER
	#define COM1       	    	        	    USART2
	#define COM1_IRQ  	    	       		    USART2_IRQn
	#define COM1_GPIO   	            	  	GPIOD
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART2
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOD
	#define COM1_GPIO_REMAP						GPIO_Remap_USART2
	#define COM1_TxPin		                    GPIO_Pin_5
	#define COM1_RxPin      		            GPIO_Pin_6
#endif

 // USART 3
 
#ifdef USE_USART3_DEFAULT_PB10_PB11
	#define USART3_USE_INTERRUPT_HANDLER
	#define COM1       	    	        	    USART3
	#define COM1_IRQ  	    	       		    USART3_IRQn
	#define COM1_GPIO   	            	  	GPIOB
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART3
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOB
	#define COM1_GPIO_REMAP						0
	#define COM1_TxPin		                    GPIO_Pin_10
	#define COM1_RxPin      		            GPIO_Pin_11
#endif

#ifdef USE_USART3_REMAP1_PC10_PC11
	#define USART3_USE_INTERRUPT_HANDLER
	#define COM1       	    	        	    USART3
	#define COM1_IRQ  	    	       		    USART3_IRQn
	#define COM1_GPIO   	            	  	GPIOC
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART3
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOC
	#define COM1_GPIO_REMAP						GPIO_PartialRemap_USART3
	#define COM1_TxPin		                    GPIO_Pin_10
	#define COM1_RxPin      		            GPIO_Pin_11
#endif

#ifdef USE_USART3_REMAP2_PD8_PD9
	#define USART3_USE_INTERRUPT_HANDLER										
	#define COM1       	    	        	    USART3
	#define COM1_IRQ  	    	       		    USART3_IRQn
	#define COM1_GPIO   	            	  	GPIOC
	#define COM1_CLK_APB               		    1
	#define COM1_CLK               	   		    RCC_APB1Periph_USART3
	#define COM1_GPIO_CLK               	    RCC_APB2Periph_GPIOC
	#define COM1_GPIO_REMAP						GPIO_FullRemap_USART3
	#define COM1_TxPin		                    GPIO_Pin_8
	#define COM1_RxPin      		            GPIO_Pin_9
#endif

// Подсчет количества светодиодов

#if (defined LED1) && (defined LED2) && (defined LED3) && (defined LED4)
	#define LEDS_NUM							4 
#elif (defined LED1) && (defined LED2) && (defined LED3)
	#define LEDS_NUM							3
#elif (defined LED1) && (defined LED2)
	#define LEDS_NUM							2
#elif (defined LED1)
	#define LEDS_NUM							1
#else
	#define LEDS_NUM							0
#endif

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
