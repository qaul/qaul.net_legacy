################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/secure/src/md5.c \
../lib/secure/src/olsrd_plugin.c \
../lib/secure/src/olsrd_secure.c 

OBJS += \
./lib/secure/src/md5.o \
./lib/secure/src/olsrd_plugin.o \
./lib/secure/src/olsrd_secure.o 

C_DEPS += \
./lib/secure/src/md5.d \
./lib/secure/src/olsrd_plugin.d \
./lib/secure/src/olsrd_secure.d 


# Each subdirectory must supply rules for building sources it contributes
lib/secure/src/%.o: ../lib/secure/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


