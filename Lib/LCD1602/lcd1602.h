/**
 * @file    lcd1602.h
 * @brief   LCD1602 (HD44780) 可移植驱动 —— 纯逻辑层 (u8g2 消息回调风格)
 *
 * @note    本文件不包含任何硬件相关头文件。
 *          移植时只需实现一个 lcd1602_msg_cb 回调函数，
 *          在其中 switch-case 处理各种消息即可。
 *
 *          参考 u8g2 架构：
 *            - lcd1602.h / lcd1602.c  = 纯逻辑 (类似 u8g2 核心)
 *            - lcd1602_port.h / .c   = 硬件适配 (类似 u8g2_stm32.c)
 *
 *          消息粒度设计为最原子的硬件操作（SET_RS / WRITE_DATA / PULSE_EN），
 *          逻辑层自行组合 4-bit / 8-bit 差异，port 层无需关心模式。
 */

#ifndef LCD1602_H
#define LCD1602_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ================================================================== */
/* 消息类型 —— 原子硬件操作                                               */
/* ================================================================== */

/** @brief 硬件初始化 (port 层可在此初始化 GPIO) */
#define LCD1602_MSG_INIT          0

/**
 * @brief  设置 RS (寄存器选择) 线
 * @param  arg_int = 0 (指令) / 1 (数据)
 */
#define LCD1602_MSG_SET_RS        1

/**
 * @brief  写数据到数据线 (D0~D7 或 D4~D7, 取决于硬件连接)
 * @param  arg_int = 数据值 (8-bit 模式: 完整字节; 4-bit 模式: 高 nibble 或低 nibble)
 * @note   不产生 E 脉冲，由调用方在适当时机发 LCD1602_MSG_PULSE_EN
 */
#define LCD1602_MSG_WRITE_DATA    2

/**
 * @brief  产生 E 脉冲 (高→低, HD44780 在下降沿锁存数据)
 */
#define LCD1602_MSG_PULSE_EN      3

/**
 * @brief  毫秒延时
 * @param  arg_int = 延时毫秒数 (≤255)
 */
#define LCD1602_MSG_DELAY_MS      4

/**
 * @brief  背光控制 (可选)
 * @param  arg_int = 0 关闭 / 1 开启
 */
#define LCD1602_MSG_BACKLIGHT     5

/* ================================================================== */
/* 前向声明                                                              */
/* ================================================================== */

typedef struct lcd1602_t lcd1602_t;

/* ================================================================== */
/* 回调函数类型 —— 类似 u8x8_msg_cb                                      */
/* ================================================================== */

/**
 * @brief  消息回调
 * @param  lcd      LCD1602 句柄
 * @param  msg      消息类型 (LCD1602_MSG_xxx)
 * @param  arg_int  消息参数
 * @param  arg_ptr  消息参数指针
 * @retval  1=成功, 0=失败
 */
typedef uint8_t (*lcd1602_msg_cb)(lcd1602_t *lcd, uint8_t msg, uint8_t arg_int, void *arg_ptr);

/* ================================================================== */
/* 接口模式 (告诉 HD44780 数据线宽度)                                      */
/* ================================================================== */

typedef enum {
    LCD1602_MODE_4BIT = 0,
    LCD1602_MODE_8BIT = 1
} lcd1602_mode_t;

/* ================================================================== */
/* LCD1602 句柄 —— 1 个回调 + 逻辑状态, 无硬件字段                        */
/* ================================================================== */

struct lcd1602_t {
    lcd1602_msg_cb  msg_cb;         /**< 消息回调 (port 层实现)        */
    lcd1602_mode_t  mode;           /**< 4-bit 或 8-bit 接口模式      */
    uint8_t         display_ctrl;   /**< Display ON/OFF/Cursor/Blink  */
    uint8_t         entry_mode;     /**< Entry Mode 缓存              */
};

/* ================================================================== */
/* 公开 API —— 纯逻辑, 只通过 msg_cb 与硬件交互                            */
/* ================================================================== */

void lcd1602_init(lcd1602_t *lcd);
void lcd1602_clear(lcd1602_t *lcd);
void lcd1602_home(lcd1602_t *lcd);
void lcd1602_set_cursor(lcd1602_t *lcd, uint8_t col, uint8_t row);

void lcd1602_display_on(lcd1602_t *lcd);
void lcd1602_display_off(lcd1602_t *lcd);
void lcd1602_cursor_on(lcd1602_t *lcd);
void lcd1602_cursor_off(lcd1602_t *lcd);
void lcd1602_blink_on(lcd1602_t *lcd);
void lcd1602_blink_off(lcd1602_t *lcd);

void lcd1602_put_char(lcd1602_t *lcd, char ch);
void lcd1602_print(lcd1602_t *lcd, const char *str);

void lcd1602_create_char(lcd1602_t *lcd, uint8_t idx, const uint8_t pattern[8]);

void lcd1602_shift_left(lcd1602_t *lcd);
void lcd1602_shift_right(lcd1602_t *lcd);

void lcd1602_backlight_on(lcd1602_t *lcd);
void lcd1602_backlight_off(lcd1602_t *lcd);

#ifdef __cplusplus
}
#endif

#endif /* LCD1602_H */
