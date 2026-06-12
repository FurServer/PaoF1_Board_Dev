/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// 头文件

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// 类型定义


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// 宏常量

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// 函数宏

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// 全局变量


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
// 函数原型

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 预处理
#include "aht20.h"
#include "median_lowpass.h"
#include "i2c_scanner.h"
#include "uart_gpio_indicator.h"
#include "printf.h"
#include "shell.h"
#include "shell_port.h"
#include "u8g2.h"
#include "u8g2_stm32.h"
#include "switch_i2c1.h"
#include "simple_key.h"

#define TX_BUFFER_SIZE 64
#define RX_BUFFER_SIZE 64
#define UART_TIMEOUT 100

#define ADC_CHANNEL_COUNT 2


// 定时器触发标志
volatile uint32_t trigger_1k = 0;
volatile uint32_t trigger_60 = 0;

// UART
int8_t tx_buffer[TX_BUFFER_SIZE];
int8_t rx_buffer[RX_BUFFER_SIZE];
int8_t rx_data;
int8_t recv_buf = 0;

// ADC
volatile uint16_t adc_dma_buffer[ADC_CHANNEL_COUNT];

// flag
volatile uint8_t flag = 0;

// count
volatile uint32_t count = 0;

// u8g2
u8g2_t u8g2;

// AHT20
int16_t temperature;
uint16_t humidity;

// key
Key_t key;

// data
int8_t data1[32] = {0};
int8_t data2[32] = {0};
int8_t data3[32] = {0};
int8_t data4[32] = {0};


// 定时器中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM2)
    {
        // 1000Hz 1ms
        trigger_1k++;
    }
    if (htim->Instance == TIM4)
    {
        // 60Hz 16.667ms
        trigger_60++;
    }
}

// UART回调
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart->Instance == USART1)
    {
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    /* 判断是哪个串口触发的中断 */
    if (huart->Instance == USART1)
    {
        //调用shell处理数据的接口
        shellHandler(&shell, recv_buf);
        //使能串口中断接收
        HAL_UART_Receive_IT(&huart1, (uint8_t*)&recv_buf, 1);
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    // 极早期初始化

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */
    // 外设初始化

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */
    // 系统初始化

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_USART3_UART_Init();
    MX_TIM1_Init();
    MX_USART1_UART_Init();
    MX_CAN_Init();
    MX_I2C1_Init();
    MX_SPI1_Init();
    MX_RTC_Init();
    MX_TIM4_Init();
    /* USER CODE BEGIN 2 */

    // 应用初始化
    Switch_I2C1(0);

    // 定时器中断
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim4);

    // UART接收中断
    HAL_UART_Receive_IT(&huart1, (uint8_t*)&recv_buf, 1);

    //ACD开启DMA
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_dma_buffer, ADC_CHANNEL_COUNT);

    // 中位值一阶低通
    MEDIAN_LP_DEFINE(adc_filter_A, 7, 0.10f);
    MEDIAN_LP_DEFINE(adc_filter_V, 7, 0.10f);

    // 按键
    Key_Init(&key, GPIOA, GPIO_PIN_0, 0, 50);

    // 初始化 shell
    userShellInit();

    u8g2Init(&u8g2);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        // 用户代码 ===================================================

        if (trigger_60 >= 1)
        {

            trigger_60 = 0;
        }

        Switch_I2C1(I2C1_ON);
        AHT20_Measure(&hi2c1);
        HAL_Delay(80);
        AHT20_Get_Data(&hi2c1,&temperature,&humidity);
        Switch_I2C1(I2C1_OFF);
        temperature = median_lowpass(&adc_filter_A,temperature);
        humidity = median_lowpass(&adc_filter_V,humidity);
        sprintf((char*)data1,"T:%02d.%02d°C",temperature/100,temperature%100);
        sprintf((char*)data2,"R:%02d.%02d%%",humidity/100,humidity%100);


        u8g2_FirstPage(&u8g2);
        do
        {
            u8g2_SetFontMode(&u8g2, 1);
            u8g2_SetFontDirection(&u8g2, 0);
            u8g2_SetFont(&u8g2, u8g2_font_t0_16_mr);
            u8g2_DrawStr(&u8g2, 0, 10, data1);
            u8g2_DrawStr(&u8g2, 0, 24, data2);

        }
        while (u8g2_NextPage(&u8g2));


        // 用户代码 ===================================================
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
// 用户函数区


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
