################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gui/win32/Shim/shim.c 

OBJS += \
./gui/win32/Shim/shim.o 

C_DEPS += \
./gui/win32/Shim/shim.d 


# Each subdirectory must supply rules for building sources it contributes
gui/win32/Shim/%.o: ../gui/win32/Shim/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


