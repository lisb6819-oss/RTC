#include "task_main.h"
#include "knob.h"
#include "oled.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>

#define CURSOR_FLASH_INTERVAL 500

typedef enum {CanlendarState_Normal,CanlendarState_Setting} CanlendarState;
CanlendarState canlendarState = CanlendarState_Normal;

typedef enum {Year = 0,Month,day,Hour,Minute,Second} SettingState;
SettingState settingState = Year;

typedef struct{uint8_t x1;uint8_t y1; uint8_t x2 ;uint8_t y2;}  CursorPosition;
CursorPosition cursorpositions[6] = {
    {24+0*8,17,24+4*8,17},   //year
    {24+5*8,17,24+7*8,17},      //month
    {24+8*8,17,24+10*8,17},     //day
    {16+0*12,45,16+2*12,45},        //hour
    {16+3*12,45,16+5*12,45},        //minute
    {16+6*12,45,16+8*12,45},        //second
};

struct tm settingTime;

char weeks[7][10] ={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

void onKnobForward(){
    printf("Forward!\r\n");
}

void onKonbBackward(){
    printf("Backward!\r\n");
}
void onKnobPress(){
    if(canlendarState == CanlendarState_Normal){
        settingTime = *KK_RTC_GetTime();
        settingState = Year;
        canlendarState = CanlendarState_Setting;
    }else{
        if(settingState = Second){
            canlendarState = CanlendarState_Normal;
        }else {
            settingState++;
        }
    }
}

void showCursor(){
    static uint32_t startTime =0;
    uint32_t difftime = HAL_GetTick()-startTime;
    if(difftime>2*CURSOR_FLASH_INTERVAL){
        startTime = HAL_GetTick();
    }else if (difftime>CURSOR_FLASH_INTERVAL) {
        CursorPosition position = cursorpositions[settingState];
        OLED_DrawLine(position.x1, position.y1 ,position.x2, position.y2,OLED_COLOR_NORMAL);
    }
}

void ShowTime(struct tm* time) {
    char str[50];
    sprintf(str, "%04d-%02d-%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);
    OLED_PrintASCIIString(24,0,str,&afont16x8,OLED_COLOR_NORMAL);

    sprintf(str,"%02d:%02d:%02d", time->tm_hour, time->tm_min, time->tm_sec);
    OLED_PrintASCIIString(16,20,str,&afont24x12,OLED_COLOR_NORMAL);

    char* week = weeks[time->tm_wday];
    uint8_t x_week = (128-(strlen(week)*8))/2;
    OLED_PrintASCIIString(x_week,48,week,&afont16x8,OLED_COLOR_NORMAL);


}

void MainTaskInit()
{
    HAL_Delay(20);
    OLED_Init();
    KK_RTC_Init();
    Knob_Init();
    Knob_SetForwardCallback(onKnobForward);
    Knob_SetBackwardCallback(onKonbBackward);
    Knob_SetPressCallback(onKnobPress);
}

void MainTask()
{
    Knob_Loop();
    OLED_NewFrame();
    if(canlendarState == CanlendarState_Normal){
    struct tm *now = KK_RTC_GetTime();
    ShowTime(now);
    }else {
        ShowTime(&settingTime);
        showCursor();
    }

    OLED_ShowFrame();


}