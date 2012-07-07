################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bsd/apm.c \
../src/bsd/dummy.c \
../src/bsd/kernel_routes.c \
../src/bsd/net.c 

OBJS += \
./src/bsd/apm.o \
./src/bsd/dummy.o \
./src/bsd/kernel_routes.o \
./src/bsd/net.o 

C_DEPS += \
./src/bsd/apm.d \
./src/bsd/dummy.d \
./src/bsd/kernel_routes.d \
./src/bsd/net.d 


# Each subdirectory must supply rules for building sources it contributes
src/bsd/%.o: ../src/bsd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


