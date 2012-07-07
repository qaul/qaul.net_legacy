################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/pud/wireformat-java/src/main/c/org_olsr_plugin_pud_ClusterLeader.c \
../lib/pud/wireformat-java/src/main/c/org_olsr_plugin_pud_PositionUpdate.c \
../lib/pud/wireformat-java/src/main/c/org_olsr_plugin_pud_UplinkMessage.c 

OBJS += \
./lib/pud/wireformat-java/src/main/c/org_olsr_plugin_pud_ClusterLeader.o \
./lib/pud/wireformat-java/src/main/c/org_olsr_plugin_pud_PositionUpdate.o \
./lib/pud/wireformat-java/src/main/c/org_olsr_plugin_pud_UplinkMessage.o 

C_DEPS += \
./lib/pud/wireformat-java/src/main/c/org_olsr_plugin_pud_ClusterLeader.d \
./lib/pud/wireformat-java/src/main/c/org_olsr_plugin_pud_PositionUpdate.d \
./lib/pud/wireformat-java/src/main/c/org_olsr_plugin_pud_UplinkMessage.d 


# Each subdirectory must supply rules for building sources it contributes
lib/pud/wireformat-java/src/main/c/%.o: ../lib/pud/wireformat-java/src/main/c/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


