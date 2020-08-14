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

typedef struct Chirp_Time_tag
{
	uint16_t		chirp_year;
	uint8_t			chirp_month;
	uint8_t			chirp_date;
	uint8_t			chirp_day;
	uint8_t			chirp_hour;
	uint8_t			chirp_min;
	uint8_t			chirp_sec;
} Chirp_Time;

typedef struct LoRa_TX_CONFIG_tag
{
    unsigned int    tx_freq;
	uint8_t			tx_num;
	uint8_t			chirp_hour;
	uint8_t			chirp_min;
	uint8_t			chirp_sec;
} LoRa_TX_CONFIG;
// 10 bytes

extern Chirp_Time chirp_time;
extern unsigned int tx_config_freq;

void init_GPS(void);
void read_GPS(void);
#endif /* __GPS_DATA_H__ */


