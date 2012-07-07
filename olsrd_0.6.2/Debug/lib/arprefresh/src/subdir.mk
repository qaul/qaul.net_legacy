################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/arprefresh/src/olsrd_arprefresh.c 

OBJS += \
./lib/arprefresh/src/olsrd_arprefresh.o 

C_DEPS += \
./lib/arprefresh/src/olsrd_arprefresh.d 


# Each subdirectory must supply rules for building sources it contributes
lib/arprefresh/src/%.o: ../lib/arprefresh/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


