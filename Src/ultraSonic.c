
#include "ultraSonic.h"


uint16_t IC_Value1_L = 0;
uint16_t IC_Value1_M = 0;
uint16_t IC_Value1_R = 0;
uint16_t IC_Value2_L = 0;
uint16_t IC_Value2_M = 0;
uint16_t IC_Value2_R = 0;
uint16_t echoTime_L = 0;
uint16_t echoTime_M = 0;
uint16_t echoTime_R = 0;
uint8_t captureFlag_1 = 0;
uint8_t captureFlag_2 = 0;
uint8_t captureFlag_3 = 0;
uint16_t distance_L = 0;
uint16_t distance_M = 0;
uint16_t distance_R = 0;



void HCSR04_TRIG_L(void)
{
  HAL_GPIO_WritePin(TRIG_PORT_L, TRIG_PIN_L, GPIO_PIN_RESET);
  delay_us(1);
  HAL_GPIO_WritePin(TRIG_PORT_L, TRIG_PIN_L, GPIO_PIN_SET);
  delay_us(10);
  HAL_GPIO_WritePin(TRIG_PORT_L, TRIG_PIN_L, GPIO_PIN_RESET);

  __HAL_TIM_SET_COUNTER(&htim1, 0);
  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC3);
  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC2);
  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
}

//
//void HCSR04_TRIG_M(void)
//{
//  HAL_GPIO_WritePin(TRIG_PORT_M, TRIG_PIN_M, GPIO_PIN_RESET);
//  delay_us(1);
//  HAL_GPIO_WritePin(TRIG_PORT_M, TRIG_PIN_M, GPIO_PIN_SET);
//  delay_us(10);
//  HAL_GPIO_WritePin(TRIG_PORT_M, TRIG_PIN_M, GPIO_PIN_RESET);
//
//  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC2);
//}
//
//
//void HCSR04_TRIG_R(void)
//{
//  HAL_GPIO_WritePin(TRIG_PORT_R, TRIG_PIN_R, GPIO_PIN_RESET);
//  delay_us(1);
//  HAL_GPIO_WritePin(TRIG_PORT_R, TRIG_PIN_R, GPIO_PIN_SET);
//  delay_us(10);
//  HAL_GPIO_WritePin(TRIG_PORT_R, TRIG_PIN_R, GPIO_PIN_RESET);
//
//  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
//}
