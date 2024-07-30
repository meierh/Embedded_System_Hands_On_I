/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "accelerometer.h"
#include "barometer.h"
#include "factory.h"
#include "LPS331AP_LPS25H.h"
#include "LSM303D.h"
#include "uart_helper.h"
#include "FIRFilter.h"
#include "filter_data.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart5;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART5_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART5_UART_Init();
    /* USER CODE BEGIN 2 */

    barometer* myBarometer = factory::getBarometer();
    accelerometer* myAccelerometer = factory::getAccelerometer();
    magnetometer* myMagnetometer = factory::getMagnetometer();

    // create filter instance
    double NutTalWindow_fc10KHz_fs1500Hz_N20[21] = {
            0.000000000000000000004253308519220963015,
            0.000074667429161935584738113425018468661,
            0.000505633795131200805897386807430393674,
            0.000619245125354309978484013132771224264,
            -0.003454754239834230151312555179288210638,
            -0.014486040014796485592141195297699596267,
            -0.018551153749797145292577127406730141956,
            0.019681440747691934106677891236358846072,
            0.121128729896247275266141230076755164191,
            0.244109657333017204816627554464503191411,
            0.300745147355648001497030463724513538182,
            0.244109657333017260327778785722330212593,
            0.121128729896247303021716845705668674782,
            0.01968144074769194104557179514358722372,
            -0.018551153749797152231471031313958519604,
            -0.014486040014796496000482051158542162739,
            -0.003454754239834232319716900150297078653,
            0.000619245125354310629005316624073884668,
            0.000505633795131200914317604055980837074,
            0.000074667429161934026197490477105844775,
            0.000000000000000000004253308519220963015
    };
    //FIRFilter<double,20> filter_nutTal_n20_x(NutTalWindow_fc10KHz_fs1500Hz_N20);
    //FIRFilter<double,20> filter_nutTal_n20_y(NutTalWindow_fc10KHz_fs1500Hz_N20);
    //FIRFilter<double,20> filter_nutTal_n20_z(NutTalWindow_fc10KHz_fs1500Hz_N20);

    FIRFilter<float, 561> filter_nulTal_n561_x(NutTalWindow_fc10KHz_fs1500Hz_N562);
    FIRFilter<float, 561> filter_nulTal_n561_y(NutTalWindow_fc10KHz_fs1500Hz_N562);
    FIRFilter<float, 561> filter_nulTal_n561_z(NutTalWindow_fc10KHz_fs1500Hz_N562);


    while (1)
    {
        uint32_t num_measurements = uart_read_num();
        uint32_t additional_delay = uart_read_num();

        // not used in this task
        // uart_write_id("Barometer", myBarometer->readWHO_AM_I());
        // uart_write_id("Magnetometer", myMagnetometer->readWHO_AM_I());

        uart_write_id("Accelerometer", myAccelerometer->readWHO_AM_I());

        uart_write_header_filter_test();
        // not used in this task
        // uart_write_header();

        uint32_t measurement_start_ms = HAL_GetTick(); // the start of the measurements in ms

        double temperature, pressure, magnet_x, magnet_y, magnet_z, acc_x, acc_y, acc_z;
        for (uint32_t i = 0; i < num_measurements; i++)
        {
            uint32_t iteration_start = HAL_GetTick();

            double current_s = ((double) HAL_GetTick() - measurement_start_ms) / 1000; // current time since measurement_start_ms in s

            // barometer and magnetometer are not used in this task
            // myBarometer->read_barometer(temperature, pressure);
            // myMagnetometer->read_magnetometer(magnet_x, magnet_y, magnet_z);

            // get measurements
            myAccelerometer->read_accelerometer(acc_x, acc_y, acc_z);

            // filter acc values
            double acc_x_filtered = filter_nulTal_n561_x.applyFilter((float) acc_x);
            double acc_y_filtered = filter_nulTal_n561_y.applyFilter((float) acc_y);
            double acc_z_filtered = filter_nulTal_n561_z.applyFilter((float) acc_z);

            // write filtered data
            uart_write_filter_test_data(&current_s, &acc_x, &acc_y,
                                        &acc_z, &acc_x_filtered, &acc_y_filtered, &acc_z_filtered);

            // unfiltered data
            // uart_write_data(&current_s, &temperature, &pressure, &magnet_x, &magnet_y,
            //                &magnet_z, &acc_x, &acc_y, &acc_z);

            // every measurement should be 20ms long to get a even frequency
            // we wait until 20ms ware reached
            uint32_t  wait_time = 20 - (HAL_GetTick() - iteration_start);
            if(wait_time > 0) {
                HAL_Delay(wait_time);
            }

            if (additional_delay > 0)
                HAL_Delay(additional_delay);
        }
        uart_goodbye();
    }

    delete myBarometer;
    delete myAccelerometer;
    delete myMagnetometer;

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
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
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{
    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x2000090E;  //  0x2000090E 0x00100001
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
    */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
    */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
  * @brief USART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART5_UART_Init(void)
{
    /* USER CODE BEGIN USART5_Init 0 */

    /* USER CODE END USART5_Init 0 */

    /* USER CODE BEGIN USART5_Init 1 */

    /* USER CODE END USART5_Init 1 */
    huart5.Instance = USART5;
    huart5.Init.BaudRate = 115200;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart5) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART5_Init 2 */

    /* USER CODE END USART5_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    /* USER CODE BEGIN MX_GPIO_Init_1 */
    /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* USER CODE BEGIN MX_GPIO_Init_2 */
    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
