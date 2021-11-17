#include "spi.h"
#include "main.h"

void spi_config(void)
{
    spi_parameter_struct spi_init_struct;

    /* deinitilize SPI and the parameters */
    spi_disable(SPI0);
    spi_i2s_deinit(SPI0);
    
    gpio_bit_set(W25NSS_PORT, W25NSS_PIN);

    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter configuration */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_2;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    spi_crc_polynomial_set(SPI0, 7);
 
    spi_enable(SPI0);
}

uint32_t spi_master_write(uint32_t spi_periph, uint8_t value) 
{
    int count = 0;
    while((RESET == spi_i2s_flag_get(spi_periph, SPI_FLAG_TBE)) && (count++ < 1000));
    if(count >= 1000) {
        return -1;
    }
    else {
        spi_i2s_data_transmit(spi_periph, value);
    }

    count = 0;
    while((RESET == spi_i2s_flag_get(spi_periph, SPI_FLAG_RBNE)) && (count++ < 1000));
    if(count >= 1000) {
        return -1;
    }
    else {
        return spi_i2s_data_receive(spi_periph);
    }
}

void spi_data_transmit(uint32_t spi_periph, uint8_t *data, uint32_t length)
{
    // uint16_t count = 0;
    // while(count < length) {
    //     while(RESET == spi_i2s_flag_get(spi_periph, SPI_FLAG_TBE));
    //     uint16_t x = data[count];
    //     spi_i2s_data_transmit(spi_periph, x);
    //     //while(spi_i2s_flag_get(SPI0, SPI_FLAG_TRANS));
    //     count++;
    // }

    uint32_t count = 0;
    while(count < length) {
        spi_master_write(spi_periph, data[count++]);
    }
}

void spi_data_receive(uint32_t spi_periph, uint8_t *data, uint32_t length)
{
    // uint16_t count = 0;
    // while(count < length) {
    //     while(RESET == spi_i2s_flag_get(spi_periph, SPI_FLAG_RBNE));
    //     uint16_t x = spi_i2s_data_receive(spi_periph);
    //     data[count] = (uint8_t)x;
    //     //while(spi_i2s_flag_get(SPI0, SPI_FLAG_TRANS));
    //     count++;
    // }

    uint32_t count = 0;
    while(count < length) {
        data[count++] = spi_master_write(spi_periph, 0xFF);
    }

    // uint32_t count = 0;
    // while(count < length) {
    //     uint16_t timeout = 0;
    //     while((RESET == spi_i2s_flag_get(spi_periph, SPI_FLAG_RBNE)) && (timeout++ < 1000));
    //     if(timeout >= 1000) {
    //         return;
    //     }
    //     else {
    //         data[count++] = spi_i2s_data_receive(spi_periph);
    //     }
    // }
}
