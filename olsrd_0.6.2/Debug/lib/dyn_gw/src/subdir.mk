################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/dyn_gw/src/olsrd_dyn_gw.c \
../lib/dyn_gw/src/olsrd_plugin.c 

OBJS += \
./lib/dyn_gw/src/olsrd_dyn_gw.o \
./lib/dyn_gw/src/olsrd_plugin.o 

C_DEPS += \
./lib/dyn_gw/src/olsrd_dyn_gw.d \
./lib/dyn_gw/src/olsrd_plugin.d 


# Each subdirectory must supply rules for building sources it contributes
lib/dyn_gw/src/%.o: ../lib/dyn_gw/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


