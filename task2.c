#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"

void delay_ms(uint32_t ms)
{
    for(uint32_t i=0; i < ms * 8000; i++) {
        __NOP(); 
    }
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef gpio;

    gpio.GPIO_Pin   = GPIO_Pin_12;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Pin   = GPIO_Pin_0;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &gpio);

    gpio.GPIO_Pin   = GPIO_Pin_9;
    gpio.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &gpio);
}

void EXTI_Config(void)
{
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource9);

    EXTI_InitTypeDef exti;
    exti.EXTI_Line = EXTI_Line9;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling;
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);

    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

int main(void)
{
    SystemInit();     
    GPIO_Config();    
    EXTI_Config();    

    GPIO_ResetBits(GPIOA, GPIO_Pin_12);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);

    while (1)
    {
   
        GPIO_SetBits(GPIOB, GPIO_Pin_0);  
        delay_ms(1000);                    
        GPIO_ResetBits(GPIOB, GPIO_Pin_0); 
        delay_ms(1000);                   
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line9) != RESET) 
    {
        delay_ms(20); 

        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == Bit_RESET)
        {
            if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12))
                GPIO_ResetBits(GPIOA, GPIO_Pin_12); 
            else
                GPIO_SetBits(GPIOA, GPIO_Pin_12);  
        }

        EXTI_ClearITPendingBit(EXTI_Line9); 
    }
}

