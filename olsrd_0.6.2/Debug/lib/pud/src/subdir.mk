################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/pud/src/configuration.c \
../lib/pud/src/dedup.c \
../lib/pud/src/gpsConversion.c \
../lib/pud/src/netTools.c \
../lib/pud/src/networkInterfaces.c \
../lib/pud/src/posAvg.c \
../lib/pud/src/posFile.c \
../lib/pud/src/pud.c \
../lib/pud/src/pudOlsrdPlugin.c \
../lib/pud/src/receiver.c \
../lib/pud/src/state.c \
../lib/pud/src/timers.c \
../lib/pud/src/uplinkGateway.c 

OBJS += \
./lib/pud/src/configuration.o \
./lib/pud/src/dedup.o \
./lib/pud/src/gpsConversion.o \
./lib/pud/src/netTools.o \
./lib/pud/src/networkInterfaces.o \
./lib/pud/src/posAvg.o \
./lib/pud/src/posFile.o \
./lib/pud/src/pud.o \
./lib/pud/src/pudOlsrdPlugin.o \
./lib/pud/src/receiver.o \
./lib/pud/src/state.o \
./lib/pud/src/timers.o \
./lib/pud/src/uplinkGateway.o 

C_DEPS += \
./lib/pud/src/configuration.d \
./lib/pud/src/dedup.d \
./lib/pud/src/gpsConversion.d \
./lib/pud/src/netTools.d \
./lib/pud/src/networkInterfaces.d \
./lib/pud/src/posAvg.d \
./lib/pud/src/posFile.d \
./lib/pud/src/pud.d \
./lib/pud/src/pudOlsrdPlugin.d \
./lib/pud/src/receiver.d \
./lib/pud/src/state.d \
./lib/pud/src/timers.d \
./lib/pud/src/uplinkGateway.d 


# Each subdirectory must supply rules for building sources it contributes
lib/pud/src/%.o: ../lib/pud/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


