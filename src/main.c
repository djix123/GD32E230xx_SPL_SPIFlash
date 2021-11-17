#include "gd32e23x.h"

#include "main.h"

#include <stdio.h>

#include "rcu.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"

#include "test.h"
#include "lfs_demo.h"

//#define __TEST__
#define __LFS_DEMO__


int _write(int fd, char *ch, int len)
{
    int count = 0;
    while(count < len) {
        usart_data_transmit(USART1, (uint8_t) ch[count++]);
        while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    }
    return len;
}

int main(void)
{
    systick_config();

    rcu_config();
    gpio_config();

    com_gpio_init();
    com_usart_init();

    spi_config();
    
#ifdef __TEST__
    test_main();
#endif

#ifdef __LFS_DEMO__
    lfs_demo();
#endif
}

volatile static uint32_t delay;

void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        /* capture error */
        while (1)
        {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

void delay_1ms(uint32_t count)
{
    delay = count;

    while (0U != delay)
    {
    }
}

void delay_decrement(void)
{
    if (0U != delay)
    {
        delay--;
    }
}

void NMI_Handler(void) {}

void HardFault_Handler(void)
{
    while (1)
        ;
}

void MemManage_Handler(void)
{
    while (1)
        ;
}

void BusFault_Handler(void)
{
    while (1)
        ;
}

void UsageFault_Handler(void)
{
    while (1)
        ;
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    delay_decrement();
}