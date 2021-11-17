#include "gd32e23x.h"
#include "main.h"
#include "gpio.h"

void gpio_config(void)
{
    // LED
    gpio_mode_set(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
    gpio_output_options_set(LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_PIN);

    // SPI0 NSS - Software Controlled
    gpio_bit_set(W25NSS_PORT, W25NSS_PIN);
    gpio_mode_set(W25NSS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, W25NSS_PIN);
    gpio_output_options_set(W25NSS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, W25NSS_PIN);

    /* SPI0 GPIO configuration: SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
}
