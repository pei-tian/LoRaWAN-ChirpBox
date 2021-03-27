

#ifndef __TEST_CONFIG_H__
#define __TEST_CONFIG_H__

#define GPS_DATA                            1

#include <stdint.h>
/*config---------------------------------------------------------------------------*/
/*for all---------------------------------------------------------------------------*/
// config 1: payload size
#define MX_PAYLOAD_CONF_SIZE	            30
/*mixer---------------------------------------------------------------------------*/
// config 2: setup time in seconds (mixer round) and needed slot number
#define MIXER_PERIOD_TIME_S                 4
// config 3: needed slot number in setup phase
#define MX_ROUND_CONF_LENGTH                6
/*echo---------------------------------------------------------------------------*/
// config 4: one echo slot number
#define MX_SESSION_LENGTH		            200
#define MX_SESSION_LENGTH_INTERVAL          220
// config 5: transition time for time table
#define MX_TRANSITION_LENGTH		        3
/*duty cycle-------------------------------------------------------------*/
// config 6: duty cycle limit in 1/percent, should be 100 (1%) in ETSI restriction
#define MX_DUTY_CYCLE_PERCENT		        20
/*node group-------------------------------------------------------------*/
// config 7: number of groups in the network
#define GROUP_NUM                           2
// config 8: number of sensing nodes in one group
#define SENSOR_NUM                          2
// config 9: number of actuator nodes in one group
#define ACTUATOR_NUM                        1
/*new packet-------------------------------------------------------------*/
// config 10: the condition of an action is consecutive number of "1" of its group
#define CONSECUTIVE_NUM                     2

static const uint8_t NUM_ELEMENTS_nodes = (SENSOR_NUM + ACTUATOR_NUM ) * GROUP_NUM;

// calculated by script (config 11-17):
// config 11: the maximum new packet time of all the sensor nodes (calculated by script)
// config 12: the maximum new packet time of all the group (calculated by script)
// config 13: the maximum action time of all the group (calculated by script)
// config 14: new packet generated time in seconds (calculated by script)
// config 15: new packet generated index of each sensor node (calculated by script)
// config 15: new packet generated num of each sensor node (calculated by script)
// config 17: actuator time in seconds (calculated by script)
// config 18: assign the sensoring nodes and actuator nodes in groups (calculated by script)
// config 19: assign the actuator nodes in groups (calculated by script)
// config 20: each node packet is 1 (calculated by script)
#define MAX_GENERATE_LENGTH                  8
#define MAX_GENERATE_GROUP_LENGTH            13
#define MAX_ACTION_LENGTH                    3
static const uint16_t new_packets_time[] = {0, 10, 18, 28, 38, 47, 56, 66, 76, 84, 93, 102, 112, 120, 129, 138, 146, 155, 164, 172, 180};
static const uint8_t node_generate[SENSOR_NUM * GROUP_NUM][MAX_GENERATE_LENGTH] = {{2, 5, 7, 14, 15}, {3, 6, 8, 10, 11, 12, 18, 19}, {1, 4, 9, 13, 20}, {16, 17}};     
static const uint8_t node_generate_num[SENSOR_NUM * GROUP_NUM] = {5, 8, 5, 2};
static const uint16_t nominal_action_time[GROUP_NUM][MAX_ACTION_LENGTH] = {{47, 66, 138}, {120}};
static const uint8_t sensor_group[GROUP_NUM][SENSOR_NUM] = {{0, 1}, {2, 3}};
static const uint8_t actuator_group[GROUP_NUM][ACTUATOR_NUM] = {{4}, {5}};
static const uint8_t set_packet[SENSOR_NUM * GROUP_NUM][MAX_GENERATE_LENGTH] = {{5, 7, 14, 15}, {3, 6, 18}, {1, 9, 13, 20}, {16}};
/*---------------------------------------------------------------------------*/
#define MX_PACKET_TABLE_SIZE		        NUM_ELEMENTS(new_packets_time)
/*setup phase----------------------------------------------------------------*/
// config 21: setup round num
#define SETUP_MIXER_ROUND  		            1
/*multiple tests-------------------------------------------------------------*/
// config 22: test times
#define TEST_ROUND_NUM  		            1
// #define ECHO_PERIOD			                (MX_SESSION_LENGTH * ONE_SECOND_LENGTH_IN_TIM2 / 1e3)
#define ECHO_PERIOD			                (MX_SESSION_LENGTH * 1e3)

/*gps (utc time)-------------------------------------------------------------*/
// config 23: test begin and end time in UTC time
#define GPS_BEGIN_YEAR  		            2019
#define GPS_BEGIN_MONTH  		            12
#define GPS_BEGIN_DAY     		            9

#define GPS_BEGIN_HOUR     		            10
#define GPS_BEGIN_MINUTE   		            00
#define GPS_BEGIN_SECOND   		            0

#define GPS_ECHO_HOUR     		            11
#define GPS_ECHO_MINUTE   		            45
#define GPS_ECHO_SECOND   		            00

#define GPS_END_HOUR     		            15
#define GPS_END_MINUTE   		            0
#define GPS_END_SECOND   		            0

/*-------------------------------------------------------------*/
static const uint8_t payload_distribution[] = {0, 1};

#define ONE_SECOND_LENGTH_IN_TIM2	    16000000U

#define ENERGEST_CONF_ON                1

#endif /* __TEST_CONFIG_H__ */


