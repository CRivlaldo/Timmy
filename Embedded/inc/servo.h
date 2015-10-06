#ifndef _SERVO_H
#define _SERVO_H

#include "stm32f10x.h"

#define MIN_PULSE_WIDTH 771
#define MAX_PULSE_WIDTH 2193
#define MAX_ROTATION 120

void Servo_Init();
void Servo_SetRotation(int servoId, uint16_t rotation);

#endif