################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/win32/apm.c \
../src/win32/compat.c \
../src/win32/dummy.c \
../src/win32/ifnet.c \
../src/win32/kernel_routes.c \
../src/win32/log.c \
../src/win32/misc.c \
../src/win32/net.c 

OBJS += \
./src/win32/apm.o \
./src/win32/compat.o \
./src/win32/dummy.o \
./src/win32/ifnet.o \
./src/win32/kernel_routes.o \
./src/win32/log.o \
./src/win32/misc.o \
./src/win32/net.o 

C_DEPS += \
./src/win32/apm.d \
./src/win32/compat.d \
./src/win32/dummy.d \
./src/win32/ifnet.d \
./src/win32/kernel_routes.d \
./src/win32/log.d \
./src/win32/misc.d \
./src/win32/net.d 


# Each subdirectory must supply rules for building sources it contributes
src/win32/%.o: ../src/win32/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


