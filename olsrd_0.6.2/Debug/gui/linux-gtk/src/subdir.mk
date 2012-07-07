################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gui/linux-gtk/src/callbacks.c \
../gui/linux-gtk/src/interface.c \
../gui/linux-gtk/src/ipc.c \
../gui/linux-gtk/src/main.c \
../gui/linux-gtk/src/nodes.c \
../gui/linux-gtk/src/olsr.c \
../gui/linux-gtk/src/packet.c \
../gui/linux-gtk/src/routes.c 

OBJS += \
./gui/linux-gtk/src/callbacks.o \
./gui/linux-gtk/src/interface.o \
./gui/linux-gtk/src/ipc.o \
./gui/linux-gtk/src/main.o \
./gui/linux-gtk/src/nodes.o \
./gui/linux-gtk/src/olsr.o \
./gui/linux-gtk/src/packet.o \
./gui/linux-gtk/src/routes.o 

C_DEPS += \
./gui/linux-gtk/src/callbacks.d \
./gui/linux-gtk/src/interface.d \
./gui/linux-gtk/src/ipc.d \
./gui/linux-gtk/src/main.d \
./gui/linux-gtk/src/nodes.d \
./gui/linux-gtk/src/olsr.d \
./gui/linux-gtk/src/packet.d \
./gui/linux-gtk/src/routes.d 


# Each subdirectory must supply rules for building sources it contributes
gui/linux-gtk/src/%.o: ../gui/linux-gtk/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


