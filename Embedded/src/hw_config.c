/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : Ezhik (thx to MCD Application Team)
* Version            : V1.0.0
* Date               : 09/06/2011
* Description        : ������������ � ���������
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

// ������������� ����� � �������
void Set_System(void)
{

  	// RCC configuration 
 	RCC_Config();

#ifdef USB_DISCONNECT
	// ������������ ����, ������������ USB
	USB_Disconnect_Config();

#endif

}


// ������������� USART
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
	

	// TX ��� Alternate Function Push-Pull
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = COM1_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM1_GPIO, &GPIO_InitStructure);

	// RX ��� Floating Input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = COM1_RxPin;
	GPIO_Init(COM1_GPIO, &GPIO_InitStructure);

	// 115200 ���, 8 ���, 1 ����, N ��������, ��� ��������, ��������� ����� � ��������
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM1, &USART_InitStructure);

	// ������������ ����������
	NVIC_InitStructure.NVIC_IRQChannel = COM1_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// �������� ���������� �� ���������
	USART_ITConfig(COM1, USART_IT_RXNE, ENABLE);

	
	USART_Cmd(COM1, ENABLE);
}

// ���������� ���������� USART, � ������ ������ - ��������� �����
// ����� - ����� ������� ������ �� ��������.
// ����� ���������� �� ��������� ������
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

// ��������� fputc ��� ������ stdout �� USART1
int fputc(int ch, FILE *f)
{
	USART_SendData(COM1, (uint8_t) ch);

	while (USART_GetFlagStatus(COM1, USART_FLAG_TXE) == RESET)
	{}
	
	return ch;
}

// ������ ����� USB (48 ���)
void Set_USBClock(void)
{
#ifndef STM32F10X_CL
	// ��������
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	
	// �������� ���� USB
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

#else
	// ��������
  	RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);

  	// �������� ���� USB
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;

#endif
  
}



// ����� ������� �����������������. ����� ����� ��� ��������� ��� ������ �����.
void Enter_LowPowerMode(void)
{
  // ��������� ��������� ������� � Suspended
  bDeviceState = SUSPENDED;
}

// ����� �� ������ ������� �����������������. �������� ����� �������.
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  // ������ ���������� ��������� �������
  if (pInfo->Current_Configuration != 0)
  {
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }

}

// ������������ USB-����������
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

// ������������ ����������� 
void Periphery_Config(void)
{
	// �������� � led.c, ��� ��� ����������� ����� ���� ������ ������, � ��� ��� ���������
	Led_Init();
	Servo_Init();
}

// ����������� �������-���������� USB-������
void USB_Cable_Config (FunctionalState NewState)
{
#ifdef USB_DISCONNECT
  if (NewState != USB_DISCONNECT_LOG1)
  {
  	// ���. 0 - �������
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
  	// ���. 1 - ����������
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
#endif
}


// �������� ��������� ������ ����������.
// �� �����������. ������ ��� ������������� � VID, PID � Windows-��������� �� STM
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


// ������������ ����� � �������
static void RCC_Config(void)
{ 
  // ������������ SYSCLK, HCLK, PCLK2 � PCLK1 
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

#ifdef STM32F10X_CL	// ������ � 105-107 ������ ���������� ����� � 25 ���, � �� ����� ��� � ��������� - 8 ���
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
  	// ���� ������ ������ HSE, ����� �������� ������������ ����� 
	// ����� ��� �������� ��� ����������

    while (1)
    {
    }
  }
}


// ������������ ����, ����������� �� ����������� �������-���������� USB
// RCC_APB2Periph_GPIO_DISCONNECT, USB_DISCONNECT, USB_DISCONNECT_PIN ���������� ������� � platform_config.h
#ifdef USB_DISCONNECT
void USB_Disconnect_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // �������� ������ ����
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);


  // ��� ��������������� ��� Out Push-Pull
  GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

  // ��������� �������� - ���. 1. ������������� ����������� USB.
  GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
}
#endif

// ����� �����
