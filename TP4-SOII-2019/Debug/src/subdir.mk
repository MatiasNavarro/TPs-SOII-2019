################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/heap_2.c \
../src/list.c \
../src/main.c \
../src/port.c \
../src/queue.c \
../src/tasks.c \
../src/trcKernelPort.c \
../src/trcSnapshotRecorder.c \
../src/trcStreamingRecorder.c 

OBJS += \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/heap_2.o \
./src/list.o \
./src/main.o \
./src/port.o \
./src/queue.o \
./src/tasks.o \
./src/trcKernelPort.o \
./src/trcSnapshotRecorder.o \
./src/trcStreamingRecorder.o 

C_DEPS += \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/heap_2.d \
./src/list.d \
./src/main.d \
./src/port.d \
./src/queue.d \
./src/tasks.d \
./src/trcKernelPort.d \
./src/trcSnapshotRecorder.d \
./src/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__LPC17XX__ -D__REDLIB__ -I"/home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/TP4-SOII-2019/inc" -I"/home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/CMSISv1p30_LPC17xx/inc" -I"/home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/TP4-SOII-2019/inc/FreeRTOS" -I"/home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/TP4-SOII-2019/inc/Tracealycer" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


