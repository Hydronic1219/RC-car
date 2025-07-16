/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "usart.h"
#include "delay_us.h"
#include "ultraSonic.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint8_t rxData;
extern uint8_t rxBt;
uint8_t rxType;
uint8_t rxFlag;
uint8_t autoFlag = 0;


DriveState currentState = STOP;
TurnState turn = None;

extern uint16_t IC_Value1_L;
extern uint16_t IC_Value1_M;
extern uint16_t IC_Value1_R;
extern uint16_t IC_Value2_L;
extern uint16_t IC_Value2_M;
extern uint16_t IC_Value2_R;
extern uint16_t echoTime_L;
extern uint16_t echoTime_M;
extern uint16_t echoTime_R;
extern uint8_t captureFlag_1;
extern uint8_t captureFlag_2;
extern uint8_t captureFlag_3;
extern uint16_t distance_L;
extern uint16_t distance_M;
extern uint16_t distance_R;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		HAL_UART_Transmit(&huart2, &rxData, sizeof(rxData), 5);

		if (rxData == 'w')
		{
			currentState = FORWARD;
			turn = None;
		}
		else if (rxData == 's')
		{
			currentState = BACKWARD;
			turn = None;
		}
		else if (rxData == 'a')
		{
			if (currentState == FORWARD)
			{
				currentState = FORWARD;
				turn = Left;
			}
			else if (currentState == BACKWARD)
			{
				currentState = BACKWARD;
				turn = Left;
			}
		}
		else if (rxData == 'd')
		{
			if (currentState == FORWARD)
			{
				currentState = FORWARD;
				turn = Right;
			}
			else if (currentState == BACKWARD)
			{
				currentState = BACKWARD;
				turn = Right;
			}
		}
		else if (rxData == 'b')
		{
			currentState = STOP;
			turn = None;
		}

		HAL_UART_Receive_IT(&huart2, &rxData, sizeof(rxData));
	}
	if(huart->Instance == USART1)
	{


		if (rxBt == 'F')
		{
			HAL_UART_Transmit(&huart1, &rxBt, sizeof(rxBt), 5);
			rxFlag = 1;
			currentState = FORWARD;
			turn = None;
		}
		else if (rxBt == 'B')
		{
			HAL_UART_Transmit(&huart1, &rxBt, sizeof(rxBt), 5);
			rxFlag = 1;
			currentState = BACKWARD;
			turn = None;
		}
		else if (rxBt == 'L')
		{
			HAL_UART_Transmit(&huart1, &rxBt, sizeof(rxBt), 5);
			rxFlag = 1;
			if (currentState == FORWARD)
			{
				currentState = FORWARD;
				turn = Left;
			}
			else if (currentState == BACKWARD)
			{
				currentState = BACKWARD;
				turn = Left;
			}
		}
		else if (rxBt == 'R')
		{
			HAL_UART_Transmit(&huart1, &rxBt, sizeof(rxBt), 5);
			rxFlag = 1;
			if (currentState == FORWARD)
			{
				currentState = FORWARD;
				turn = Right;
			}
			else if (currentState == BACKWARD)
			{
				currentState = BACKWARD;
				turn = Right;
			}
		}
		else if (rxBt == 'Y')
		{
			HAL_UART_Transmit(&huart1, &rxBt, sizeof(rxBt), 5);
			rxFlag = 1;
			currentState = STOP;
			turn = None;
		}
		else if (rxBt == 'X')
		{
			HAL_UART_Transmit(&huart1, &rxBt, sizeof(rxBt), 5);
			autoFlag = 1;
			rxFlag = 0;
			currentState = STOP;
			turn = None;
		}
		else if (rxBt == 'x')
		{
			HAL_UART_Transmit(&huart1, &rxBt, sizeof(rxBt), 5);
			autoFlag = 0;
			rxFlag = 1;
			currentState = STOP;
			turn = None;
		}

		HAL_UART_Receive_IT(&huart1, &rxBt, sizeof(rxBt));
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if(captureFlag_1 == 0)  // 아직 캡처를 안했다면
		{
			IC_Value1_R = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
			captureFlag_1 = 1;    // 캡처 했음 !!!
			// 캡처에 대한 극성을 라이징에서 폴링으로 바꿈
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(captureFlag_1 == 1) // 캡처를 했다면
		{
			IC_Value2_R = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
//			__HAL_TIM_SET_COUNTER(&htim1, 0);

			if(IC_Value2_R > IC_Value1_R)
			{
				echoTime_R = IC_Value2_R - IC_Value1_R;
			}
			else if(IC_Value1_R > IC_Value2_R)
			{
				echoTime_R = (0xffff - IC_Value1_R) + IC_Value2_R;
			}
			distance_R = echoTime_R / 58;
			captureFlag_1 = 0;
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);

		}
	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		if(captureFlag_2 == 0)  // 아직 캡처를 안했다면
		{
			IC_Value1_M = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_2);
			captureFlag_2 = 1;    // 캡처 했음 !!!
			// 캡처에 대한 극성을 라이징에서 폴링으로 바꿈
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(captureFlag_2 == 1) // 캡처를 했다면
		{
			IC_Value2_M = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_2);
//			__HAL_TIM_SET_COUNTER(&htim1, 0);

			if(IC_Value2_M > IC_Value1_M)
			{
				echoTime_M = IC_Value2_M - IC_Value1_M;
			}
			else if(IC_Value1_M > IC_Value2_M)
			{
				echoTime_M = (0xffff - IC_Value1_M) + IC_Value2_M;
			}
			distance_M = echoTime_M / 58;
			captureFlag_2 = 0;
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC2);

		}

	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if(captureFlag_3 == 0)  // 아직 캡처를 안했다면
		{
			IC_Value1_L = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_3);
			captureFlag_3 = 1;    // 캡처 했음 !!!
			// 캡처에 대한 극성을 라이징에서 폴링으로 바꿈
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(captureFlag_3 == 1) // 캡처를 했다면
		{
			IC_Value2_L = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_3);
