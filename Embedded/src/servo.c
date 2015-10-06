#include "servo.h"
#include "stm32f10x.h"
#include "platform_config.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

typedef struct _servo_params
{
	GPIO_TypeDef* GPIOx;
	uint16_t Pin;
	uint32_t RCC_APB2Periph;
} servo_params;

servo_params servos[] = 
	{
		{SERVO1_GPIO_PORT, SERVO1_GPIO_PIN, SERVO1_GPIO_CLK},
		{SERVO2_GPIO_PORT, SERVO2_GPIO_PIN, SERVO2_GPIO_CLK},
		{SERVO3_GPIO_PORT, SERVO3_GPIO_PIN, SERVO3_GPIO_CLK},
		{SERVO4_GPIO_PORT, SERVO4_GPIO_PIN, SERVO4_GPIO_CLK},	
		{SERVO5_GPIO_PORT, SERVO5_GPIO_PIN, SERVO5_GPIO_CLK},	
		{SERVO6_GPIO_PORT, SERVO6_GPIO_PIN, SERVO6_GPIO_CLK}				
	};
	
void Servo_Init()
{		
	TIM_TimeBaseInitTypeDef timer;
	TIM_OCInitTypeDef TIM_OCConfig;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	int i;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	timer.TIM_Prescaler = 71;//(SystemFrequency / 1000000) - 1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	timer.TIM_Period = 20000;
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM2, &timer);
	TIM_TimeBaseInit(TIM3, &timer);
	
	for(i = 0; i < SERVOS_NUM; i++)
	{
		TIM_OCConfig.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCConfig.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCConfig.TIM_Pulse = (MIN_PULSE_WIDTH + MAX_PULSE_WIDTH) / 2;//!!!initial array
		TIM_OCConfig.TIM_OCPolarity = TIM_OCPolarity_High;

		switch(i)
		{
			case SERVO1:
				TIM_OC1Init(TIM2, &TIM_OCConfig);
				break;
			case SERVO2:
				TIM_OC2Init(TIM2, &TIM_OCConfig);
				break;
			case SERVO3:
				TIM_OC3Init(TIM2, &TIM_OCConfig);
				break;
			case SERVO4:
				TIM_OC4Init(TIM2, &TIM_OCConfig);
				break;
			case SERVO5:
				TIM_OC1Init(TIM3, &TIM_OCConfig);
				break;
			case SERVO6:
				TIM_OC2Init(TIM3, &TIM_OCConfig);
				break;
		}
	}
	 
	 TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
   TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);
	
	 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
   TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);

	for(i = 0; i < SERVOS_NUM; i++)
	{
	  	RCC_APB2PeriphClockCmd(servos[i].RCC_APB2Periph, ENABLE);
	  	GPIO_InitStructure.GPIO_Pin = servos[i].Pin;
  		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  	GPIO_Init(servos[i].GPIOx, &GPIO_InitStructure);	
	}
	
	//Turn on timer 2
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
//	TIM2->CCER |= (TIM_CCER_CC1E);
//	TIM2->CCMR1|= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
//	TIM2->PSC = 72;//!!!????
//	TIM2->ARR = 20000 - 1;
//	TIM2->CCR2 = 20 * 3;
//	TIM2->CR1 |= TIM_CR1_CEN;
}

////!!!vladimir: move it
//void Delay_us(uint32_t ms)
//{
//        volatile uint32_t nCount;
//        RCC_ClocksTypeDef RCC_Clocks;
//    RCC_GetClocksFreq (&RCC_Clocks);

//       nCount=(RCC_Clocks.HCLK_Frequency/10000000)*ms;
//        for (; nCount!=0; nCount--);
//}
uint16_t GetPulseWidthByRotation(uint16_t rotation)
{
	uint32_t pulse = rotation * (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) / MAX_ROTATION;
	return MIN_PULSE_WIDTH + (uint16_t)pulse;
}

void Servo_SetRotation(int servoId, uint16_t rotation)
{
	uint16_t pulseWidth = GetPulseWidthByRotation(rotation);
	
	switch(servoId)
	{
		case SERVO1:
			TIM2->CCR1 = pulseWidth;
			break;
		case SERVO2:
			TIM2->CCR2 = pulseWidth;
			break;
		case SERVO3:
			TIM2->CCR3 = pulseWidth;
			break;
		case SERVO4:
			TIM2->CCR4 = pulseWidth;
			break;
		case SERVO5:
			TIM3->CCR1 = pulseWidth;
			break;
		case SERVO6:
			TIM3->CCR2 = pulseWidth;
			break;
	}
}