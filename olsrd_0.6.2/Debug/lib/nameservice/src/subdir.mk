################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/nameservice/src/compat.c \
../lib/nameservice/src/mapwrite.c \
../lib/nameservice/src/nameservice.c \
../lib/nameservice/src/olsrd_plugin.c 

OBJS += \
./lib/nameservice/src/compat.o \
./lib/nameservice/src/mapwrite.o \
./lib/nameservice/src/nameservice.o \
./lib/nameservice/src/olsrd_plugin.o 

C_DEPS += \
./lib/nameservice/src/compat.d \
./lib/nameservice/src/mapwrite.d \
./lib/nameservice/src/nameservice.d \
./lib/nameservice/src/olsrd_plugin.d 


# Each subdirectory must supply rules for building sources it contributes
lib/nameservice/src/%.o: ../lib/nameservice/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


