#ifndef FUNCTION_H
#define FUNCTION_H

#include "main.h"
#include <stdio.h>

#include "i2c_scanner.h"
#include "shell.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;


// LED
void LED_GREEN(void);
void LED_BLUE(void);
void LED_TOGGLE(void);

// CS Select
void CS_Select(uint8_t cs);
void CS_Deselect(void);


#endif
