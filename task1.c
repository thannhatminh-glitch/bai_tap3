/*
#include "stm32f10x.h"  
#include "stm32f10x_gpio.h"            

void gpio_config (void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef gpio ;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_13;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);	
}
void timer_Init(void)
{
	TIM_TimeBaseInitTypeDef timer_init ;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timer_init.TIM_CounterMode	= TIM_CounterMode_Up;
	timer_init.TIM_Period = 65535;
	timer_init.TIM_Prescaler = 1;
	timer_init.TIM_RepetitionCounter = 0;
	timer_init.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &timer_init);
	
	TIM_Cmd(TIM2, ENABLE);
	
	
}	

void delay_1ms (void)
{
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) <36000);
}

void delay_ms(unsigned int time)
{
	while(time--)
	{
		delay_1ms();
	}
}


int main(void)
{
	gpio_config();
  timer_Init();
	while(1)
	{
		GPIOC->ODR^=GPIO_Pin_13;
		delay_ms(1000);
	}
}*/
