#ifndef __GPS_DATA_H__
#define __GPS_DATA_H__


// //**************************************************************************************************
// //***** Includes ***********************************************************************************
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32l4xx_hal.h"
#include "vcom.h"
// //**************************************************************************************************
void init_GPS(void);
void GPIO_Enable_IRQ(uint16_t GPIO_Pin);
void GPIO_Disable_IRQ(uint16_t GPIO_Pin);
void read_GPS(void);
int32_t operation_time(void);
uint32_t operation_time_ms(void);
void clear_pps_count(void);
int32_t calculate_time_diff(uint8_t given_hour, uint8_t given_minute, uint8_t given_second,
                            uint8_t begin_hour, uint8_t begin_minute, uint8_t begin_second);
void wait_til_gps_time(uint8_t given_hour, uint8_t given_minute, uint8_t given_second);

#endif /* __GPS_DATA_H__ */


