//**************************************************************************************************
//**** Includes ************************************************************************************
#include "test_config.h"

#include "gps.h"
#include "evaluation.h"
#include "hw.h"
#include <stdlib.h>
#include "stm32l4xx_hw_conf.h"
#include "stm32l4xx_hal_uart.h"
#include "energest.h"
#include "vcom.h"

//**************************************************************************************************
//***** Local (Static) Variables *******************************************************************

uint32_t real_action_time[MAX_ACTION_LENGTH];
uint32_t real_action_time_ms[MAX_ACTION_LENGTH];

uint32_t real_action_num;
the_evaluation evaluation;
extern uint8_t sensor_node;



uint8_t test_num;
uint32_t the_action_reliability[TEST_ROUND_NUM];
uint32_t the_action_latency[TEST_ROUND_NUM];
uint32_t the_energy[TEST_ROUND_NUM];

Sta_result test_action_reliability;
Sta_result test_action_latency;
Sta_result test_energy;

extern int32_t the_given_second;
extern uint8_t test_round;
extern uint32_t ts_ms;

//**************************************************************************************************
//***** Global Variables ***************************************************************************

extern uint8_t node_id_allocate;
extern int32_t the_given_second;

//**************************************************************************************************
//***** Global Functions ***************************************************************************

void init_action_table(void)
{
    memset(real_action_time, 0, sizeof(real_action_time));
    memset(real_action_time_ms, 0, sizeof(real_action_time_ms));
    real_action_num = 0;
}

void add_to_packet_table(void)
{
    real_action_time[real_action_num] = operation_time() - the_given_second - test_round * MX_SESSION_LENGTH_INTERVAL;
    real_action_time_ms[real_action_num] = ts_ms;
    printf("add_to_packet_table:%lu, %lu, %lu, %lu\n", real_action_time[real_action_num], operation_time(), the_given_second, test_round * MX_SESSION_LENGTH_INTERVAL);
    real_action_num ++;
}

void actuator_evaluation_results(void)
{
    uint8_t i;
    uint32_t action_latency = 0, nominal_action_num = 0, action_latency_ms;
    uint8_t group_id = (node_id_allocate -  SENSOR_NUM * GROUP_NUM + 1) / GROUP_NUM;
    for ( i = 0; i < MAX_ACTION_LENGTH; i++)
    {
        if (real_action_time[i])
        {
            action_latency += real_action_time[i] - nominal_action_time[group_id][i];
            action_latency_ms += real_action_time_ms[i];
    printf("action_latency:%lu, %lu\n", action_latency, action_latency_ms);

        }
        if (nominal_action_time[group_id][i])
            nominal_action_num ++;
        else
            break;
    }
    evaluation.result_packet.action_reliability = (uint32_t)((real_action_num * 1e4) / nominal_action_num);
    action_latency += action_latency_ms / 1000;
    evaluation.result_packet.action_latency = (uint32_t)((action_latency * 1e2) / real_action_num);
    evaluation.result_packet.energy = ((((uint32_t)(energest_type_time(ENERGEST_TYPE_LISTEN) +
                        energest_type_time(ENERGEST_TYPE_TRANSMIT))) * 1e6) / (uint32_t)GPI_TICK_US_TO_FAST(ECHO_PERIOD));

    PRINTF("action reliability:%lu\n", evaluation.result_packet.action_reliability);
    PRINTF("action latency:%lu\n", evaluation.result_packet.action_latency);
	PRINTF("action energy:%lu.%03lu\n", evaluation.result_packet.energy / 1000, evaluation.result_packet.energy % 1000);

    the_action_reliability[test_num] = evaluation.result_packet.action_reliability;
    the_action_latency[test_num] = evaluation.result_packet.action_latency;

    the_energy[test_num] = evaluation.result_packet.energy;
    test_num++;
}

void sensor_evaluation_results(void)
{
    evaluation.result_packet.energy = ((((uint32_t)(energest_type_time(ENERGEST_TYPE_LISTEN) +
                        energest_type_time(ENERGEST_TYPE_TRANSMIT))) * 1e6) / (uint32_t)GPI_TICK_US_TO_FAST(ECHO_PERIOD));
	PRINTF("sensor energy:%lu.%03lu\n", evaluation.result_packet.energy / 1000, evaluation.result_packet.energy % 1000);

    the_energy[test_num] = evaluation.result_packet.energy;
    test_num++;
}

void init_results_table(void)
{
    memset(&evaluation, 0, sizeof(the_evaluation));
}

static void statistics_array_results(uint32_t *array, uint8_t result_case, uint8_t worst_case)
{
    uint32_t i, sum = 0, worst = 0, avg = 0, Spow = 0, std = 0;
    if(!worst_case)
        worst = 0xFFFFFFFFU;

    for ( i = 0; i < TEST_ROUND_NUM; i++)
    {
        sum += array[i];
        if (!worst_case)
        {
            // the lowest is worst
            if (worst > array[i])
                worst = array[i];
        }
        else
        {
            // the highest is worst
            if (worst < array[i])
                worst = array[i];
        }
    }

    avg = sum / TEST_ROUND_NUM;
    for( i = 0 ; i < TEST_ROUND_NUM; i++)
        Spow += (array[i] - avg) * (array[i] - avg);
    std = sqrt(Spow / (TEST_ROUND_NUM - 1));

    switch (result_case)
    {
    case 1:
        {
            test_action_reliability.mean = avg;
            test_action_reliability.std = std;
            test_action_reliability.worst = worst;
        }
        break;
    case 2:
        {
            test_action_latency.mean = avg;
            test_action_latency.std = std;
            test_action_latency.worst = worst;
        }
        break;
    case 3:
        {
            test_energy.mean = avg;
            test_energy.std = std;
            test_energy.worst = worst;
        }
        break;
    default:
        break;
    }
}

void statistics_results(void)
{
    if(!sensor_node)
    {
        statistics_array_results(the_action_reliability, 1, 0);
        statistics_array_results(the_action_latency, 2, 1);
    }
    statistics_array_results(the_energy, 3, 1);

    PRINTF("all test results:-----------\n");
    if(!sensor_node)
    {
        PRINTF("action_reliability:%lu.%02lu%%, %lu.%02lu%%, %lu.%02lu%%\n", test_action_reliability.mean / 100, test_action_reliability.mean % 100,
                                            test_action_reliability.std / 100, test_action_reliability.std % 100,
                                            test_action_reliability.worst / 100, test_action_reliability.worst % 100);
        PRINTF("action_latency (slot/second):%lu.%02lu, %lu.%02lu, %lu.%02lu\n", test_action_latency.mean / 100, test_action_latency.mean % 100,
                                            test_action_latency.std / 100, test_action_latency.std % 100,
                                            test_action_latency.worst / 100, test_action_latency.worst % 100);
    }
	PRINTF("energy:%lu.%03lu, %lu.%03lu, %lu.%03lu\n", test_energy.mean / 1000, test_energy.mean % 1000,
                                            test_energy.std / 1000, test_energy.std % 1000,
                                            test_energy.worst / 1000, test_energy.worst % 1000);
}
