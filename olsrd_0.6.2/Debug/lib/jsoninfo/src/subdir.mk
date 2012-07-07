################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/jsoninfo/src/olsrd_jsoninfo.c \
../lib/jsoninfo/src/olsrd_plugin.c 

OBJS += \
./lib/jsoninfo/src/olsrd_jsoninfo.o \
./lib/jsoninfo/src/olsrd_plugin.o 

C_DEPS += \
./lib/jsoninfo/src/olsrd_jsoninfo.d \
./lib/jsoninfo/src/olsrd_plugin.d 


# Each subdirectory must supply rules for building sources it contributes
lib/jsoninfo/src/%.o: ../lib/jsoninfo/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


