/**
 * @file    lcd1602_port.c
 * @brief   LCD1602 移植层 —— u8g2 风格消息回调
 *
 * 一个 msg_cb 函数, switch-case 处理所有原子消息。
 * 直接引用项目全局变量 (hi2c1, lcdDC_GPIO_Port 等),
 * 无需 port_ctx 抽象。
 *
 * 硬件配置: PCF8574 (I2C) 输出 D0~D7, RS/RW/E 用独立 GPIO
 */

#include "lcd1602_port.h"
#include "i2c.h"
#include "main.h"       /* lcdDC, lcdRW, lcdE 引脚定义 */

/* ================================================================== */
/* 消息回调 —— 原子硬件操作                                               */
/* ================================================================== */

static uint8_t lcd1602_port_msg_cb(lcd1602_t *lcd, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    (void)lcd;       /* 当前实现直接引用全局变量, 不使用 lcd 句柄 */
    (void)arg_ptr;

    switch (msg) {

    case LCD1602_MSG_INIT:
        /* 需要时在此初始化 GPIO 方向 */
        /* RW = 0 (写) */
        HAL_GPIO_WritePin(lcdRW_GPIO_Port, lcdRW_Pin, GPIO_PIN_RESET);
        break;

    case LCD1602_MSG_SET_RS:
        /*
         * arg_int: 0=指令, 1=数据
         */
        HAL_GPIO_WritePin(lcdDC_GPIO_Port, lcdDC_Pin, arg_int ? GPIO_PIN_SET : GPIO_PIN_RESET);

        break;

    case LCD1602_MSG_WRITE_DATA:
        /*
         * arg_int: 数据值 (8-bit: 完整字节; 4-bit: nibble)
         * 通过 PCF8574 (I2C) 输出到 D0~D7
         */
        HAL_I2C_Master_Transmit(&hi2c1, 0x27 << 1, &arg_int, 1, HAL_MAX_DELAY);
        break;

    case LCD1602_MSG_PULSE_EN:
        /*
         * E 脉冲: 高 → 低, HD44780 在下降沿锁存数据
         */
        HAL_GPIO_WritePin(lcdE_GPIO_Port, lcdE_Pin, GPIO_PIN_SET);
        HAL_Delay(1);   /* > 450ns (E 高电平最小宽度) */
        HAL_GPIO_WritePin(lcdE_GPIO_Port, lcdE_Pin, GPIO_PIN_RESET);
        HAL_Delay(1);   /* > 37us (指令执行时间) */
        break;

    case LCD1602_MSG_DELAY_MS:
        HAL_Delay(arg_int);
        break;

    case LCD1602_MSG_BACKLIGHT:
        /* 背光控制 (可选) */
        break;

    default:
        return 0;
    }

    return 1;
}

/* ================================================================== */
/* 初始化 —— 绑定回调 + 设置模式                                          */
/* ================================================================== */

void lcd1602_port_init(lcd1602_t *lcd)
{
    lcd->msg_cb = lcd1602_port_msg_cb;
    lcd->mode   = LCD1602_MODE_8BIT;
    lcd1602_init(lcd);
}
