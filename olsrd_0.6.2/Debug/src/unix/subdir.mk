################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/unix/ifnet.o \
../src/unix/log.o \
../src/unix/misc.o 

C_SRCS += \
../src/unix/ifnet.c \
../src/unix/log.c \
../src/unix/misc.c 

OBJS += \
./src/unix/ifnet.o \
./src/unix/log.o \
./src/unix/misc.o 

C_DEPS += \
./src/unix/ifnet.d \
./src/unix/log.d \
./src/unix/misc.d 


# Each subdirectory must supply rules for building sources it contributes
src/unix/%.o: ../src/unix/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


