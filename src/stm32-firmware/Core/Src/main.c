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
  *8
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

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

/* USER CODE BEGIN PV */
static uint8_t led_on = 0x00;

#define LR_COUNT_START 16
static uint16_t current_lr_counter = 0;
static uint8_t DMA_Started = 0x00;

#define SEND_BUFF_SZ 8
static uint16_t send_buff[SEND_BUFF_SZ] = {0};
static uint16_t rcv_buff[SEND_BUFF_SZ] = {0};
static uint16_t adc_buff[SEND_BUFF_SZ] = {0};

// #define TRANSMIT_TIMEOUT 1000
#define TRANSMIT_TIMEOUT 1200
static uint16_t tm_timeout = 0;


static uint8_t adc_dma_started = 0x00;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static inline void i2s3_full_stop(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = PCM0_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // GPIO_InitStruct.Alternate = ;
    HAL_GPIO_Init(PCM0_CLK_GPIO_Port, &GPIO_InitStruct);
}

// static inline void i2s2_full_stop(void){
//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     GPIO_InitStruct.Pin = I2S1_LRCK_Pin|I2S1_BCK_Pin|I2S1_DATA_OUT_Pin;
//     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     // GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
// }
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

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_I2C1_Init();
  MX_I2S1_Init();
  // MX_I2S2_Init();
  // MX_I2S3_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(LED_PIN_GPIO_Port, LED_PIN_Pin, GPIO_PIN_SET);
  

  /* USER CODE END 2 */
  

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */    
      tm_timeout = DMA_Started ? tm_timeout+1 : 0;
      if(tm_timeout >= TRANSMIT_TIMEOUT){
        i2s3_full_stop();
        HAL_I2S_DeInit(&hi2s3);
        // HAL_Delay(5);
        MX_I2S3_Init();
        DMA_Started = 0x0;
        tm_timeout = 0;
      }

      if(DMA_Started && !adc_dma_started){        
        adc_dma_started = 0x1;
      }

      if(!DMA_Started && adc_dma_started){        
        HAL_I2S_DeInit(&hi2s2);
        // HAL_Delay(5);
        MX_I2S2_Init();
        // memset((uint8_t*)&adc_buff[0], 0x00, sizeof(adc_buff[0])*SEND_BUFF_SZ);

        if(led_on){
          HAL_GPIO_WritePin(LED_PIN_GPIO_Port, LED_PIN_Pin, GPIO_PIN_SET);
          led_on = 0x0;
        }

        adc_dma_started = 0x0;
      }

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 13;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_TIM|RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 358;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 26;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 7;
  PeriphClkInitStruct.TIMPresSelection = RCC_TIMPRES_ACTIVATED;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* RCC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RCC_IRQn, 2, 3);
  HAL_NVIC_EnableIRQ(RCC_IRQn);
  /* FPU_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(FPU_IRQn, 3, 1);
  HAL_NVIC_EnableIRQ(FPU_IRQn);
  /* FLASH_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(FLASH_IRQn, 2, 1);
  HAL_NVIC_EnableIRQ(FLASH_IRQn);
  /* PVD_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PVD_IRQn, 1, 3);
  HAL_NVIC_EnableIRQ(PVD_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* I2C1_ER_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  /* I2C1_EV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  /* SPI1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SPI1_IRQn, 3, 2);
  HAL_NVIC_EnableIRQ(SPI1_IRQn);
  /* SPI2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SPI2_IRQn, 3, 2);
  HAL_NVIC_EnableIRQ(SPI2_IRQn);
  /* SPI3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SPI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SPI3_IRQn);
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case GPIO_PIN_1:
    {    
      if((!DMA_Started) && (current_lr_counter >= LR_COUNT_START)){
        HAL_I2SEx_TransmitReceive_DMA(&hi2s3, (uint16_t*)&send_buff[0], rcv_buff, SEND_BUFF_SZ/2);
        HAL_I2S_Receive_DMA(&hi2s2, (uint16_t*)&adc_buff[0], SEND_BUFF_SZ/2);

        DMA_Started = 0x1;
        current_lr_counter = 0;

        if(!led_on){
          HAL_GPIO_WritePin(LED_PIN_GPIO_Port, LED_PIN_Pin, GPIO_PIN_RESET);
          led_on = 0x1;
        }

      }

      if(DMA_Started){
        tm_timeout = 0;
        break;
      }

      current_lr_counter++;
      break;
    }
    case GPIO_PIN_0:
    {
      led_on = (led_on ? 0 : 1 );
      HAL_GPIO_WritePin(LED_PIN_GPIO_Port, LED_PIN_Pin, led_on ?  GPIO_PIN_RESET : GPIO_PIN_SET);
      __NOP();
    }
    default:{
      __NOP();
      break;
    }
  }
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef* hi2s){
  if(hi2s == &hi2s2){
    memcpy((uint8_t*)&send_buff[0], (uint8_t*)&adc_buff[0], sizeof(send_buff[0])*SEND_BUFF_SZ/2);
  }
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef* hi2s){
  if(hi2s == &hi2s2){
    memcpy((uint8_t*)&send_buff[SEND_BUFF_SZ/2], (uint8_t*)&adc_buff[SEND_BUFF_SZ/2], sizeof(send_buff[0])*SEND_BUFF_SZ/2);
  }
}

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
