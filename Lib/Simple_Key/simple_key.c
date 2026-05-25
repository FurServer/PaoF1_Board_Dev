#include "simple_key.h"


void Key_Init(Key_t* key, GPIO_TypeDef* port, uint16_t pin, uint8_t trig_level, uint32_t debounce)
{
    key->Port = port;
    key->Pin = pin;
    key->TrigLevel = trig_level;
    key->Debounce = debounce;
    key->Tick = 0;
    key->Flag = !trig_level;
}

uint8_t Key_Scan(Key_t* key)
{
    key->Flag = HAL_GPIO_ReadPin(key->Port, key->Pin);

    if (key->Flag == key->TrigLevel)
    {
        if (HAL_GetTick() - key->Tick < key->Debounce)
        {
            key->Flag = !key->TrigLevel;
        }
        key->Tick = HAL_GetTick();
    }

    if (key->Flag == key->TrigLevel)
    {
        return 1;
    }
    return 0;
}
