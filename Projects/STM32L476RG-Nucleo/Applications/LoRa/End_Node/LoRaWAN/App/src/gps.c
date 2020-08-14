//**************************************************************************************************
//**** Includes ************************************************************************************
#include "test_config.h"


#include "gps.h"
#include "hw.h"
#include <stdlib.h>
#include "stm32l4xx_hw_conf.h"
#include "stm32l4xx_hal_uart.h"
#include "dataset.h"
#include "lora.h"

UART_HandleTypeDef huart3;

#define UNIX_TIME_LENGTH                10
//**************************************************************************************************
//***** Local (Static) Variables *******************************************************************

uint8_t aRxBuffer[UNIX_TIME_LENGTH];
volatile uint16_t pps_count;

Chirp_Time chirp_time;
unsigned int tx_config_freq;
//**************************************************************************************************
//***** Local Functions ****************************************************************************
// unix timestamp to time string

#define UTC_BASE_YEAR   1970
#define MONTH_PER_YEAR  12
#define DAY_PER_YEAR    365
#define SEC_PER_DAY     86400
#define SEC_PER_HOUR    3600
#define SEC_PER_MIN     60

const unsigned char g_day_per_mon[MONTH_PER_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static unsigned char applib_dt_is_leap_year(unsigned short year)
{
    if ((year % 400) == 0)
        return 1;
    else if ((year % 100) == 0)
        return 0;
    else if ((year % 4) == 0)
        return 1;
    else
        return 0;
}

static unsigned char applib_dt_last_day_of_mon(unsigned char month, unsigned short year)
{
    if ((month == 0) || (month > 12))
        return g_day_per_mon[1] + applib_dt_is_leap_year(year);

    if (month != 2)
        return g_day_per_mon[month - 1];
    else
        return g_day_per_mon[1] + applib_dt_is_leap_year(year);
}


static void change_unix(long ts, Chirp_Time *gps_time)
{
    int year = 0;
    int month = 0;
    int date = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;

    int dates = ts / SEC_PER_DAY;
    int yearTmp = 0;
    int dayTmp = 0;
    for (yearTmp = UTC_BASE_YEAR; dates > 0; yearTmp++)
    {
        dayTmp = (DAY_PER_YEAR + applib_dt_is_leap_year(yearTmp));
        if (dates >= dayTmp)
            dates -= dayTmp;
        else
            break;
    }
    year = yearTmp;

    int monthTmp = 0;
    for (monthTmp = 1; monthTmp < MONTH_PER_YEAR; monthTmp++)
    {
        dayTmp = applib_dt_last_day_of_mon(monthTmp, year);
        if (dates >= dayTmp)
            dates -= dayTmp;
        else
            break;
    }
    month = monthTmp;

    date = dates + 1;

    int secs = ts % SEC_PER_DAY;
    hour = secs / SEC_PER_HOUR;
    hour += 8;
    if (hour >= 24)
    {
        hour -= 24;
        date++;
        dayTmp = applib_dt_last_day_of_mon(monthTmp, year);
        if (date > dayTmp)
        {
            date -= dayTmp;
            if (month == 12)
                yearTmp = yearTmp + 1;

            monthTmp = (monthTmp + 1) % MONTH_PER_YEAR;
        }
    }
    year = yearTmp;
    month = monthTmp;

    secs %= SEC_PER_HOUR;
    minute = secs / SEC_PER_MIN;
    second = secs % SEC_PER_MIN;

    if (monthTmp == 1 || monthTmp == 2)
    {
        monthTmp += 12;
        yearTmp--;
    }
    day =  (date + 2 * monthTmp + 3 * (monthTmp + 1) / 5 + yearTmp + yearTmp / 4 - yearTmp / 100 + yearTmp / 400) % 7 + 1;

    gps_time->chirp_year = (uint16_t)year;
    gps_time->chirp_month = (uint8_t)month;
    gps_time->chirp_date = (uint8_t)date;
    gps_time->chirp_day = (uint8_t)day;
    gps_time->chirp_hour = (uint8_t)hour;
    gps_time->chirp_min = (uint8_t)minute;
    gps_time->chirp_sec = (uint8_t)second;
    PRINTF("%d-%d-%d %d:%d:%d week: %d\n", gps_time->chirp_year, gps_time->chirp_month, gps_time->chirp_date, gps_time->chirp_hour, gps_time->chirp_min, gps_time->chirp_sec, gps_time->chirp_day);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    change_unix(strtol(aRxBuffer, NULL, 10) - 1, &chirp_time);
    __HAL_UART_DISABLE(&huart3);
}

void gps_pps_IRQ()
{
    PRINTF("pps:%d\n", pps_count);
    pps_count++;
}

//**************************************************************************************************
//***** Global Variables ***************************************************************************

//**************************************************************************************************
//***** Global Functions ***************************************************************************
void init_GPS(void)
{
    /* USER CODE END USART3_Init 1 */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }

    GPIO_InitTypeDef GPIO_InitStruct;

    /*Configure trigger */
    HAL_GPIO_WritePin(GPS_TRIGGER_Port, GPS_TRIGGER_Pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HW_GPIO_Init(GPS_TRIGGER_Port, GPS_TRIGGER_Pin, &GPIO_InitStruct );

    // we do not use pps in mixer
    /*Configure pps */
    GPIO_InitStruct.Pin = GPS_PPS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPS_PPS_Port, &GPIO_InitStruct);

    HW_GPIO_SetIrq( GPS_PPS_Port, GPS_PPS_Pin, 0, gps_pps_IRQ );
}

void read_GPS(void)
{
    __HAL_UART_DISABLE(&huart3);
    __HAL_UART_ENABLE(&huart3);
    memset(aRxBuffer, 0, sizeof(aRxBuffer));
    memset(&chirp_time, 0, sizeof(chirp_time));

    HAL_UART_Receive_IT(&huart3, (uint8_t *)aRxBuffer, sizeof(aRxBuffer));

    HAL_GPIO_WritePin(GPS_TRIGGER_Port, GPS_TRIGGER_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPS_TRIGGER_Port, GPS_TRIGGER_Pin, GPIO_PIN_RESET);
}

//**************************************************************************************************
//**************************************************************************************************

