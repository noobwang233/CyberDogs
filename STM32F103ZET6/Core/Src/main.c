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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "font.h"
#include "bmp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define move_delay 150 //
#define move_delay_slow 300 //
#define move_speed_fast 2 //
#define move_speed 4 //
#define move_speed_slow 7 //
#define rest_time 30000U
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t move_mode = '0';
uint8_t previous_mode = '0';
uint8_t flag_tick = 1;
uint32_t time_record = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Rbt_Init(void);
void Rbt_Init_Slow(void);
uint16_t angle_to_CCR(uint8_t angle);
uint8_t CCR_to_angle(uint16_t CCRR);
uint16_t angle3_to_CCR(uint8_t angle);
uint8_t CCR_to_angle3(uint16_t CCRR);
//uint32_t time_record_now = 0;
void move_forward(void);
void move_behind(void);
void move_right(void);
void move_left(void);
void move_swing(void);
void move_stretch(void);
void move_test(void);
void move_sleep_w(void);
void move_sleep_p(void);
void move_two_hands(void);
void lan_yao(void);
void two_legs_down_in(void);
void two_legs_down_out(void);
void butt_up(void);
void move_random();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	//重新设置中断

	if (move_mode == 'f') {
		//OLED_Clear();
		OLED_DrawBMP(0, 0, 128, 8, BMP2); //前进
	} else if (move_mode == 'b') {
		OLED_DrawBMP(0, 0, 128, 8, BMP2); //后退
	} else if (move_mode == 'l') {
		OLED_DrawBMP(0, 0, 128, 8, BMP4); //左转
	} else if (move_mode == 'r') {
		OLED_DrawBMP(0, 0, 128, 8, BMP3); //右转
	} else if (move_mode == 'w') {
		OLED_DrawBMP(0, 0, 128, 8, BMP_very_happy); //摇摆
	}
	time_record = HAL_GetTick();
	flag_tick = 1;
	HAL_UART_Receive_IT(&huart1, &move_mode, 1);
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
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
    OLED_Init();
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); //前腿1
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); //前腿2
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_UART_Receive_IT(&huart1, &move_mode, 1); //
	OLED_DrawBMP(0, 0, 128, 8, BMP1); //立正
    uint8_t i=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {

        if (i>=180) {
            i=0;
        }
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, angle_to_CCR(i));
		HAL_Delay(10000);
        i++;
		//HAL_Delay(1);
		// time_record_now = HAL_GetTick() + 1000;
		if (move_mode == 'f') { //前进
			move_forward();
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
		} else if (move_mode == 'b') { //后退
			move_behind();
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
		} else if (move_mode == 'l') { //左转
			move_left();
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
		} else if (move_mode == 'r') { //右转
			move_right();
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
		} else if (move_mode == 'w') { //摇摆
			move_swing();
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
		} else if (move_mode == '5') { //立正
			OLED_DrawBMP(0, 0, 128, 8, BMP1);
			Rbt_Init();
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
			move_mode = '0';
		} else if (move_mode == 'q' && previous_mode != '0') { //起身
			Rbt_Init_Slow();
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
			move_mode = '0';
		} else if (move_mode == 's' && previous_mode != 's') { //坐下
			Rbt_Init_Slow();
			OLED_DrawBMP(0, 0, 128, 8, BMP2);
			move_stretch();
			OLED_DrawBMP(0, 0, 128, 8, BMP_miao);
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
			move_mode = '0';
		} else if (move_mode == 'j') { //交替抬手
			Rbt_Init();  // 表情
			move_two_hands();
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
			move_mode = '0';
		} else if (move_mode == 'y') { //伸懒腰
			Rbt_Init_Slow();  // 表情
			HAL_Delay(move_delay_slow);
			OLED_DrawBMP(0, 0, 128, 8, BMP_happy);
			lan_yao();
			OLED_DrawBMP(0, 0, 128, 8, BMP1);
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
			move_mode = '0';
		} else if (move_mode == '1') { //抬头
			Rbt_Init_Slow();  // 表情
			HAL_Delay(move_delay);
			two_legs_down_in();
			OLED_DrawBMP(0, 0, 128, 8, BMP_tiao_pi);
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
			move_mode = '0';
		} else if (move_mode == '9') { //撅腚
			Rbt_Init_Slow();  // 表情
			HAL_Delay(move_delay);
			OLED_DrawBMP(0, 0, 128, 8, BMP_mihu);
			butt_up();
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
			previous_mode = move_mode;
			move_mode = '0';
		} else if (move_mode == 'p' && previous_mode != 'p') { //趴下睡觉
			if (previous_mode != '5' && previous_mode != 'q') {
				Rbt_Init_Slow();
				HAL_Delay(move_delay_slow);
			}
			move_sleep_p();
			OLED_DrawBMP(0, 0, 128, 8, BMP_sleep_p);
			previous_mode = move_mode;
			move_mode = '0';
		} else if (move_mode == '2' && previous_mode != '2') { //卧下睡觉
			if (previous_mode != '5' && previous_mode != 'q') {
				Rbt_Init_Slow();
				HAL_Delay(move_delay_slow);
			}
			move_sleep_w();
			OLED_DrawBMP(0, 0, 128, 8, BMP_sleep_w);
			previous_mode = move_mode;
			move_mode = '0';
		} else if ((HAL_GetTick() + 1000) - time_record > rest_time) {
			if(flag_tick == 1){
				HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
				OLED_NewFrame();
				OLED_ShowFrame();
				flag_tick = 0;
			}

		}

	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
