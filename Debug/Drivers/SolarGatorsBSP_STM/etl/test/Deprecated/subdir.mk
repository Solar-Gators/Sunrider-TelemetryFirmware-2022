################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_embedded_compile.cpp \
../Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_factory.cpp 

OBJS += \
./Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_embedded_compile.o \
./Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_factory.o 

CPP_DEPS += \
./Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_embedded_compile.d \
./Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_factory.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/%.o: ../Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/%.cpp Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -DETL_CALLBACK_TIMER_USE_ATOMIC_LOCK -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"C:/Users/georg/STM32CubeIDE/SolarGators/OpenTelemetry/Drivers/SolarGatorsBSP_STM/DataModules/inc" -I"C:/Users/georg/STM32CubeIDE/SolarGators/OpenTelemetry/Drivers/SolarGatorsBSP_STM/Drivers/inc" -I"C:/Users/georg/STM32CubeIDE/SolarGators/OpenTelemetry/Drivers/SolarGatorsBSP_STM/etl/include" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-SolarGatorsBSP_STM-2f-etl-2f-test-2f-Deprecated

clean-Drivers-2f-SolarGatorsBSP_STM-2f-etl-2f-test-2f-Deprecated:
	-$(RM) ./Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_embedded_compile.d ./Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_embedded_compile.o ./Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_factory.d ./Drivers/SolarGatorsBSP_STM/etl/test/Deprecated/test_factory.o

.PHONY: clean-Drivers-2f-SolarGatorsBSP_STM-2f-etl-2f-test-2f-Deprecated

