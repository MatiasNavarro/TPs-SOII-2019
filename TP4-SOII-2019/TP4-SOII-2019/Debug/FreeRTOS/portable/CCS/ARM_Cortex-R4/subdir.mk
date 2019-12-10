################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../FreeRTOS/portable/CCS/ARM_Cortex-R4/portASM.asm 

C_SRCS += \
../FreeRTOS/portable/CCS/ARM_Cortex-R4/port.c 

OBJS += \
./FreeRTOS/portable/CCS/ARM_Cortex-R4/port.o \
./FreeRTOS/portable/CCS/ARM_Cortex-R4/portASM.o 

C_DEPS += \
./FreeRTOS/portable/CCS/ARM_Cortex-R4/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/CCS/ARM_Cortex-R4/%.o: ../FreeRTOS/portable/CCS/ARM_Cortex-R4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"E:\Workspace2\SimpleDemo" -I"E:\Workspace2\CMSISv1p30_LPC17xx\inc" -O0 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS/portable/CCS/ARM_Cortex-R4/%.o: ../FreeRTOS/portable/CCS/ARM_Cortex-R4/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -DDEBUG -D__CODE_RED -D__REDLIB__ -g3 -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


