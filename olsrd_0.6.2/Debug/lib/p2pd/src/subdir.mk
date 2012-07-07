################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/p2pd/src/NetworkInterfaces.c \
../lib/p2pd/src/Packet.c \
../lib/p2pd/src/PacketHistory.c \
../lib/p2pd/src/dllist.c \
../lib/p2pd/src/olsrd_plugin.c \
../lib/p2pd/src/p2pd.c 

OBJS += \
./lib/p2pd/src/NetworkInterfaces.o \
./lib/p2pd/src/Packet.o \
./lib/p2pd/src/PacketHistory.o \
./lib/p2pd/src/dllist.o \
./lib/p2pd/src/olsrd_plugin.o \
./lib/p2pd/src/p2pd.o 

C_DEPS += \
./lib/p2pd/src/NetworkInterfaces.d \
./lib/p2pd/src/Packet.d \
./lib/p2pd/src/PacketHistory.d \
./lib/p2pd/src/dllist.d \
./lib/p2pd/src/olsrd_plugin.d \
./lib/p2pd/src/p2pd.d 


# Each subdirectory must supply rules for building sources it contributes
lib/p2pd/src/%.o: ../lib/p2pd/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


