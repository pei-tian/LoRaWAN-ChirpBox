#include "hw.h"
#include "timer2.h"


TIM_HandleTypeDef htim2;
LPTIM_HandleTypeDef hlptim1;

void MX_TIM2_Init(void)
{

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 4;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFFFFFF;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
    Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
    Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
    Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */

}

void MX_LPTIM1_Init(void)
{
    hlptim1.Instance = LPTIM1;
    hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
    hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
    hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
    hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
    hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
    hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
    hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
    hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
    if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
    {
    Error_Handler();
    }
}

void HAL_LPTIM_Start(LPTIM_HandleTypeDef *hlptim)
{
    hlptim->State= HAL_LPTIM_STATE_BUSY;

    hlptim->Instance->CFGR &= ~LPTIM_CFGR_WAVE;

    __HAL_LPTIM_ENABLE(hlptim);

    __HAL_LPTIM_AUTORELOAD_SET(hlptim, 65535);

    __HAL_LPTIM_START_CONTINUOUS(hlptim);

    hlptim->State= HAL_LPTIM_STATE_READY;
}

uint16_t gpi_tick_fast_native(void)
{
	// return htim2.Instance->CNT;
	return hlptim1.Instance->CNT;
    // hlptim->Instance->CNT
}

uint32_t gpi_tick_hybrid_to_ms(uint32_t ticks)
{
    return (((ticks * 1000) / GPI_HYBRID_CLOCK_RATE));
}

uint32_t GPI_TICK_US_TO_FAST(uint32_t ms)
{
    return ms * GPI_HYBRID_CLOCK_RATE / 1000;
}
