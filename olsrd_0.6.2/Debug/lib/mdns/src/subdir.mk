################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/mdns/src/Address.c \
../lib/mdns/src/NetworkInterfaces.c \
../lib/mdns/src/Packet.c \
../lib/mdns/src/mdns.c \
../lib/mdns/src/olsrd_plugin.c 

OBJS += \
./lib/mdns/src/Address.o \
./lib/mdns/src/NetworkInterfaces.o \
./lib/mdns/src/Packet.o \
./lib/mdns/src/mdns.o \
./lib/mdns/src/olsrd_plugin.o 

C_DEPS += \
./lib/mdns/src/Address.d \
./lib/mdns/src/NetworkInterfaces.d \
./lib/mdns/src/Packet.d \
./lib/mdns/src/mdns.d \
./lib/mdns/src/olsrd_plugin.d 


# Each subdirectory must supply rules for building sources it contributes
lib/mdns/src/%.o: ../lib/mdns/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


