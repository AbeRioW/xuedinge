#ifndef __ULN2003_H
#define __ULN2003_H

#include "stdint.h"
#include "gpio.h"
#include "main.h"

#define ULN2003_IN1_Clr()  HAL_GPIO_WritePin(ULN2003_IN1_GPIO_Port, ULN2003_IN1_Pin, GPIO_PIN_RESET)
#define ULN2003_IN1_Set()  HAL_GPIO_WritePin(ULN2003_IN1_GPIO_Port, ULN2003_IN1_Pin, GPIO_PIN_SET)

#define ULN2003_IN2_Clr()  HAL_GPIO_WritePin(ULN2003_IN2_GPIO_Port, ULN2003_IN2_Pin, GPIO_PIN_RESET)
#define ULN2003_IN2_Set()  HAL_GPIO_WritePin(ULN2003_IN2_GPIO_Port, ULN2003_IN2_Pin, GPIO_PIN_SET)

#define ULN2003_IN3_Clr()  HAL_GPIO_WritePin(ULN2003_IN3_GPIO_Port, ULN2003_IN3_Pin, GPIO_PIN_RESET)
#define ULN2003_IN3_Set()  HAL_GPIO_WritePin(ULN2003_IN3_GPIO_Port, ULN2003_IN3_Pin, GPIO_PIN_SET)

#define ULN2003_IN4_Clr()  HAL_GPIO_WritePin(ULN2003_IN4_GPIO_Port, ULN2003_IN4_Pin, GPIO_PIN_RESET)
#define ULN2003_IN4_Set()  HAL_GPIO_WritePin(ULN2003_IN4_GPIO_Port, ULN2003_IN4_Pin, GPIO_PIN_SET)

void ULN2003_Init(void);
void ULN2003_Step_CW(uint16_t steps, uint16_t delay_ms);
void ULN2003_Step_CCW(uint16_t steps, uint16_t delay_ms);
void ULN2003_Stop(void);
void ULN2003_Control_By_Distance(float distance);

#endif

