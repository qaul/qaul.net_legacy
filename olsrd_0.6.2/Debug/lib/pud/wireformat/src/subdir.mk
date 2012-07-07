################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/pud/wireformat/src/nodeIdConversion.c \
../lib/pud/wireformat/src/wireFormat.c 

OBJS += \
./lib/pud/wireformat/src/nodeIdConversion.o \
./lib/pud/wireformat/src/wireFormat.o 

C_DEPS += \
./lib/pud/wireformat/src/nodeIdConversion.d \
./lib/pud/wireformat/src/wireFormat.d 


# Each subdirectory must supply rules for building sources it contributes
lib/pud/wireformat/src/%.o: ../lib/pud/wireformat/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


