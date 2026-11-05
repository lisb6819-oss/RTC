#include "task_main.h"

char weeks[7][10] ={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

void MainTaskInit()
{
    HAL_Delay(20);
    OLED_Init();
    KK_RTC_Init();
}

void MainTask()
{
    OLED_NewFrame();

    struct tm *now = KK_RTC_GetTime();
    char str[50];
    sprintf(str, "%04d-%02d-%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
    OLED_PrintASCIIString(24,0,str,&afont16x8,OLED_COLOR_NORMAL);

    sprintf(str,"%02d:%02d:%02d", now->tm_hour, now->tm_min, now->tm_sec);
    OLED_PrintASCIIString(16,20,str,&afont24x12,OLED_COLOR_NORMAL);

    char* week = weeks[now->tm_wday];
    uint8_t x_week = (128-(strlen(week)*8))/2;
    OLED_PrintASCIIString(x_week,48,week,&afont16x8,OLED_COLOR_NORMAL);

    OLED_ShowFrame();


}