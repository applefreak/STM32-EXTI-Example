#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "2digit.h"

uint8_t ten,one;

void setDisplay(uint8_t d) {
	ten = d/10;
	one = d-(ten*10);	
}

void multiplex(uint8_t d) {
	if(d) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);
		GPIO_SetBits(GPIOB, GPIO_Pin_15);
		output(one);
	}
	else {
		GPIO_SetBits(GPIOB, GPIO_Pin_14);
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);
		output(ten);
	}
}

void output(uint16_t d) {
	switch(d) {
		case 0:
			GPIO_Write(GPIOC, 0xC0); break;
		case 1:
			GPIO_Write(GPIOC, 0xF9); break;
		case 2:
			GPIO_Write(GPIOC, 0xA4); break;
		case 3:
			GPIO_Write(GPIOC, 0xB0); break;
		case 4:
			GPIO_Write(GPIOC, 0x99); break;
		case 5:
			GPIO_Write(GPIOC, 0x92); break;
		case 6:
			GPIO_Write(GPIOC, 0x82); break;
		case 7:
			GPIO_Write(GPIOC, 0xF8); break;
		case 8:
			GPIO_Write(GPIOC, 0x80); break;
		case 9:
			GPIO_Write(GPIOC, 0x90); break;
	}
}

void dispGPIO(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14 | GPIO_Pin_15; 

  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);
}

void DTIM_Config(void) {
	//Timer NVIC Configs
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//Timer configs
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period=1000-1;
	TIM_TimeBaseStructure.TIM_Prescaler=SystemCoreClock/100000-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
