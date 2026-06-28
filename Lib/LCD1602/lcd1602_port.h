/**
 * @file    lcd1602_port.h
 * @brief   LCD1602 移植层 —— 初始化声明
 *
 * 只需调用 lcd1602_port_init() 即可绑定 msg_cb 并设置接口模式。
 */

#ifndef LCD1602_PORT_H
#define LCD1602_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lcd1602.h"

/**
 * @brief  初始化 LCD1602 移植层 (绑定 msg_cb, 设置 mode)
 * @param  lcd  LCD1602 句柄
 * @note   调用后即可使用 lcd1602_init() 及其他 API
 */
void lcd1602_port_init(lcd1602_t *lcd);

#ifdef __cplusplus
}
#endif

#endif /* LCD1602_PORT_H */
