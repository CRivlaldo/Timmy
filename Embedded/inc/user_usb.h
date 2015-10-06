#ifndef _USER_USB_H
#define _USER_USB_H

// Дефайны

#define TIMMYCMD_SIMPLE 				0x00
#define TIMMYCMD_SIMPLE_LENGTH 		0x0B

#define TIMMYCMD_SIMPLE_PING 			0x0000

#define TIMMYCMD_SIMPLE_LED 			0x0100
#define TIMMYCMD_SIMPLE_LED_STATE		0x00
#define TIMMYCMD_SIMPLE_LED_OFF		0x01
#define TIMMYCMD_SIMPLE_LED_ON		0x10
#define TIMMYCMD_SIMPLE_LED_BLINK		0x20 
#define TIMMYCMD_SIMPLE_SERVO_PULSE		0x30 

// Функции

int usb_handle_command();
void handle_led_request(uint8_t *request, int nRead);

#endif

