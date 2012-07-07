################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/pgraph/src/olsrd_pgraph.c 

OBJS += \
./lib/pgraph/src/olsrd_pgraph.o 

C_DEPS += \
./lib/pgraph/src/olsrd_pgraph.d 


# Each subdirectory must supply rules for building sources it contributes
lib/pgraph/src/%.o: ../lib/pgraph/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


