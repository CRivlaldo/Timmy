/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : All processing related to Mass Storage Demo
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
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "usb_prop.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    Timmy_init,
    Timmy_Reset,
    Timmy_Status_In,
    Timmy_Status_Out,
    Timmy_Data_Setup,
    Timmy_NoData_Setup,
    Timmy_Get_Interface_Setting,
    Timmy_GetDeviceDescriptor,
    Timmy_GetConfigDescriptor,
    Timmy_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Timmy_GetConfiguration,
    Timmy_SetConfiguration,
    Timmy_GetInterface,
    Timmy_SetInterface,
    Timmy_GetStatus,
    Timmy_ClearFeature,
    Timmy_SetEndPointFeature,
    Timmy_SetDeviceFeature,
    Timmy_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)Timmy_DeviceDescriptor,
    Timmy_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)Timmy_ConfigDescriptor,
    Timmy_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR String_Descriptor[5] =
  {
    {(uint8_t*)Timmy_StringLangID, Timmy_SIZ_STRING_LANGID},
    {(uint8_t*)Timmy_StringVendor, Timmy_SIZ_STRING_VENDOR},
    {(uint8_t*)Timmy_StringProduct, Timmy_SIZ_STRING_PRODUCT},
    {(uint8_t*)Timmy_StringSerial, Timmy_SIZ_STRING_SERIAL},
    {(uint8_t*)Timmy_StringInterface, Timmy_SIZ_STRING_INTERFACE},
  };

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Timmy_init
* Description    : Device init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Timmy_init()
{
  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();

  pInformation->Current_Configuration = 0;

  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : Timmy_Reset
* Description    : Device reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Timmy_Reset()
{
  /* Set the device as not configured */
  Device_Info.Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = Timmy_ConfigDescriptor[7];


#ifdef STM32F10X_CL   
  
// $USBCONFIG - Тут нужно добавить/удалить инициализацию ендпойнтов при их изменении

  /* EP0 is already configured by USB_SIL_Init() function */

  /* Init EP1 IN as Bulk endpoint */
  OTG_DEV_EP_Init(EP1_IN, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);
  
  /* Init EP2 OUT as Bulk endpoint */
  OTG_DEV_EP_Init(EP2_OUT, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE); 
  
  /* Init EP1 IN as Bulk endpoint */
  OTG_DEV_EP_Init(EP3_IN, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);
  
  /* Init EP2 OUT as Bulk endpoint */
  OTG_DEV_EP_Init(EP4_OUT, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE); 
  
#else 

// $USBCONFIG - Тут нужно добавить/удалить инициализацию ендпойнтов при их изменении

  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_NAK);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_BULK);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxStatus(ENDP1, EP_TX_NAK);
  SetEPRxStatus(ENDP1, EP_RX_DIS);

  /* Initialize Endpoint 2 */
  SetEPType(ENDP2, EP_BULK);
  SetEPRxAddr(ENDP2, ENDP2_RXADDR);
  SetEPRxCount(ENDP2, Device_Property.MaxPacketSize);
  SetEPRxStatus(ENDP2, EP_RX_VALID);
  SetEPTxStatus(ENDP2, EP_TX_DIS);

 
  // Initialize Endpoint 3 
  SetEPType(ENDP3, EP_BULK);
  SetEPTxAddr(ENDP3, ENDP3_TXADDR);
  SetEPTxStatus(ENDP3, EP_TX_NAK);
  SetEPRxStatus(ENDP3, EP_RX_DIS);

  // Initialize Endpoint 4 
  SetEPType(ENDP4, EP_BULK);
  SetEPRxAddr(ENDP4, ENDP4_RXADDR);
  SetEPRxCount(ENDP4, Device_Property.MaxPacketSize);
  SetEPRxStatus(ENDP4, EP_RX_VALID);
  SetEPTxStatus(ENDP4, EP_TX_DIS);
// */

  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Set the device to response on default address */
  SetDeviceAddress(0);
#endif /* STM32F10X_CL */

  bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : Timmy_SetConfiguration
* Description    : Handle the SetConfiguration request.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Timmy_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;

#ifdef STM32F10X_CL 

	// $USBCONFIG - Тут нужно добавить/удалить инициализацию ендпойнтов при их изменении

    /* Init EP1 IN as Bulk endpoint */
    OTG_DEV_EP_Init(EP1_IN, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);
  
    /* Init EP2 OUT as Bulk endpoint */
    OTG_DEV_EP_Init(EP2_OUT, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);     
    /* Init EP3 IN as Bulk endpoint */
    OTG_DEV_EP_Init(EP3_IN, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);
  
    /* Init EP4 OUT as Bulk endpoint */
    OTG_DEV_EP_Init(EP4_OUT, OTG_DEV_EP_TYPE_BULK, BULK_MAX_PACKET_SIZE);     
#else    
	// $USBCONFIG - Тут нужно добавить/удалить очистку DTOG_bit ендпойнтов при их изменении

    ClearDTOG_TX(ENDP1);
    ClearDTOG_RX(ENDP2);
    ClearDTOG_TX(ENDP3);
    ClearDTOG_RX(ENDP4);
#endif /* STM32F10X_CL */

  }
}

/*******************************************************************************
* Function Name  : Timmy_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Timmy_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : Timmy_Status_In
* Description    : Device Status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Timmy_Status_In(void)
{
  return;
}

/*******************************************************************************
* Function Name  : Timmy_Status_Out
* Description    : Device Status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Timmy_Status_Out(void)
{
  return;
}

/*******************************************************************************
* Function Name  : Timmy_Data_Setup.
* Description    : Handle the data class specific requests..
* Input          : RequestNo.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT Timmy_Data_Setup(uint8_t RequestNo)
{
  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Timmy_NoData_Setup.
* Description    : Handle the no data class specific requests.
* Input          : RequestNo.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT Timmy_NoData_Setup(uint8_t RequestNo)
{
  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Timmy_Get_Interface_Setting
* Description    : Test the interface and the alternate setting according to the
*                  supported one.
* Input          : uint8_t Interface, uint8_t AlternateSetting.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT Timmy_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;/* in this application we don't have AlternateSetting*/
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;/*in this application we have only 1 interfaces*/
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Timmy_GetDeviceDescriptor
* Description    : Get the device descriptor.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *Timmy_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor );
}

/*******************************************************************************
* Function Name  : Timmy_GetConfigDescriptor
* Description    : Get the configuration descriptor.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *Timmy_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor );
}

/*******************************************************************************
* Function Name  : Timmy_GetStringDescriptor
* Description    : Get the string descriptors according to the needed index.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t *Timmy_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;

  if (wValue0 > 5)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}
