################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/tas/src/glua.c \
../lib/tas/src/glua_ext.c \
../lib/tas/src/http.c \
../lib/tas/src/lib.c \
../lib/tas/src/os_unix.c \
../lib/tas/src/plugin.c 

OBJS += \
./lib/tas/src/glua.o \
./lib/tas/src/glua_ext.o \
./lib/tas/src/http.o \
./lib/tas/src/lib.o \
./lib/tas/src/os_unix.o \
./lib/tas/src/plugin.o 

C_DEPS += \
./lib/tas/src/glua.d \
./lib/tas/src/glua_ext.d \
./lib/tas/src/http.d \
./lib/tas/src/lib.d \
./lib/tas/src/os_unix.d \
./lib/tas/src/plugin.d 


# Each subdirectory must supply rules for building sources it contributes
lib/tas/src/%.o: ../lib/tas/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


