################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../basic_io.c \
../consoleprint.c \
../cr_startup_lpc175x_6x.c \
../heap_2.c \
../list.c \
../main.c \
../port.c \
../queue.c \
../tasks.c \
../trcKernelPort.c \
../trcSnapshotRecorder.c \
../trcStreamingRecorder.c 

OBJS += \
./basic_io.o \
./consoleprint.o \
./cr_startup_lpc175x_6x.o \
./heap_2.o \
./list.o \
./main.o \
./port.o \
./queue.o \
./tasks.o \
./trcKernelPort.o \
./trcSnapshotRecorder.o \
./trcStreamingRecorder.o 

C_DEPS += \
./basic_io.d \
./consoleprint.d \
./cr_startup_lpc175x_6x.d \
./heap_2.d \
./list.d \
./main.d \
./port.d \
./queue.d \
./tasks.d \
./trcKernelPort.d \
./trcSnapshotRecorder.d \
./trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"/home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/TP4-SOII___2019/inc/tracealyzer" -I"/home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/TP4-SOII___2019/inc/freertos" -I"/home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/TP4-SOII___2019" -I"/home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/CMSISv1p30_LPC17xx/inc" -O0 -g3 -fsigned-char -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