//			__HAL_TIM_SET_COUNTER(&htim1, 0);

			if(IC_Value2_L > IC_Value1_L)
			{
				echoTime_L = IC_Value2_L - IC_Value1_L;
			}
			else if(IC_Value1_L > IC_Value2_L)
			{
				echoTime_L = (0xffff - IC_Value1_L) + IC_Value2_L;
			}
			distance_L = echoTime_L / 58;
			captureFlag_3 = 0;
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC3);

		}
	}
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Motor_Task1 */
osThreadId_t Motor_Task1Handle;
const osThreadAttr_t Motor_Task1_attributes = {
  .name = "Motor_Task1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for SonicTask */
osThreadId_t SonicTaskHandle;
const osThreadAttr_t SonicTask_attributes = {
  .name = "SonicTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Motor_Task01(void *argument);
void SonicTask01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Motor_Task1 */
  Motor_Task1Handle = osThreadNew(Motor_Task01, NULL, &Motor_Task1_attributes);

  /* creation of SonicTask */
  SonicTaskHandle = osThreadNew(SonicTask01, NULL, &SonicTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Motor_Task01 */
/**
* @brief Function implementing the Motor_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Motor_Task01 */
void Motor_Task01(void *argument)
{
  /* USER CODE BEGIN Motor_Task01 */
	/* Infinite loop */

	for(;;)
	{
		if (rxFlag == 1 && autoFlag == 0)
		{
			if (currentState == FORWARD)
			{
				if (turn == None)
				{
					frontBehave();
				}
				else if (turn == Left)
				{
					front_Left();
				}
				else if (turn == Right)
				{
					front_Right();
				}
			}
			else if (currentState == BACKWARD)
			{
				if (turn == None)
				{
					backBehave();
				}
				else if (turn == Left)
				{
					back_Left();
				}
				else if (turn == Right)
				{
					back_Right();
				}
			}
			else if (currentState == STOP)
			{
				stop();
			}

			rxFlag = 0;  // 동작 후 플래그 초기화
		}
		osDelay(1);
	}
  /* USER CODE END Motor_Task01 */
}

/* USER CODE BEGIN Header_SonicTask01 */
/**
* @brief Function implementing the SonicTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SonicTask01 */
void SonicTask01(void *argument)
{
	/* USER CODE BEGIN SonicTask01 */
	/* Infinite loop */
	for(;;)
	{
		if(autoFlag == 1)
		{
            HCSR04_TRIG_ALL();
            HAL_Delay(10);
			// 자동 모드 종료 체크
			if (autoFlag == 0)
			{
				autostop();
				continue;
			}

			// 장애물 판단 로직 강화
			if ((distance_R <= 20 && distance_L <= 20) || distance_M <= 35)
			{
				autostop();

				// 더 넓은 쪽으로 회피
				if (distance_L > distance_R)
				{
					auto_Left();
				}
				else if(distance_R > distance_L)
				{
					auto_Right();
				}
			}
			else if (distance_R <= 15 && distance_L > 20)
			{
				front_Left();
			}
			else if (distance_L <= 15 && distance_R > 20)
			{
				front_Right();
			}
			else
			{
				autofrontBehave();  // 정상 전진
			}
		}

		osDelay(1);
	}
}



  /* USER CODE END SonicTask01 */


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

