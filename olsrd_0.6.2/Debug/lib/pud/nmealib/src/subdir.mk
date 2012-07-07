################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/pud/nmealib/src/context.c \
../lib/pud/nmealib/src/generate.c \
../lib/pud/nmealib/src/generator.c \
../lib/pud/nmealib/src/gmath.c \
../lib/pud/nmealib/src/info.c \
../lib/pud/nmealib/src/parse.c \
../lib/pud/nmealib/src/parser.c \
../lib/pud/nmealib/src/sentence.c \
../lib/pud/nmealib/src/time.c \
../lib/pud/nmealib/src/tok.c \
../lib/pud/nmealib/src/util.c 

OBJS += \
./lib/pud/nmealib/src/context.o \
./lib/pud/nmealib/src/generate.o \
./lib/pud/nmealib/src/generator.o \
./lib/pud/nmealib/src/gmath.o \
./lib/pud/nmealib/src/info.o \
./lib/pud/nmealib/src/parse.o \
./lib/pud/nmealib/src/parser.o \
./lib/pud/nmealib/src/sentence.o \
./lib/pud/nmealib/src/time.o \
./lib/pud/nmealib/src/tok.o \
./lib/pud/nmealib/src/util.o 

C_DEPS += \
./lib/pud/nmealib/src/context.d \
./lib/pud/nmealib/src/generate.d \
./lib/pud/nmealib/src/generator.d \
./lib/pud/nmealib/src/gmath.d \
./lib/pud/nmealib/src/info.d \
./lib/pud/nmealib/src/parse.d \
./lib/pud/nmealib/src/parser.d \
./lib/pud/nmealib/src/sentence.d \
./lib/pud/nmealib/src/time.d \
./lib/pud/nmealib/src/tok.d \
./lib/pud/nmealib/src/util.d 


# Each subdirectory must supply rules for building sources it contributes
lib/pud/nmealib/src/%.o: ../lib/pud/nmealib/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


