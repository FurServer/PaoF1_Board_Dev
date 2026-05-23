#ifndef SWITCH_I2C1_H
#define SWITCH_I2C1_H

#include <stdint.h>
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

typedef enum
{
    I2C1_OFF = 0u,
    I2C1_ON
} I2C1_State;

void switch_i2c1(uint8_t stat);


#endif
