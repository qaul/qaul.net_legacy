################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/pud/nmealib/samples/math/main.c 

OBJS += \
./lib/pud/nmealib/samples/math/main.o 

C_DEPS += \
./lib/pud/nmealib/samples/math/main.d 


# Each subdirectory must supply rules for building sources it contributes
lib/pud/nmealib/samples/math/%.o: ../lib/pud/nmealib/samples/math/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


