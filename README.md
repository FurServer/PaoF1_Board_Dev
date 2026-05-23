芝士泡泡用来学STM32而创建的仓库
---
PaoF1开发板是泡泡自己画的一块开发板<br>
基于STM32F103C8T6<br>
但是由于画的太答辩了所以只产出了一块呜呜呜

板载设备:
```text
通讯:
 Debuger DapLink
 WiFi ESP-12F
 GPS E108-GN03
 Bluetooth KT6368A
 NRG24L01 E01-ML01SP2
传感:
 Current INA226
 Temperature AHT20
 Barometer BMP280
 3-Axis-Magnetic QMC5883
 IMU/Gyroscopic BMI270
显示:
 RGB-LED WS2812B
 OLED12864 SSD1315
 LCD1602/LCD12864 (仅接口)
模拟:
 Current INA199
 Voltage LMV321
其他:
 FLASH 25Qxx
 Motor drive DRV8870
 Serial2Parallel PCF8574A (给LCD用)
 LED/BEEP/Rotary-Encoder
```

引脚配置:
```text
PA0 --> Rotary-Encoder KEY
PA1 --> Rotary-Encoder A
PA2 --> Rotary-Encoder B
PA3 --> LED (推>绿 挽>蓝)
PA4 --> INA199 (Analog current 2A/V)
PA5 --> LMV321 (Analog voltage 0.2V/V)
PA6 --> 74HC138-A (SPI使能)
PA7 --> 74HC138-B
PB0 --> 74HC138-C (发现C用不到)
PB1 --> BEEP
PB2 --> 74HC138-G1 (高启用)
PB10 -> UART3-TX (E108-GN03 and ESP-12F)
PB11 -> UART3-RX     GPS^        WiFi^
PB12 -> LCD1602/LCD12864 RW
PB13 -> LCD1602/LCD12864 E
PB14 -> DRV8870 Reversal (PWM)
PB15 -> DRV8870 Forward (PWM)
PA8 --> WS2812B *24
PA9 --> UART3-TX (DapLink KT6368A)
PA10 -> UART3-RX  Debuger   BT
PA11 -> USB-DM/CAN-RX
PA12 -> USB-DP/CAN-TX
PA13 -> SWDIO
PA14 -> SWCLK
PA15 -> LCD/OLED DC(RS)
PB3 --> SPI1-SCK
PB4 --> SPI1-MISO
PB5 --> SPI1-MOSI
PB6 --> NRF24L01-CE
PB7 --> NRF24L01-IRQ
PB8 --> I2C1-SCL
PB9 --> I2C1-SDA
PC13 -> BMI270-INT
PC14 -> OSC32
PC15 -> OSC32
```
其他信息:
```text
I2C devices Addr
0x2C QMC5883
0x38 AHT20
0x3F PCF8574
0x40 INA226
0x76 BMP280

SPI devices CS
Y0 BMI270
Y1 NRF24L01
Y2 SSD1315
Y3 25Qxxx
```


~~呜呜呜第一次用git不规范补药骂了~~