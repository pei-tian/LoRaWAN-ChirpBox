//**************************************************************************************************
//**** Includes ************************************************************************************
#include "test_config.h"

#if GPS_DATA

#include "gps.h"
#include "hw.h"
#include <stdlib.h>
#include "stm32l4xx_hw_conf.h"
#include "stm32l4xx_hal_uart.h"
#include "dataset.h"
#include "lora.h"

UART_HandleTypeDef huart3;
extern uint8_t join_flag;

extern uint8_t test_round;

#define UNIX_TIME_LENGTH                17
//**************************************************************************************************
//***** Local (Static) Variables *******************************************************************

uint8_t aRxBuffer[UNIX_TIME_LENGTH];
int32_t now_second = 0;
RTC_TimeTypeDef stimestructure;
RTC_DateTypeDef sdatestructure;
RTC_TimeTypeDef stimestructure_temp;
uint32_t ts_ms;
volatile uint16_t pps_count;
extern uint8_t ECHO_END_FLAG;

int32_t the_given_second;

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


static uint16_t change_unix(long ts, uint16_t begin_hour, uint16_t begin_minute, uint16_t begin_second)
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;

    int days = ts / SEC_PER_DAY;
    int yearTmp = 0;
    int dayTmp = 0;
    for (yearTmp = UTC_BASE_YEAR; days > 0; yearTmp++)
    {
        dayTmp = (DAY_PER_YEAR + applib_dt_is_leap_year(yearTmp));
        if (days >= dayTmp)
            days -= dayTmp;
        else
            break;
    }
    year = yearTmp;

    int monthTmp = 0;
    for (monthTmp = 1; monthTmp < MONTH_PER_YEAR; monthTmp++)
    {
        dayTmp = applib_dt_last_day_of_mon(monthTmp, year);
        if (days >= dayTmp)
            days -= dayTmp;
        else
            break;
    }
    month = monthTmp;

    day = days + 1;

    int secs = ts % SEC_PER_DAY;
    hour = secs / SEC_PER_HOUR;
    hour += 8;
    if(hour >= 24)
    {
        hour -= 24;
        day ++;
    }
    secs %= SEC_PER_HOUR;
    minute = secs / SEC_PER_MIN;
    second = secs % SEC_PER_MIN;

    PRINTF("%d-%d-%d %d:%d:%d\n", year, month, day, hour, minute, second);
    int32_t past_hour_second = ((int32_t)hour - begin_hour) * 3600;
    int32_t past_minute_second = ((int32_t)minute - begin_minute) * 60;
    int32_t past_second_second = ((int32_t)second - begin_second);
    int32_t past_second = past_hour_second + past_minute_second + past_second_second;

    PRINTF("%lu\n", past_second);
    return past_second;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_GPIO_WritePin(GPS_TRIGGER_Port, GPS_TRIGGER_Pin, GPIO_PIN_RESET);
    ts_ms = (aRxBuffer[13]-48) * 100 + (aRxBuffer[14]-48) * 10 + aRxBuffer[15]-48;
    // PRINTF("%s\n", aRxBuffer);
    now_second = change_unix(strtol(aRxBuffer,NULL,10), GPS_BEGIN_HOUR, GPS_BEGIN_MINUTE, GPS_BEGIN_SECOND);
    // __HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE);
    __HAL_UART_DISABLE(&huart3);
}

void gps_pps_IRQ()
{
    PRINTF("pps:%d\n", pps_count);
    pps_count++;
    if((join_flag) && (pps_count >= MX_SESSION_LENGTH))
    {
        ECHO_END_FLAG = 1;
        return;
    }
    update_packet(pps_count);
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

    the_given_second = calculate_time_diff(GPS_ECHO_HOUR, GPS_ECHO_MINUTE, GPS_ECHO_SECOND,
                                    GPS_BEGIN_HOUR, GPS_BEGIN_MINUTE, GPS_BEGIN_SECOND);
}

void GPIO_Enable_IRQ(uint16_t GPIO_Pin)
{
    IRQn_Type IRQnb;

    IRQnb = MSP_GetIRQn( GPIO_Pin );

    HAL_NVIC_DisableIRQ( IRQnb );
}

void GPIO_Disable_IRQ(uint16_t GPIO_Pin)
{
    IRQn_Type IRQnb;

    IRQnb = MSP_GetIRQn( GPIO_Pin );

    HAL_NVIC_DisableIRQ( IRQnb );
}

void read_GPS(void)
{
    __HAL_UART_ENABLE(&huart3);
	HAL_UART_Receive_IT(&huart3, (uint8_t*)aRxBuffer,UNIX_TIME_LENGTH);
	HAL_GPIO_WritePin(GPS_TRIGGER_Port, GPS_TRIGGER_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPS_TRIGGER_Port, GPS_TRIGGER_Pin, GPIO_PIN_RESET);
}

//**************************************************************************************************
int32_t operation_time(void)
{
    return now_second;
}

uint32_t operation_time_ms(void)
{
    return ts_ms;
}

void clear_pps_count(void)
{
    pps_count = 0;
}

int32_t calculate_time_diff(uint8_t given_hour, uint8_t given_minute, uint8_t given_second,
                            uint8_t begin_hour, uint8_t begin_minute, uint8_t begin_second)
{
    int32_t past_hour_second = ((int32_t)given_hour - (int32_t)begin_hour) * 3600;
    int32_t past_minute_second = ((int32_t)given_minute - (int32_t)begin_minute) * 60;
    int32_t past_second_second = ((int32_t)given_second - (int32_t)begin_second);
    int32_t past_second = past_hour_second + past_minute_second + past_second_second;
    // PRINTF("%d\n", past_second);

    return past_second;
}


void wait_til_gps_time(uint8_t given_hour, uint8_t given_minute, uint8_t given_second)
{
    read_GPS();
    uint32_t delay_ms = operation_time_ms();
    DelayMs(1000 - delay_ms);

    int32_t now_second = operation_time() - 1;
    static int32_t diff_gps_second;
    diff_gps_second = now_second - the_given_second - test_round * MX_SESSION_LENGTH_INTERVAL;
    PRINTF("diff time:%d\n", diff_gps_second);

    if (diff_gps_second < 0)
    {
        // PRINTF("abs:%u\n", abs(diff_gps_second));
        pps_count = 0;
        while(((int32_t)(abs(diff_gps_second))) >= pps_count - 1);
    }
    pps_count = 0;
}

//**************************************************************************************************
//**************************************************************************************************

#endif	// GPS_DATA

