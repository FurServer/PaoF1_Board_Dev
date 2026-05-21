#include "function.h"

// CS Select
void CS_Select(uint8_t cs)
{
    static uint8_t last_cs = 0;
    if (cs != last_cs)
    {
        HAL_GPIO_WritePin(decoderE_GPIO_Port, decoderE_Pin, GPIO_PIN_RESET);
        last_cs = cs;
        HAL_GPIO_WritePin(decoderA_GPIO_Port, decoderA_Pin, (cs & 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(decoderB_GPIO_Port, decoderB_Pin, (cs & 2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(decoderE_GPIO_Port, decoderE_Pin, GPIO_PIN_SET);
}

void CS_Deselect(void)
{
    HAL_GPIO_WritePin(decoderE_GPIO_Port, decoderE_Pin, GPIO_PIN_RESET);
}
