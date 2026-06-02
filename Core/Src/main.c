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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "tim.h"
#include "uln2003.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Delay_us(uint32_t us)
{
  uint32_t i;
  us *= 8;
  for(i = 0; i < us; i++)
  {
    __NOP();
  }
}

float HCSR04_Measure(void)
{
  uint32_t timeout = 0;
  uint32_t duration = 0;
  
  HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
  Delay_us(2);
  HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
  Delay_us(10);
  HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);
  
  timeout = 10000;
  while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET)
  {
    timeout--;
    if (timeout == 0) return 0;
    Delay_us(1);
  }
  
  timeout = 10000;
  while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_SET)
  {
    duration++;
    timeout--;
    if (timeout == 0) return 0;
    Delay_us(1);
  }
  
  return (float)duration * 0.017f;
}
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  ULN2003_Init();
  HAL_GPIO_WritePin(LAY_GPIO_Port, LAY_Pin, GPIO_PIN_RESET);
  OLED_Clear();
  OLED_ShowString(0, 0, (uint8_t *)"Distance:", 8, 1);
  OLED_ShowString(0, 8, (uint8_t *)"---.-- cm", 8, 1);
  OLED_ShowString(0, 16, (uint8_t *)"Motor: Stop   ", 8, 1);
  OLED_ShowString(0, 24, (uint8_t *)"Pump: Off    ", 8, 1);
  OLED_Refresh();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  static float last_distance = -1;
  static int last_motor_state = -1;
  static int last_pump_state = -1;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    float distance = HCSR04_Measure();
    
    int current_motor_state = (distance > 0 && distance < 5.0f) ? 1 : 0;
    int current_pump_state = (distance > 0 && distance < 5.0f) ? 1 : 0;
    
    if (distance != last_distance || current_motor_state != last_motor_state || current_pump_state != last_pump_state)
    {
      last_distance = distance;
      last_motor_state = current_motor_state;
      last_pump_state = current_pump_state;
      
      OLED_ShowString(0, 0, (uint8_t *)"Distance:", 8, 1);
      
      if (distance > 0 && distance < 400)
      {
        uint16_t int_part = (uint16_t)distance;
        uint8_t dec_part = (uint8_t)((distance - int_part) * 10);
        
        OLED_ShowString(0, 8, (uint8_t *)"          ", 8, 1);
        
        OLED_ShowNum(0, 8, int_part, 3, 8, 1);
        OLED_ShowString(18, 8, (uint8_t *)".", 8, 1);
        OLED_ShowNum(24, 8, dec_part, 1, 8, 1);
        OLED_ShowString(30, 8, (uint8_t *)"cm", 8, 1);
      }
      else
      {
        OLED_ShowString(0, 8, (uint8_t *)"---.-- cm  ", 8, 1);
      }
      
      if (current_motor_state)
      {
        OLED_ShowString(0, 16, (uint8_t *)"Motor: Running", 8, 1);
      }
      else
      {
        OLED_ShowString(0, 16, (uint8_t *)"Motor: Stop   ", 8, 1);
      }
      
      if (current_pump_state)
      {
        OLED_ShowString(0, 24, (uint8_t *)"Pump: On     ", 8, 1);
      }
      else
      {
        OLED_ShowString(0, 24, (uint8_t *)"Pump: Off    ", 8, 1);
      }
      
      OLED_Refresh();
    }
    
    if (current_pump_state)
    {
      HAL_GPIO_WritePin(LAY_GPIO_Port, LAY_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(LAY_GPIO_Port, LAY_Pin, GPIO_PIN_RESET);
    }
    
    ULN2003_Control_By_Distance(distance);
    HAL_Delay(50);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
#ifdef USE_FULL_ASSERT
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
