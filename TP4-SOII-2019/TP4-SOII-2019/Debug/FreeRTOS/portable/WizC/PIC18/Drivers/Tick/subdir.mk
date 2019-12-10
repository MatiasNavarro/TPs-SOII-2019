################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/WizC/PIC18/Drivers/Tick/Tick.c \
../FreeRTOS/portable/WizC/PIC18/Drivers/Tick/isrTick.c 

OBJS += \
./FreeRTOS/portable/WizC/PIC18/Drivers/Tick/Tick.o \
./FreeRTOS/portable/WizC/PIC18/Drivers/Tick/isrTick.o 

C_DEPS += \
./FreeRTOS/portable/WizC/PIC18/Drivers/Tick/Tick.d \
./FreeRTOS/portable/WizC/PIC18/Drivers/Tick/isrTick.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/WizC/PIC18/Drivers/Tick/%.o: ../FreeRTOS/portable/WizC/PIC18/Drivers/Tick/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"E:\Workspace2\SimpleDemo" -I"E:\Workspace2\CMSISv1p30_LPC17xx\inc" -O0 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


