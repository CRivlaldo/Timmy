#include "usb_lib.h"

#include "user_usb.h"
#include "led.h"
#include "servo.h"
#include "hw_config.h"
#include "platform_config.h"

#include <stdio.h>

bool led_state_ = FALSE;

// ������ � ��������� ���������� �������
int usb_handle_command()
{
	int nRead;

	uint8_t request[64];
	uint8_t answer[64];

	uint8_t command_type;
	uint16_t command_id;
	uint16_t *command_id_ptr;

	int i;

	for(i=0;i<64;i++)
	{
		request[i] = 0;
	}

	nRead = USB_SIL_Read(EP2_OUT, request);
	request[nRead] = 0; 
	command_type = request[0];
	command_id = *(uint16_t *)(request + 1);

	#ifdef _DEBUG_
	printf("Command type %d id %d received\r\n", command_type, command_id);
	#endif

	switch(command_type)
	{
		case TIMMYCMD_SIMPLE:
			#ifdef _DEBUG_
			printf("Simple command\r\n");
			#endif

			if(nRead != TIMMYCMD_SIMPLE_LENGTH)
			{
				#ifdef _DEBUG_
				printf("Bad length = %d\r\n", nRead);
				#endif
				for(i=1;i<64;i++)
					answer[i] = 0;
				answer[0] = 0xff; // bad command
				USB_SIL_Write(EP1_IN, answer, nRead);
				break;
			}
			switch (command_id)
			{
				case TIMMYCMD_SIMPLE_PING:
					#ifdef _DEBUG_
					printf("Ping command %d %d %d %d\r\n", request[3], request[4], request[5], request[6]);
					#endif
					// else ������ ���� ���� �� ����� ������� �������
					USB_SIL_Write(EP1_IN, request, nRead);
					break;
				case TIMMYCMD_SIMPLE_LED:
					handle_led_request(request, nRead);
					break;
				default:
					#ifdef _DEBUG_
					printf("Unknown command id\r\n");
					#endif
					for(i=1;i<64;i++)
						answer[i] = 0;
					answer[0] = 0xff; // bad command
					answer[1] = 0xff; // bad command
					answer[2] = 0xff; // bad command
					USB_SIL_Write(EP1_IN, answer, nRead);
					break;
			}
			break;
		default:
				#ifdef _DEBUG_
				printf("Unknown command type\r\n");
				#endif
				for(i=1;i<64;i++)
					answer[i] = 0;
				answer[0] = 0xff; // bad command
				USB_SIL_Write(EP1_IN, answer, nRead);
				break;
			break;
	}

	// ���������� �����
	SetEPTxValid(ENDP1);
}

void handle_led_request(uint8_t *request, int nRead)
{
	uint8_t led_command = request[3];
	uint8_t led_number = request[4];
	uint16_t blink_on = *(uint16_t *)(request + 5);
	uint16_t blink_off = *(uint16_t *)(request + 7);
	uint16_t servo_number;
	uint16_t rotation;

	#ifdef _DEBUG_
	printf("LED Command: %d %d %d %d\r\n", led_command, led_number, blink_on, blink_off);
	#endif
	switch(led_command)
	{
		case TIMMYCMD_SIMPLE_LED_STATE:
			request[3] = LED_GetBlinkState(led_number, (uint16_t *)(request + 5), (uint16_t *)(request + 7));
			if(led_number == 0xff)
			{
				request[4] = LEDS_NUM;
				#ifdef _DEBUG_
				printf("Requested LED_NUM. Answer - %d\r\n", request[4]);
				#endif
			}
			USB_SIL_Write(EP1_IN, request, nRead);
			break;
		case TIMMYCMD_SIMPLE_LED_OFF:
			LED_Off(led_number);
			request[3] = LED_GetState(led_number);
		    USB_SIL_Write(EP1_IN, request, nRead);
			break;
		case TIMMYCMD_SIMPLE_LED_ON:
			LED_On(led_number);
			request[3] = LED_GetState(led_number);
			USB_SIL_Write(EP1_IN, request, nRead);
			break;
		case TIMMYCMD_SIMPLE_LED_BLINK:
			LED_OnBlink(led_number, blink_on, blink_off);
			request[3] = LED_GetBlinkState(led_number, (uint16_t *)(request + 5), (uint16_t *)(request + 7));
			USB_SIL_Write(EP1_IN, request, nRead);
			break;
		case TIMMYCMD_SIMPLE_SERVO_PULSE:
			servo_number = request[4];
			rotation = *(uint16_t *)(request + 5);
			Servo_SetRotation(servo_number, rotation);
			USB_SIL_Write(EP1_IN, request, nRead);
			break;
	}
}

