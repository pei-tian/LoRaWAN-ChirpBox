#ifndef __EVALUATION_H__
#define __EVALUATION_H__


// //**************************************************************************************************
// //***** Includes ***********************************************************************************
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32l4xx_hal.h"
#include "energest.h"
//**************************************************************************************************
typedef struct Result_packet_tag
{
	uint32_t			action_reliability;
	uint32_t			action_latency;
	uint32_t			energy;
} Result_packet;

typedef struct evaluation
{
	Result_packet				result_packet;
} the_evaluation;

typedef struct Sta_result_tag
{
	uint32_t			mean;
	uint32_t			std;
	uint32_t			worst;
} Sta_result;

//**************************************************************************************************
void init_action_table(void);
void add_to_packet_table(void);
void actuator_evaluation_results(void);
void sensor_evaluation_results(void);
void init_results_table(void);
void statistics_results(void);
#endif /* __EVALUATION_H__ */



