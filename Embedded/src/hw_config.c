/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : Ezhik (thx to MCD Application Team)
* Version            : V1.0.0
* Date               : 09/06/2011
* Description        : Конфигурация и установки
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


// Includes ------------------------------------------------------------------
#include "stm32f10x_it.h"
#include "hw_config.h"
#include "platform_config.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_lib.h"
#include "led.h"
#include "servo.h"

#include <stdio.h>


// Private variables ---------------------------------------------------------
ErrorStatus HSEStartUpStatus;

 
// Private function prototypes -----------------------------------------------
static void RCC_Config(void);
void USB_Disconnect_Config(void);

// Private functions ---------------------------------------------------------

// Конфигурируем клоки и питание
void Set_System(void)
{

  	// RCC configuration 
 	RCC_Config();

#ifdef USB_DISCONNECT
	// Конфигурация пина, отключающего USB
	USB_Disconnect_Config();

#endif

}


// Конфигурируем USART
void InitCOM(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(COM1_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	if(COM1_GPIO_REMAP != 0)
	{
		GPIO_PinRemapConfig(COM1_GPIO_REMAP, ENABLE);
	}

	switch(COM1_CLK_APB)
	{
		case 1:
			RCC_APB1PeriphClockCmd(COM1_CLK, ENABLE);
			break;
		case 2:
		default:
			RCC_APB2PeriphClockCmd(COM1_CLK, ENABLE);
			break;
	}
	

	// TX как Alternate Function Push-Pull
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = COM1_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM1_GPIO, &GPIO_InitStructure);

	// RX как Floating Input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = COM1_RxPin;
	GPIO_Init(COM1_GPIO, &GPIO_InitStructure);

	// 115200 бод, 8 бит, 1 стоп, N четность, без контроля, разрешены прием и передача
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM1, &USART_InitStructure);

	// Конфигурация прерывания
	NVIC_InitStructure.NVIC_IRQChannel = COM1_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Включаем прерывание на получение
	USART_ITConfig(COM1, USART_IT_RXNE, ENABLE);

	
	USART_Cmd(COM1, ENABLE);
}

// Обработчик прерывания USART, в данном случае - получения байта
// Далее - вывод символа обрано на терминал.
// Можно доработать до командной строки
#ifdef USART1_USE_INTERRUPT_HANDLER
void USART1_IRQHandler()
#elif defined USART2_USE_INTERRUPT_HANDLER
void USART2_IRQHandler()
#elif defined USART3_USE_INTERRUPT_HANDLER
void USART3_IRQHandler()
#endif
{
	char c = USART_ReceiveData(COM1);
	printf("%c",c);
}

// подменяем fputc для вывода stdout на USART1
int fputc(int ch, FILE *f)
{
	USART_SendData(COM1, (uint8_t) ch);

	while (USART_GetFlagStatus(COM1, USART_FLAG_TXE) == RESET)
	{}
	
	return ch;
}

// Задаем клоки USB (48 МГц)
void Set_USBClock(void)
{
#ifndef STM32F10X_CL
	// Источник
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	
	// Включаем клок USB
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

#else
	// Источник
  	RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  	// Включаем клок USB
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;

#endif
  
}



// Режим низкого энергопотребления. Лучше всего тут отключить все лишние клоки.
void Enter_LowPowerMode(void)
{
  // Переводим состояние девайса в Suspended
  bDeviceState = SUSPENDED;
}

// Выход из режима низкого энергопотребления. Включить клоки обратно.
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  // Задаем правильное состояние девайса
  if (pInfo->Current_Configuration != 0)
  {
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }

}

// Конфигурация USB-прерываний
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

#ifdef STM32F10X_CL 
  NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#else
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif /* STM32F10X_CL */

   
}

// Конфигурация индикаторов 
void Periphery_Config(void)
{
	// отсылаем к led.c, так как индикаторов может быть больше одного, а там все разрулено
	Led_Init();
	Servo_Init();
}

// Программный коннект-дисконнект USB-кабеля
void USB_Cable_Config (FunctionalState NewState)
{
#ifdef USB_DISCONNECT
  if (NewState != USB_DISCONNECT_LOG1)
  {
  	// Лог. 0 - коннект
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
  	// Лог. 1 - дисконнект
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
#endif
}


// Создание серийного номера устройства.
// Не обязательно. Только при использовании с VID, PID и Windows-драйвером от STM
void Get_SerialNum(void)
{
  /* 
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

  if (Device_Serial0 != 0)
  {
    Timmy_StringSerial[2] = (uint8_t)(Device_Serial0 & 0x000000FF);
    Timmy_StringSerial[4] = (uint8_t)((Device_Serial0 & 0x0000FF00) >> 8);
    Timmy_StringSerial[6] = (uint8_t)((Device_Serial0 & 0x00FF0000) >> 16);
    Timmy_StringSerial[8] = (uint8_t)((Device_Serial0 & 0xFF000000) >> 24);

    Timmy_StringSerial[10] = (uint8_t)(Device_Serial1 & 0x000000FF);
    Timmy_StringSerial[12] = (uint8_t)((Device_Serial1 & 0x0000FF00) >> 8);
    Timmy_StringSerial[14] = (uint8_t)((Device_Serial1 & 0x00FF0000) >> 16);
    Timmy_StringSerial[16] = (uint8_t)((Device_Serial1 & 0xFF000000) >> 24);

    Timmy_StringSerial[18] = (uint8_t)(Device_Serial2 & 0x000000FF);
    Timmy_StringSerial[20] = (uint8_t)((Device_Serial2 & 0x0000FF00) >> 8);
    Timmy_StringSerial[22] = (uint8_t)((Device_Serial2 & 0x00FF0000) >> 16);
    Timmy_StringSerial[24] = (uint8_t)((Device_Serial2 & 0xFF000000) >> 24);
  }
  */
}


// Конфигурация клока и питания
static void RCC_Config(void)
{ 
  // Конфигурация SYSCLK, HCLK, PCLK2 и PCLK1 
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    RCC_PCLK1Config(RCC_HCLK_Div2);

#ifdef STM32F10X_CL	// обычно к 105-107 сериям подключают кварц в 25 МГц, в то время как к остальным - 8 МГц
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);
    RCC_PLL2Cmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
    {}
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#else
    // PLLCLK = 8MHz * 9 = 72 MHz
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#endif

    RCC_PLLCmd(ENABLE);

    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  else
  {
  	// Если ошибка старта HSE, будет неверная конфигурация клока 
	// Можно эту ситуацию тут обработать

    while (1)
    {
    }
  }
}


// Конфигурация пина, отвечающего за программный коннект-дисконнект USB
// RCC_APB2Periph_GPIO_DISCONNECT, USB_DISCONNECT, USB_DISCONNECT_PIN необходимо править в platform_config.h
#ifdef USB_DISCONNECT
void USB_Disconnect_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Включаем нужный клок
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);


  // Пин конфигурируется как Out Push-Pull
  GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

  // Начальное значение - лог. 1. Соответствует дисконнекту USB.
  GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
}
#endif

// Конец файла
