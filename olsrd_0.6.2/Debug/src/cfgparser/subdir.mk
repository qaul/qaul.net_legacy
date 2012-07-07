################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/cfgparser/cfgfile_gen.o \
../src/cfgparser/olsrd_conf.o \
../src/cfgparser/oparse.o \
../src/cfgparser/oscan.o 

C_SRCS += \
../src/cfgparser/cfgfile_gen.c \
../src/cfgparser/olsrd_conf.c \
../src/cfgparser/oparse.c \
../src/cfgparser/oscan.c 

OBJS += \
./src/cfgparser/cfgfile_gen.o \
./src/cfgparser/olsrd_conf.o \
./src/cfgparser/oparse.o \
./src/cfgparser/oscan.o 

C_DEPS += \
./src/cfgparser/cfgfile_gen.d \
./src/cfgparser/olsrd_conf.d \
./src/cfgparser/oparse.d \
./src/cfgparser/oscan.d 


# Each subdirectory must supply rules for building sources it contributes
src/cfgparser/%.o: ../src/cfgparser/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