uint16_t angle_to_CCR(uint8_t angle) {
	return angle * 2000 / 180 + 500;
}

uint8_t CCR_to_angle(uint16_t CCRR) {
	return (CCRR - 500) * 180 / 2000;
}

uint16_t angle3_to_CCR(uint8_t angle) {
	return angle * 2000 / 193 + 500;
}

uint8_t CCR_to_angle3(uint16_t CCRR) {
	return (CCRR - 500) * 193 / 2000;
}

void Rbt_Init(void) {
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90));
	HAL_Delay(100);
}

void Rbt_Init_Slow(void) {
	if (previous_mode == '0')
		return;
	OLED_DrawBMP(0, 0, 128, 8, BMP1);
	uint8_t angle_1, angle_2, angle_3, angle_4;
	angle_1 = CCR_to_angle(TIM2->CCR1);
	angle_3 = CCR_to_angle3(TIM2->CCR3);
	angle_2 = CCR_to_angle(TIM2->CCR2);
	angle_4 = CCR_to_angle(TIM2->CCR4);
	while (angle_1 != 90 || angle_3 != 90 || angle_2 != 90 || angle_4 != 90) {
		if (angle_1 > 90) {
			--angle_1;
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, angle_to_CCR(angle_1));
		} else if (angle_1 < 90) {
			++angle_1;
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, angle_to_CCR(angle_1));
		}
		//
		if (angle_3 > 90) {
			--angle_3;
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,
					angle3_to_CCR(angle_3));
		} else if (angle_3 < 90) {
			++angle_3;
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3,
					angle3_to_CCR(angle_3));
		}
		//
		if (angle_2 > 90) {
			--angle_2;
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, angle_to_CCR(angle_2));
		} else if (angle_2 < 90) {
			++angle_2;
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, angle_to_CCR(angle_2));
		}
		//
		if (angle_4 > 90) {
			--angle_4;
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, angle_to_CCR(angle_4));
		} else if (angle_4 < 90) {
			++angle_4;
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, angle_to_CCR(angle_4));
		}
		HAL_Delay(move_speed_slow);
	}

}

void move_forward(void) {
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90));

	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90));
	HAL_Delay(move_delay);
}

void move_behind(void) {
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90));

	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90));
	HAL_Delay(move_delay);
}

void move_right(void) {
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90));
	HAL_Delay(move_delay);
}

void move_left(void) {
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90));
	HAL_Delay(move_delay);
}

