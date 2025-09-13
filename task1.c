#include "stm32f10x.h"       //thu vien CMSIS cua stm32f103
#include "stm32f10x_rcc.h"   //quan ly RCC
#include "stm32f10x_gpio.h"  //cau hinh gpio
#include "stm32f10x_exti.h"  //cau hinh ngat ngoai (EXTI)
#include "misc.h"            //cau hinh NVIC ( ngat trong ARM Cortex)

#define LED_PORT      GPIOA             // led noi cong A
#define LED_PIN       GPIO_Pin_12       // led trên chân PA12
#define BUTTON_PORT   GPIOA             // nut nhan noi o cong A
#define BUTTON_PIN    GPIO_Pin_9        // nut nhan o chan PA9

static void GPIO_Config(void)           //cau hinh GPIO
{
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

 
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin   = LED_PIN;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LED_PORT, &gpio);

 
    gpio.GPIO_Pin   = BUTTON_PIN;
    gpio.GPIO_Mode  = GPIO_Mode_IPU;  
    GPIO_Init(BUTTON_PORT, &gpio);
}

static void EXTI_Config(void)
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

    
    GPIO_ResetBits(LED_PORT, LED_PIN);

    while (1) {
        
        __NOP();
    }
}

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        for (volatile int i = 0; i < 50000; i++) __NOP();
        if (GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN) == Bit_RESET)
        {
            LED_PORT->ODR ^= LED_PIN;
        }

        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}
