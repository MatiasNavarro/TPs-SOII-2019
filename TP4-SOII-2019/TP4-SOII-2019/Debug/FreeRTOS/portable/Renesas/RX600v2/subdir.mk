################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/Renesas/RX600v2/port.c 

OBJS += \
./FreeRTOS/portable/Renesas/RX600v2/port.o 

C_DEPS += \
./FreeRTOS/portable/Renesas/RX600v2/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/Renesas/RX600v2/%.o: ../FreeRTOS/portable/Renesas/RX600v2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"E:\Workspace2\SimpleDemo" -I"E:\Workspace2\CMSISv1p30_LPC17xx\inc" -O0 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


