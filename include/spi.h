#pragma once

#include "gd32e23x.h"

void spi_config(void);

uint32_t spi_master_write(uint32_t spi_periph, uint8_t value);
void spi_data_transmit(uint32_t spi_periph, uint8_t *data, uint32_t length);
void spi_data_receive(uint32_t spi_periph, uint8_t *data, uint32_t length);
