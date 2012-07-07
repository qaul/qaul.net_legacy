################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/linux/apm.o \
../src/linux/kernel_routes_nl.o \
../src/linux/kernel_tunnel.o \
../src/linux/link_layer.o \
../src/linux/net.o 

C_SRCS += \
../src/linux/apm.c \
../src/linux/kernel_routes_nl.c \
../src/linux/kernel_tunnel.c \
../src/linux/link_layer.c \
../src/linux/net.c 

OBJS += \
./src/linux/apm.o \
./src/linux/kernel_routes_nl.o \
./src/linux/kernel_tunnel.o \
./src/linux/link_layer.o \
./src/linux/net.o 

C_DEPS += \
./src/linux/apm.d \
./src/linux/kernel_routes_nl.d \
./src/linux/kernel_tunnel.d \
./src/linux/link_layer.d \
./src/linux/net.d 


# Each subdirectory must supply rules for building sources it contributes
src/linux/%.o: ../src/linux/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


