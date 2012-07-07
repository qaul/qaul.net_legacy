################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/quagga/src/client.c \
../lib/quagga/src/olsrd_plugin.c \
../lib/quagga/src/packet.c \
../lib/quagga/src/parse.c \
../lib/quagga/src/plugin.c \
../lib/quagga/src/quagga.c 

OBJS += \
./lib/quagga/src/client.o \
./lib/quagga/src/olsrd_plugin.o \
./lib/quagga/src/packet.o \
./lib/quagga/src/parse.o \
./lib/quagga/src/plugin.o \
./lib/quagga/src/quagga.o 

C_DEPS += \
./lib/quagga/src/client.d \
./lib/quagga/src/olsrd_plugin.d \
./lib/quagga/src/packet.d \
./lib/quagga/src/parse.d \
./lib/quagga/src/plugin.d \
./lib/quagga/src/quagga.d 


# Each subdirectory must supply rules for building sources it contributes
lib/quagga/src/%.o: ../lib/quagga/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