void move_swing(void) {
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(45));
	HAL_Delay(220);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(135));
	HAL_Delay(220);
}
void move_stretch(void) {
	if (TIM2->CCR1 < angle_to_CCR(155) && TIM2->CCR3 > angle3_to_CCR(25)) {
		for (uint8_t i = 0; i < 70; i++) {
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90+i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90-i));
			HAL_Delay(move_speed);
		}
		HAL_Delay(1000);
		for (uint8_t i = 0; i < 70; i++) {
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(160-i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(20+i));
			HAL_Delay(move_speed);
		}
		for (uint8_t i = 0; i < 65; i++) {
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90+i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90-i));
			HAL_Delay(move_speed);
		}
		for (uint8_t i = 0; i < 20; i++) {
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90-i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90+i));
			HAL_Delay(move_speed);
		}
		HAL_Delay(1000);
		for (uint8_t i = 1; i <= 60; i++) {
					__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(70+i));
					HAL_Delay(move_speed);
				}
		// __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(135)); // 右前腿 right front leg
		HAL_Delay(1000);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(180));
		HAL_Delay(500);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(130));
		HAL_Delay(500);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(180));
		HAL_Delay(500);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(130));
		HAL_Delay(500);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(70));
	}
}

void move_sleep_w(void) {

	for (uint8_t i = 0; i < 75; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90-i)); // 15   右前腿 right front leg
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90+i)); // 165  左前腿 right front leg
		HAL_Delay(move_speed);
	}

	for (uint8_t i = 0; i < 75; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90+i)); // 165 右后腿 right front leg
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90-i)); // 15 左后腿 right front leg
		HAL_Delay(move_speed);
	}

}

void move_sleep_p(void) {
	for (uint8_t i = 0; i < 75; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90+i)); // 165 右前腿 right front leg
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90-i)); // 15 左前腿 right front leg
		HAL_Delay(move_speed);
	}
	for (uint8_t i = 0; i < 75; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90-i)); // 15
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90+i)); // 165
		HAL_Delay(move_speed);
	}
}

void move_two_hands(void) {

	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(20));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle3_to_CCR(20));
	HAL_Delay(move_delay);

	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle3_to_CCR(90));
	HAL_Delay(move_delay);

	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(160));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(160));

	HAL_Delay(move_delay);
	for (uint8_t i = 1; i <= 90; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90));
	}
	HAL_Delay(move_delay);
//	for (uint8_t i = 0; i < 90; i++) {
//		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90-i));
//		HAL_Delay(move_speed_fast / 2);
//	}
//	HAL_Delay(move_delay);
//	for (uint8_t i = 1; i <= 90; i++) {
//		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(i));
//		HAL_Delay(move_speed_fast / 2);
//	}
//	HAL_Delay(move_delay);
//	for (uint8_t i = 0; i < 90; i++) {
//		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90+i));
//		HAL_Delay(move_speed_fast / 2);
//	}
//	HAL_Delay(move_delay);
//	for (uint8_t i = 1; i <= 90; i++) {
//		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(180-i));
//		HAL_Delay(move_speed_fast / 2);
//	}
//	HAL_Delay(move_delay);

}

void butt_up(void) { //撅腚
	for (uint8_t i = 0; i < 75; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90+i));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90-i));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90+i/3));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90-i/3));
		HAL_Delay(move_speed);
	}
}

void two_legs_down_in(void) { // 抬头 内
	for (uint8_t i = 0; i < 20; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90-i));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90+i));
		HAL_Delay(move_speed);
	}
	for (uint8_t i = 0; i < 65; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90+i));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90-i));
		HAL_Delay(move_speed);
	}
}

void lan_yao(void) {
	for (uint8_t i = 0; i < 75; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(90+i));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(90-i));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(90+i/2));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(90-i/2));
		HAL_Delay(move_speed + 1);
	}

	HAL_Delay(move_delay * 10);
	for (uint8_t i = 1; i < 75; i++) {
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle_to_CCR(165-i));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle3_to_CCR(15+i));
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle_to_CCR(127-i/2)); // 15
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle_to_CCR(53+i/2)); // 165
		HAL_Delay(move_speed);
	}
	HAL_Delay(move_delay);

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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
