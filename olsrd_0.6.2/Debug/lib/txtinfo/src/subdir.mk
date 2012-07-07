################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/txtinfo/src/olsrd_plugin.c \
../lib/txtinfo/src/olsrd_txtinfo.c 

OBJS += \
./lib/txtinfo/src/olsrd_plugin.o \
./lib/txtinfo/src/olsrd_txtinfo.o 

C_DEPS += \
./lib/txtinfo/src/olsrd_plugin.d \
./lib/txtinfo/src/olsrd_txtinfo.d 


# Each subdirectory must supply rules for building sources it contributes
lib/txtinfo/src/%.o: ../lib/txtinfo/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


