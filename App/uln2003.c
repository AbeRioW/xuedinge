#include "uln2003.h"

static uint8_t step_index = 0;

static const uint8_t step_sequence[8] = {
    0x01, 
    0x03, 
    0x02, 
    0x06, 
    0x04, 
    0x0C, 
    0x08, 
    0x09
};

static void Set_Step(uint8_t step)
{
    if (step & 0x01) ULN2003_IN1_Set(); else ULN2003_IN1_Clr();
    if (step & 0x02) ULN2003_IN2_Set(); else ULN2003_IN2_Clr();
    if (step & 0x04) ULN2003_IN3_Set(); else ULN2003_IN3_Clr();
    if (step & 0x08) ULN2003_IN4_Set(); else ULN2003_IN4_Clr();
}

void ULN2003_Init(void)
{
    ULN2003_Stop();
}

void ULN2003_Step_CW(uint16_t steps, uint16_t delay_ms)
{
    for (uint16_t i = 0; i < steps; i++)
    {
        Set_Step(step_sequence[step_index]);
        step_index = (step_index + 1) % 8;
        HAL_Delay(delay_ms);
    }
}

void ULN2003_Step_CCW(uint16_t steps, uint16_t delay_ms)
{
    for (uint16_t i = 0; i < steps; i++)
    {
        Set_Step(step_sequence[step_index]);
        if (step_index == 0)
        {
            step_index = 7;
        }
        else
        {
            step_index--;
        }
        HAL_Delay(delay_ms);
    }
}

void ULN2003_Stop(void)
{
    ULN2003_IN1_Clr();
    ULN2003_IN2_Clr();
    ULN2003_IN3_Clr();
    ULN2003_IN4_Clr();
}

void ULN2003_Control_By_Distance(float distance)
{
    if (distance > 0 && distance < 5.0f)
    {
        ULN2003_Step_CW(8, 2);
    }
    else
    {
        ULN2003_Stop();
    }
}

