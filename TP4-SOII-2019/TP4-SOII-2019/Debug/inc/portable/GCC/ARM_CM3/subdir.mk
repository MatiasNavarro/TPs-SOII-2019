################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../inc/portable/GCC/ARM_CM3/port.c 

OBJS += \
./inc/portable/GCC/ARM_CM3/port.o 

C_DEPS += \
./inc/portable/GCC/ARM_CM3/port.d 


# Each subdirectory must supply rules for building sources it contributes
inc/portable/GCC/ARM_CM3/%.o: ../inc/portable/GCC/ARM_CM3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"E:\Workspace2\SimpleDemo\inc\include_trace" -I"E:\Workspace2\SimpleDemo\inc\include_freertos" -I"E:\Workspace2\SimpleDemo\inc\portable\GCC\ARM_CM3" -I"E:\Workspace2\SimpleDemo" -I"E:\Workspace2\CMSISv1p30_LPC17xx\inc" -O0 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


