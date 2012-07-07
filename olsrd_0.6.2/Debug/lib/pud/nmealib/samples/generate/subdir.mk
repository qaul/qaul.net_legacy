################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/pud/nmealib/samples/generate/main.c 

OBJS += \
./lib/pud/nmealib/samples/generate/main.o 

C_DEPS += \
./lib/pud/nmealib/samples/generate/main.d 


# Each subdirectory must supply rules for building sources it contributes
lib/pud/nmealib/samples/generate/%.o: ../lib/pud/nmealib/samples/generate/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


