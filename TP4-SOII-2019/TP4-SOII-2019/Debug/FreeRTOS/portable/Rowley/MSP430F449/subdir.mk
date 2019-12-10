################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../FreeRTOS/portable/Rowley/MSP430F449/portext.asm 

C_SRCS += \
../FreeRTOS/portable/Rowley/MSP430F449/port.c 

OBJS += \
./FreeRTOS/portable/Rowley/MSP430F449/port.o \
./FreeRTOS/portable/Rowley/MSP430F449/portext.o 

C_DEPS += \
./FreeRTOS/portable/Rowley/MSP430F449/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/Rowley/MSP430F449/%.o: ../FreeRTOS/portable/Rowley/MSP430F449/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"E:\Workspace2\SimpleDemo" -I"E:\Workspace2\CMSISv1p30_LPC17xx\inc" -O0 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

FreeRTOS/portable/Rowley/MSP430F449/%.o: ../FreeRTOS/portable/Rowley/MSP430F449/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -DDEBUG -D__CODE_RED -D__REDLIB__ -g3 -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


