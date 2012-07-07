################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/dyn_gw_plain/src/olsrd_dyn_gw_plain.c 

OBJS += \
./lib/dyn_gw_plain/src/olsrd_dyn_gw_plain.o 

C_DEPS += \
./lib/dyn_gw_plain/src/olsrd_dyn_gw_plain.d 


# Each subdirectory must supply rules for building sources it contributes
lib/dyn_gw_plain/src/%.o: ../lib/dyn_gw_plain/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


