################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/TP/Study/LoRaWAN-TP/STM32CubeExpansion_LRWAN_V1.3.1/Middlewares/Third_Party/LoRaWAN/Utilities/utilities.c 

OBJS += \
./Middlewares/LoRaWAN/Utilities/utilities.o 

C_DEPS += \
./Middlewares/LoRaWAN/Utilities/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/LoRaWAN/Utilities/utilities.o: D:/TP/Study/LoRaWAN-TP/STM32CubeExpansion_LRWAN_V1.3.1/Middlewares/Third_Party/LoRaWAN/Utilities/utilities.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSX1276MB1MAS -c -I../../../LoRaWAN/App -I../../../LoRaWAN/Target -I../../../Core/Inc -I../../../../../../../../Utilities/misc -I../../../../../../../../Utilities/timer -I../../../../../../../../Utilities/trace/adv_trace -I../../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../../Utilities/sequencer -I../../../../../../../../Drivers/BSP/STM32L4xx_Nucleo -I../../../../../../../../Drivers/BSP/sx1276mb1mas -I../../../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Middlewares/Third_Party/SubGHz_Phy -I../../../../../../../../Middlewares/Third_Party/SubGHz_Phy/sx1276 -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac/Region -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/LmHandler/packages -I../../../../../../../../Drivers/BSP/IKS01A2 -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../../../Drivers/BSP/Components/lps22hb -I../../../../../../../../Drivers/BSP/Components/lsm6dsl -I../../../../../../../../Drivers/BSP/Components/lsm303agr -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/LoRaWAN/Utilities/utilities.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

