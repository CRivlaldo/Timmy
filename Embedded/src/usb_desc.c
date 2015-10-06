/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : Ezhik (thx to MCD Application Team)
* Version            : V1.0.0
* Date               : 09/06/2011
* Description        : ����������� USB-�������
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"

// $USBCONFIG - ��� ���� ��������� ��������� ��� �����������
// ������� ������������: 
// 1 ������������, 1 ���������
// 4 bulk-���������: 0x81 in, 0x02 out, 0x83 in, 0x04 out
// ��� ��������� ������ ����������� ����� ���������� ��������� ��� ������ � ����� usb_desc.h

const uint8_t Timmy_DeviceDescriptor[Timmy_SIZ_DEVICE_DESC] =
  {
    0x12,   /* bLength  */
    0x01,   /* bDescriptorType */
    0x00,   /* bcdUSB, version 2.00 */
    0x02,
    0xff,   /* bDeviceClass : each interface define the device class */
    0xff,   /* bDeviceSubClass */
    0xff,   /* bDeviceProtocol */
    0x40,   /* bMaxPacketSize0 0x40 = 64 */
    
	// $USBCONFIG - ���� �������� vendor ID (VID). ��� ����� ������ � usb.org �� ��������� ����� �������� :)
	// � ������ ������� ������������ VID �� ST Microelectrinics
	// � ��� �� �����-�� �������� ����� �������� PID ��� ������ ����������
	0x83,   /* idVendor     (0483) */
    0x04,
	// $USBCONFIG - ���� �������� product ID (PID)
	// � ������ ������� _������ � ����� ������������ � ������������_ ������������ PID "�� �����".
    0xF0,   /* idProduct	(FFF0) */
    0xFF,
    0x00,   /* bcdDevice 2.00*/
    0x02,
    1,              /* index of string Manufacturer  */
    /**/
    2,              /* index of string descriptor of product*/
    /* */
    3,              /* */
    /* */
    /* */
    0x01    /*bNumConfigurations */
  };
const uint8_t Timmy_ConfigDescriptor[Timmy_SIZ_CONFIG_DESC] =
  {

    0x09,   /* bLength: Configuation Descriptor size */
    0x02,   /* bDescriptorType: Configuration */
    Timmy_SIZ_CONFIG_DESC,

    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: */
    /*      Configuration value */
    0x00,   /* iConfiguration: */
    /*      Index of string descriptor */
    /*      describing the configuration */
    0xC0,   /* bmAttributes: */
    /*      bus powered */
    0x32,   /* MaxPower 100 mA */

    /******************** Descriptor of Mass Storage interface ********************/
    /* 09 */
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints*/
    0xff,   /* bInterfaceClass */
    0xff,   /* bInterfaceSubClass */
    0xff,   /* nInterfaceProtocol */
    4,          /* iInterface: */
    /* 18 */
    0x07,   /*Endpoint descriptor length = 7*/
    0x05,   /*Endpoint descriptor type */
    0x81,   /*Endpoint address (IN, address 1) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,   /*Polling interval in milliseconds */
    /* 25 */
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x02,   /*Endpoint address (OUT, address 2) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,     /*Polling interval in milliseconds*/
    /*32*/
	
    0x07,   //Endpoint descriptor length = 7
    0x05,   //Endpoint descriptor type 
    0x83,   //Endpoint address (IN, address 3) 
    0x02,   //Bulk endpoint type 
    0x40,   //Maximum packet size (64 bytes) 
    0x00,
    0x00,   //Polling interval in milliseconds 
    // 39 
    0x07,   //Endpoint descriptor length = 7 
    0x05,   //Endpoint descriptor type 
    0x04,   //Endpoint address (OUT, address 4) 
    0x02,   //Bulk endpoint type 
    0x40,   //Maximum packet size (64 bytes) 
    0x00,
    0x00     //Polling interval in milliseconds
    //46
	// */
  };


// ��������� ��������������


// ����
const uint8_t Timmy_StringLangID[Timmy_SIZ_STRING_LANGID] =
  {
    Timmy_SIZ_STRING_LANGID,
    0x03,
    0x09,
    0x04
  };      /* LangID = 0x0409: U.S. English */

// Vendor
const uint8_t Timmy_StringVendor[Timmy_SIZ_STRING_VENDOR] =
  {
    Timmy_SIZ_STRING_VENDOR, /* Size of manufaturer string */
    0x03,           /* bDescriptorType = String descriptor */
    /* Manufacturer */
    'S', 0, 
	'o', 0, 
	'f', 0, 
	'a', 0, 
	' ', 0, 
	'T', 0, 
	'e', 0, 
	'c', 0, 
	'h', 0,
	'n', 0,
	'o', 0,
	'l', 0,
	'o', 0,
	'g', 0,
	'i', 0,
	'e', 0,
	's', 0
  };

// �������
const uint8_t Timmy_StringProduct[Timmy_SIZ_STRING_PRODUCT] =
  {
    Timmy_SIZ_STRING_PRODUCT,
    0x03,
    /* Product name */
    'T', 0,
	'i', 0,
	'm', 0,
	'm', 0,
	'y', 0
		
  };

// $BOARDSPECIFIC - �������� �����
uint8_t Timmy_StringSerial[Timmy_SIZ_STRING_SERIAL] =
  {
    Timmy_SIZ_STRING_SERIAL,
    0x03,
    /* Serial number */
#if defined STRIVE_BOARD
    'S', 0,
	'T', 0,
	'R', 0,
	'V', 0,
	'0', 0,
	'0', 0,
	'1', 0
#elif defined EM_STM3210C_BOARD
    'E', 0,
	'M', 0,
	'B', 0,
	'C', 0,
	'0', 0,
	'0', 0,
	'1', 0
#elif defined TE_STM32F103_BOARD
    'T', 0,
	'E', 0,
	'R', 0,
	'3', 0,
	'0', 0,
	'0', 0,
	'1', 0
#else
    'D', 0,
	'E', 0,
	'F', 0,
	'T', 0,
	'0', 0,
	'0', 0,
	'1', 0
#endif
  };

// ���������
const uint8_t Timmy_StringInterface[Timmy_SIZ_STRING_INTERFACE] =
  {
    Timmy_SIZ_STRING_INTERFACE,
    0x03,
    /* Interface 0 */
    'I', 0,
	'n', 0,
	't', 0,
	'e', 0,
	'r', 0,
	'f', 0,
	'a', 0,
	'c', 0,
	'e', 0
  };

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
