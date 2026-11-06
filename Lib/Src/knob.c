#include "knob.h"
#include "tim.h"
#include <stddef.h>

typedef enum {Pressed,Unpressed} Btnstate;

#define BTN_DEBOUNCE_TICKS 10
#define COUNTER_INIT_VALUE 65535/2

uint32_t getTick(){
    return HAL_GetTick();
}
KnobCallback onForwardCallback = NULL;
KnobCallback onBackwardCallback = NULL;
KnobCallback onPressCallback = NULL;

void Knob_SetForwardCallback(KnobCallback callback){
    onForwardCallback = callback;
}
void Knob_SetBackwardCallback(KnobCallback callback){
    onBackwardCallback = callback;
}
void Knob_SetPressCallback(KnobCallback callback){
    onPressCallback = callback;
}

Btnstate getBtnstate(){
    return HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin) == GPIO_PIN_RESET ? Pressed : Unpressed;
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

    Btnstate btnstate = getBtnstate();
    static uint8_t callbackstate = 0;
    static uint32_t Presstime = 0;
    if(btnstate == Pressed)
    {
        if(Presstime == 0){
            Presstime = getTick();
        }else if(callbackstate == 0 && getTick()-Presstime > BTN_DEBOUNCE_TICKS){
            if(onPressCallback != NULL){
                onPressCallback();

            }
            callbackstate = 1;
        }
    }else{
        Presstime = 0;
        callbackstate = 0;
    }
}
