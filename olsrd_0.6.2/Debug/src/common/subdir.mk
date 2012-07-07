################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/common/autobuf.o \
../src/common/avl.o \
../src/common/list.o 

C_SRCS += \
../src/common/autobuf.c \
../src/common/avl.c \
../src/common/list.c 

OBJS += \
./src/common/autobuf.o \
./src/common/avl.o \
./src/common/list.o 

C_DEPS += \
./src/common/autobuf.d \
./src/common/avl.d \
./src/common/list.d 


# Each subdirectory must supply rules for building sources it contributes
src/common/%.o: ../src/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


