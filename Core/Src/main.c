
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "main.h"
#include "gpio.h"
#include "dht11.h"
#include "lcd.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

typedef enum{
	MODE_SHOWDATA = 0,
	MODE_CONFIG_TEMP = 1,
	MODE_CONFIG_HUMI = 2
}Mode_t;

Mode_t CURRENT_MODE = MODE_SHOWDATA;

typedef struct{
	uint8_t i_humi_config;
	uint8_t i_temp_config;
}DHT11_DataConfig_t;
DHT11_DataConfig_t dataConfig;
DHT11_Data_t dht11_data;
char msg[32];
char msg1[32];
void GPIO_BTN_IT(void){
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//GPIO LED
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
}

void GPIO_Alert(){
	if(CURRENT_MODE == MODE_SHOWDATA){
		if(dht11_data.i_humi > dataConfig.i_humi_config
				|| dht11_data.i_temp > dataConfig.i_temp_config){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		}else{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		}
	}

}

void LCD_Display_Config(void){
	LCD_Clear();
	sprintf(msg1, "Temp on Led %d'C", dataConfig.i_temp_config);
	LCD_Send_CMD(0x80); // Dòng 1
	LCD_Send_String((uint8_t*)msg1);
	sprintf(msg1, "Humi on Led %d%c", dataConfig.i_humi_config, '%');
	LCD_Send_CMD(0xC0); // Dòng 2
	LCD_Send_String((uint8_t*)msg1);
	if(CURRENT_MODE == MODE_CONFIG_TEMP){
		LCD_Send_CMD(0x8C);
	}else if(CURRENT_MODE == MODE_CONFIG_HUMI){
		LCD_Send_CMD(0xCC);
	}

}

void LCD_Display_Data(void){
	GPIO_Alert();
	LCD_Clear();
	sprintf(msg, "Temp: %d'C", dht11_data.i_temp);
	LCD_Send_CMD(0x80); // Dòng 1
	LCD_Send_String((uint8_t*)msg);
	sprintf(msg, "Humi: %d%c", dht11_data.i_humi, '%');
	LCD_Send_CMD(0xC0); // Dòng 2
	LCD_Send_String((uint8_t*)msg);
}

extern void initialise_monitor_handles(void);
static Mode_t last_mode = -1;
int main(void)
{
	initialise_monitor_handles();
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  GPIO_BTN_IT();
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  DHT11_Init(GPIOD, GPIO_PIN_1);
  LCD_Init();

  while (1)
  {
	if(last_mode != CURRENT_MODE || CURRENT_MODE == MODE_SHOWDATA){
		last_mode = CURRENT_MODE;
		if(CURRENT_MODE == MODE_SHOWDATA){
			if(DHT11_ReadData(&dht11_data)){
				if(dataConfig.i_humi_config == 0 && dataConfig.i_temp_config == 0){
					dataConfig.i_humi_config = dht11_data.i_humi + 2;
					dataConfig.i_temp_config = dht11_data.i_temp + 2;
				}
				LCD_Display_Data();
				HAL_Delay(1000);
			}else{
				sprintf(msg, "Sensor Error");
				LCD_Clear();
				LCD_Send_CMD(0x80);
				LCD_Send_String((uint8_t*)msg);
				continue;
			}
		}else if(CURRENT_MODE == MODE_CONFIG_TEMP || CURRENT_MODE == MODE_CONFIG_HUMI){
			LCD_Display_Config();
		}
	}
    /* USER CODE END WHILE */
	/* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}




void EXTI3_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if(GPIO_Pin == GPIO_PIN_3){
		delay_us(20000);
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET){
			if(CURRENT_MODE == MODE_SHOWDATA){
				CURRENT_MODE = MODE_CONFIG_TEMP;
				LCD_Display_Config();
			}else if(CURRENT_MODE == MODE_CONFIG_TEMP){
				CURRENT_MODE = MODE_CONFIG_HUMI;
				LCD_Display_Config();
			}else if(CURRENT_MODE == MODE_CONFIG_HUMI){
				CURRENT_MODE = MODE_SHOWDATA;
				LCD_Display_Data();
			}
		}
	}else if(GPIO_Pin == GPIO_PIN_4){
		//		increase
		delay_us(20000);
		if(CURRENT_MODE == MODE_CONFIG_TEMP && dataConfig.i_temp_config < 100){
			dataConfig.i_temp_config++;
			LCD_Display_Config();
		}else if(CURRENT_MODE == MODE_CONFIG_HUMI && dataConfig.i_humi_config < 100){
			dataConfig.i_humi_config++;
			LCD_Display_Config();
		}

	}else if(GPIO_Pin == GPIO_PIN_5){
		//		decrease
		delay_us(20000);
		if(CURRENT_MODE == MODE_CONFIG_TEMP && dataConfig.i_temp_config > 0 ){
			dataConfig.i_temp_config--;
			LCD_Display_Config();
		}else if(CURRENT_MODE == MODE_CONFIG_HUMI && dataConfig.i_humi_config > 0){
			dataConfig.i_humi_config--;
			LCD_Display_Config();
		}
	}
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
