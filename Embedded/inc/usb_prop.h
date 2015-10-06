/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : All processing related to Mass Storage Demo (Endpoint 0)
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usb_prop_H
#define __usb_prop_H
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

// $USBCONFIG - раскомментировать те запросы, которые будут обрабатываться

#define Timmy_GetConfiguration          NOP_Process
/* #define Timmy_SetConfiguration          NOP_Process*/
#define Timmy_GetInterface              NOP_Process
#define Timmy_SetInterface              NOP_Process
#define Timmy_GetStatus                 NOP_Process
#define Timmy_ClearFeature              NOP_Process
#define Timmy_SetEndPointFeature        NOP_Process
#define Timmy_SetDeviceFeature          NOP_Process
/*#define Timmy_SetDeviceAddress          NOP_Process*/


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Timmy_init(void);
void Timmy_Reset(void);
void Timmy_SetConfiguration(void);
void Timmy_ClearFeature(void);
void Timmy_SetDeviceAddress (void);
void Timmy_Status_In (void);
void Timmy_Status_Out (void);
RESULT Timmy_Data_Setup(uint8_t);
RESULT Timmy_NoData_Setup(uint8_t);
RESULT Timmy_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *Timmy_GetDeviceDescriptor(uint16_t );
uint8_t *Timmy_GetConfigDescriptor(uint16_t);
uint8_t *Timmy_GetStringDescriptor(uint16_t);

#endif /* __usb_prop_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

