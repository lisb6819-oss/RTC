#include "knob.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"

#define COUNTER_INIT_VALUE 65535/2
KnobCallback onForwardCallback = NULL;
KnobCallback onBackwardCallback = NULL;

void Knob_SetForwardCallback(KnobCallback callback){
    onForwardCallback = callback;
}
void Knob_SetBackwardCallback(KnobCallback callback){
    onBackwardCallback = callback;
}

void Knob_SetCounter(uint16_t value){
    __HAL_TIM_SET_COUNTER(&htim1, value);
}

uint32_t Knob_GetCounter(){
    return __HAL_TIM_GET_COUNTER(&htim1);
}

void Knob_Init(){
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    Knob_SetCounter(COUNTER_INIT_VALUE);
}

void Knob_Loop(){
    uint32_t counter = Knob_GetCounter();
    if(counter > COUNTER_INIT_VALUE){
        if(onForwardCallback != NULL){
            onForwardCallback();
        }
    }else if(counter < COUNTER_INIT_VALUE){
        if(onBackwardCallback != NULL){
            onBackwardCallback();
        }
    }
    Knob_SetCounter(COUNTER_INIT_VALUE);

}