#include "switch_i2c1.h"

void switch_i2c1(uint8_t stat)
{
    if (stat)
    {
        __HAL_RCC_I2C1_CLK_ENABLE();
        HAL_I2C_Init(&hi2c1);
    } else
    {
        __HAL_RCC_I2C1_CLK_DISABLE();
    }
}