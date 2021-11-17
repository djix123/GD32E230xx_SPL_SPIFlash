#pragma once

#include "gd32e23x.h"

/* define blinky LED pin here, board specific, otherwise default PC13 */
#define LED_PORT    GPIOB
#define LED_PIN     GPIO_PIN_1
#define LED_CLOCK   RCU_GPIOB

#define W25NSS_PORT     GPIOA
#define W25NSS_PIN      GPIO_PIN_4
#define W25NSS_CLOCK    RCU_GPIOA

void systick_config(void);
void delay_1ms(uint32_t count);

