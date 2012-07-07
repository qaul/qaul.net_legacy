################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../android/regex/engine.c \
../android/regex/regcomp.c \
../android/regex/regerror.c \
../android/regex/regexec.c \
../android/regex/regfree.c 

OBJS += \
./android/regex/engine.o \
./android/regex/regcomp.o \
./android/regex/regerror.o \
./android/regex/regexec.o \
./android/regex/regfree.o 

C_DEPS += \
./android/regex/engine.d \
./android/regex/regcomp.d \
./android/regex/regerror.d \
./android/regex/regexec.d \
./android/regex/regfree.d 


# Each subdirectory must supply rules for building sources it contributes
android/regex/%.o: ../android/regex/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


