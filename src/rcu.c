#include "gd32e23x.h"
#include "main.h"
#include "rcu.h"

void rcu_config(void)
{
    rcu_periph_clock_enable(W25NSS_CLOCK);
    rcu_periph_clock_enable(LED_CLOCK);
    rcu_periph_clock_enable(RCU_SPI0);
}
