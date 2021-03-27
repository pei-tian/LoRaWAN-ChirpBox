#ifndef __TIMER2_H__
#define __TIMER2_H__


//**************************************************************************************************
//***** Includes ***********************************************************************************
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32l4xx_hal.h"
#include "hw.h"

void MX_TIM2_Init(void);

void MX_LPTIM1_Init(void);
void HAL_LPTIM_Start(LPTIM_HandleTypeDef *hlptim);
uint16_t gpi_tick_fast_native(void);

uint32_t gpi_tick_hybrid_to_ms(uint32_t ticks);
uint32_t GPI_TICK_US_TO_FAST(uint32_t us);


#ifndef GPI_HYBRID_CLOCK_RATE
	// #define GPI_HYBRID_CLOCK_RATE	16000000u
	#define GPI_HYBRID_CLOCK_RATE	32768
#endif

//**************************************************************************************************

#endif /* __TIMER2_H__ */


