#include "test.h"

#include <stdio.h>

#include "lfs.h"
#include "lfs_char_utils.h"
#include "w25qxx.h"
#include "spi.h"
#include "test.h"

int test_main(void)
{
    systick_config();

    // LED Clock
    rcu_periph_clock_enable(RCU_GPIOB);

    // LED Port / Pins
    gpio_mode_set(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
    gpio_output_options_set(LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_PIN);

    // USART Clock
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART1);

    // USART Port / Pins
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2 | GPIO_PIN_3);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2 | GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2 | GPIO_PIN_3);

    // USART Configuration
    usart_deinit(USART1);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_baudrate_set(USART1, 115200U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);

    usart_enable(USART1);

    // SPI Initializaation
    spi_parameter_struct spi_init_struct;

    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_SPI0);

    // SPI0 GPIO configuration: SCK/PA5, MISO/PA6, MOSI/PA7
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    // NSS Configure and set high to deselect Flash
    gpio_mode_set(W25NSS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, W25NSS_PIN);
    gpio_output_options_set(W25NSS_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, W25NSS_PIN);
    gpio_bit_set(W25NSS_PORT, W25NSS_PIN);

    // Configure SPI0 initialisation structure
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    spi_crc_polynomial_set(SPI0, 7);

    spi_enable(SPI0);

    gpio_bit_reset(W25NSS_PORT, W25NSS_PIN);

    // uint32_t d1 = spi_master_write(SPI0, 0x9F);
    // uint32_t d2 = spi_master_write(SPI0, 0x00);
    // uint32_t d3 = spi_master_write(SPI0, 0x00);
    // uint32_t d4 = spi_master_write(SPI0, 0x00);

    uint8_t cmd[1] = { 0x9F };
    spi_data_transmit(SPI0, cmd, 1);

    uint8_t data[3];
    spi_data_receive(SPI0, data, 3);


    gpio_bit_set(W25NSS_PORT, W25NSS_PIN);

    printf("%x %x %x\r\n", data[0], data[1], data[2]);

    W25Qx_Parameter W25Qx_Para;
    W25Qx_Get_Parameter(&W25Qx_Para);

    printf("Flash Id: 0x%lx\r\n", W25Qx_Para.FLASH_ID);
    printf("Capacity: %ldM\r\n", W25Qx_Para.FLASH_CAPACITY / (1024*1024));
    printf("Sector Count: %ld\r\n", W25Qx_Para.SECTOR_COUNT);
    printf("Sector Size: %ld bytes\r\n", W25Qx_Para.SECTOR_SIZE);
    printf("Subsector Count: %ld\r\n", W25Qx_Para.SUBSECTOR_COUNT);
    printf("Subsector Size: %ld bytes\r\n", W25Qx_Para.SUBSECTOR_SIZE);
    printf("Page Size: %ld bytes\r\n", W25Qx_Para.PAGE_SIZE);

    uint8_t we[] = { WRITE_ENABLE_CMD };
    uint8_t rsr1[] = { READ_STATUS_REG1_CMD };
    uint8_t rsr2[] = { READ_STATUS_REG2_CMD };
    uint8_t wsr1[] = { WRITE_STATUS_REG1_CMD, 0x00, 0x00 };
    //uint8_t wsr2[] = { WRITE_STATUS_REG2_CMD };
    //uint8_t ce[] = { CHIP_ERASE_CMD };
    uint8_t wd[] = { WRITE_DISABLE_CMD };

    uint8_t status;

    W25Qx_Enable();
    spi_data_transmit(SPI0, rsr1, 1);
    spi_data_receive(SPI0, &status, 1);
    W25Qx_Disable();

    W25Qx_Enable();
    spi_data_transmit(SPI0, rsr2, 1);
    spi_data_receive(SPI0, &status, 1);
    W25Qx_Disable();

    W25Qx_Enable();
    spi_data_transmit(SPI0, we, 1);
    W25Qx_Disable();

    delay_1ms(100);

    do {
        W25Qx_Enable();
        spi_data_transmit(SPI0, rsr1, 1);
        spi_data_receive(SPI0, &status, 1);
        W25Qx_Disable();
    }
    while( status & W25QXXXX_FSR_BUSY );

     do {
        W25Qx_Enable();
        spi_data_transmit(SPI0, rsr1, 1);
        spi_data_receive(SPI0, &status, 1);
        W25Qx_Disable();
    }
    while( (status & W25QXXXX_FSR_WREN) == 0 );
   
    
    W25Qx_Enable();
    spi_data_transmit(SPI0, wsr1, 3);
    W25Qx_Disable();

    do {
        W25Qx_Enable();
        spi_data_transmit(SPI0, rsr1, 1);
        spi_data_receive(SPI0, &status, 1);
        W25Qx_Disable();
    }
    while( status & W25QXXXX_FSR_BUSY );

    W25Qx_Enable();
    spi_data_transmit(SPI0, wd, 1);
    W25Qx_Disable();

    W25Qx_Enable();
    spi_data_transmit(SPI0, rsr1, 1);
    spi_data_receive(SPI0, &status, 1);
    W25Qx_Disable();

    W25Qx_Enable();
    spi_data_transmit(SPI0, rsr2, 1);
    spi_data_receive(SPI0, &status, 1);
    W25Qx_Disable();


    while (1)
    {
        gpio_bit_set(LED_PORT, LED_PIN);
        delay_1ms(50);
        gpio_bit_reset(LED_PORT, LED_PIN);
        delay_1ms(500);

        printf("Loop\r\n");
    }
}

