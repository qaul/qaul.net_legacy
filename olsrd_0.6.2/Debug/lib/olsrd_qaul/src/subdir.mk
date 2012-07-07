################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../lib/olsrd_qaul/src/olsrd_plugin.o \
../lib/olsrd_qaul/src/qaul_chat.o \
../lib/olsrd_qaul/src/qaul_ipc.o \
../lib/olsrd_qaul/src/qaul_net.o 

C_SRCS += \
../lib/olsrd_qaul/src/olsrd_plugin.c \
../lib/olsrd_qaul/src/qaul_chat.c \
../lib/olsrd_qaul/src/qaul_ipc.c \
../lib/olsrd_qaul/src/qaul_net.c 

OBJS += \
./lib/olsrd_qaul/src/olsrd_plugin.o \
./lib/olsrd_qaul/src/qaul_chat.o \
./lib/olsrd_qaul/src/qaul_ipc.o \
./lib/olsrd_qaul/src/qaul_net.o 

C_DEPS += \
./lib/olsrd_qaul/src/olsrd_plugin.d \
./lib/olsrd_qaul/src/qaul_chat.d \
./lib/olsrd_qaul/src/qaul_ipc.d \
./lib/olsrd_qaul/src/qaul_net.d 


# Each subdirectory must supply rules for building sources it contributes
lib/olsrd_qaul/src/%.o: ../lib/olsrd_qaul/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


