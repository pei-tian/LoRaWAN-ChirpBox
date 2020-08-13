//**************************************************************************************************
//**** Includes ************************************************************************************
#include "test_config.h"

#include "dataset.h"
#include "lora.h"

#include "gps.h"
#include "hw.h"
#include <stdlib.h>
#include "stm32l4xx_hw_conf.h"
#include "stm32l4xx_hal_uart.h"

//**************************************************************************************************
//***** Local (Static) Variables *******************************************************************
uint16_t			beat = 0;
uint16_t			set_beat = 0;
//**************************************************************************************************
//***** Global Variables ***************************************************************************
uint8_t join_flag;
extern uint8_t sensor_node;

extern uint8_t set_one_flag;
extern LoraFlagStatus AppGPSRequest;
extern LoraFlagStatus AppGPSRequest;
extern uint8_t node_id_allocate;

//**************************************************************************************************
//***** Global Functions ***************************************************************************
void update_packet(uint16_t pps_count)
{
    if((join_flag) && (sensor_node))
    {
        uint16_t now_time = pps_count;
        if ((now_time == new_packets_time[node_generate[node_id_allocate][beat]]) && (beat < node_generate_num[node_id_allocate]))
        {
            PRINTF("update:%d, %d\n", now_time, beat);
            AppGPSRequest = LORA_SET;
            beat++;
            if(set_packet[node_id_allocate][set_beat] == node_generate[node_id_allocate][beat - 1])
            {
                set_one_flag = 1;
                set_beat ++;
            }
            else
            {
                set_one_flag = 0;
            }
        }
    }
}

void clear_dataset(void)
{
    beat = 0;
    set_beat = 0;
}

//**************************************************************************************************
//**************************************************************************************************


