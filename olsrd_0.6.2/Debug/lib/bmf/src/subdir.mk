################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/bmf/src/Address.c \
../lib/bmf/src/Bmf.c \
../lib/bmf/src/NetworkInterfaces.c \
../lib/bmf/src/Packet.c \
../lib/bmf/src/PacketHistory.c \
../lib/bmf/src/olsrd_plugin.c 

OBJS += \
./lib/bmf/src/Address.o \
./lib/bmf/src/Bmf.o \
./lib/bmf/src/NetworkInterfaces.o \
./lib/bmf/src/Packet.o \
./lib/bmf/src/PacketHistory.o \
./lib/bmf/src/olsrd_plugin.o 

C_DEPS += \
./lib/bmf/src/Address.d \
./lib/bmf/src/Bmf.d \
./lib/bmf/src/NetworkInterfaces.d \
./lib/bmf/src/Packet.d \
./lib/bmf/src/PacketHistory.d \
./lib/bmf/src/olsrd_plugin.d 


# Each subdirectory must supply rules for building sources it contributes
lib/bmf/src/%.o: ../lib/bmf/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


