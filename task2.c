#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"

// ================== HÀM DELAY ==================
void delay_ms(uint32_t ms)
{
    for(uint32_t i=0; i < ms * 8000; i++) {
        __NOP(); // vòng l?p gi? l?p delay (gi? s? CPU 72 MHz)
    }
}

// ================== C?U HÌNH GPIO ==================
void GPIO_Config(void)
{
    // B?t clock cho GPIOA, GPIOB và AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef gpio;

    // PA12: LED1 (di?u khi?n b?ng nút nh?n)
    gpio.GPIO_Pin   = GPIO_Pin_12;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &gpio);

    // PB0: LED2 (nháy 1 Hz)
    gpio.GPIO_Pin   = GPIO_Pin_0;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &gpio);

    // PA9: Nút nh?n (Input Pull-Up)
    gpio.GPIO_Pin   = GPIO_Pin_9;
    gpio.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &gpio);
}

// ================== C?U HÌNH EXTI ==================
void EXTI_Config(void)
{
    // N?i PA9 d?n EXTI Line9
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource9);

    // C?u hình EXTI Line9
    EXTI_InitTypeDef exti;
    exti.EXTI_Line = EXTI_Line9;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling; // Nh?n nút = c?nh xu?ng
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);

    // C?u hình NVIC cho EXTI9_5
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

// ================== MAIN ==================
int main(void)
{
    SystemInit();      // Kh?i t?o clock h? th?ng (PLL = 72 MHz)
    GPIO_Config();     // C?u hình GPIO
    EXTI_Config();     // C?u hình ng?t ngoài

    // Ban d?u t?t c? LED
    GPIO_ResetBits(GPIOA, GPIO_Pin_12);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);

    while (1)
    {
        // LED2 (PB0) nháy 1Hz: 1 giây sáng, 1 giây t?t
        GPIO_SetBits(GPIOB, GPIO_Pin_0);   // B?t LED2
        delay_ms(1000);                    // Delay 1 giây
        GPIO_ResetBits(GPIOB, GPIO_Pin_0); // T?t LED2
        delay_ms(1000);                    // Delay 1 giây
    }
}

// ================== ISR EXTI ==================
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line9) != RESET) // N?u có ng?t t? PA9
    {
        delay_ms(20); // ch?ng d?i phím ~20ms

        // Ki?m tra l?i n?u nút v?n du?c nh?n
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == Bit_RESET)
        {
            // Ð?o tr?ng thái LED1 (PA12)
            if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12))
                GPIO_ResetBits(GPIOA, GPIO_Pin_12); // N?u dang sáng thì t?t
            else
                GPIO_SetBits(GPIOA, GPIO_Pin_12);   // N?u dang t?t thì b?t
        }

        EXTI_ClearITPendingBit(EXTI_Line9); // Xóa c? ng?t
    }
}
