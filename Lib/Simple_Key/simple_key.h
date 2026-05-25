#ifndef SIMPLE_KEY_H
#define SIMPLE_KEY_H

#include "main.h"

/* 按键对象结构体 */
typedef struct
{
    GPIO_TypeDef* Port; /* 端口 */
    uint16_t Pin; /* 引脚 */
    uint8_t TrigLevel; /* 触发电平 */
    uint32_t Debounce; /* 消抖时间(ms) */
    uint32_t Tick; /* 时间戳 */
    uint8_t Flag; /* 标志位 */
} Key_t;


/**
 * @brief 按键初始化
 *
 * @param key 按键结构体
 * @param port 端口
 * @param pin 引脚
 * @param trig_level 触发电平
 * @param debounce 消抖时长
 */
void Key_Init(Key_t* key, GPIO_TypeDef* port, uint16_t pin, uint8_t trig_level, uint32_t debounce);


/**
 * @brief  按键扫描(单次按下触发)
 * @return 1:按下触发, 0:未触发
 */
uint8_t Key_Scan(Key_t* key);

#endif
