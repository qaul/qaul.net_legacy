################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/tas/src/lua/lapi.c \
../lib/tas/src/lua/lauxlib.c \
../lib/tas/src/lua/lbaselib.c \
../lib/tas/src/lua/lcode.c \
../lib/tas/src/lua/ldblib.c \
../lib/tas/src/lua/ldebug.c \
../lib/tas/src/lua/ldo.c \
../lib/tas/src/lua/ldump.c \
../lib/tas/src/lua/lfunc.c \
../lib/tas/src/lua/lgc.c \
../lib/tas/src/lua/liolib.c \
../lib/tas/src/lua/llex.c \
../lib/tas/src/lua/lmathlib.c \
../lib/tas/src/lua/lmem.c \
../lib/tas/src/lua/loadlib.c \
../lib/tas/src/lua/lobject.c \
../lib/tas/src/lua/lopcodes.c \
../lib/tas/src/lua/lparser.c \
../lib/tas/src/lua/lstate.c \
../lib/tas/src/lua/lstring.c \
../lib/tas/src/lua/lstrlib.c \
../lib/tas/src/lua/ltable.c \
../lib/tas/src/lua/ltablib.c \
../lib/tas/src/lua/ltests.c \
../lib/tas/src/lua/ltm.c \
../lib/tas/src/lua/lundump.c \
../lib/tas/src/lua/lvm.c \
../lib/tas/src/lua/lzio.c 

OBJS += \
./lib/tas/src/lua/lapi.o \
./lib/tas/src/lua/lauxlib.o \
./lib/tas/src/lua/lbaselib.o \
./lib/tas/src/lua/lcode.o \
./lib/tas/src/lua/ldblib.o \
./lib/tas/src/lua/ldebug.o \
./lib/tas/src/lua/ldo.o \
./lib/tas/src/lua/ldump.o \
./lib/tas/src/lua/lfunc.o \
./lib/tas/src/lua/lgc.o \
./lib/tas/src/lua/liolib.o \
./lib/tas/src/lua/llex.o \
./lib/tas/src/lua/lmathlib.o \
./lib/tas/src/lua/lmem.o \
./lib/tas/src/lua/loadlib.o \
./lib/tas/src/lua/lobject.o \
./lib/tas/src/lua/lopcodes.o \
./lib/tas/src/lua/lparser.o \
./lib/tas/src/lua/lstate.o \
./lib/tas/src/lua/lstring.o \
./lib/tas/src/lua/lstrlib.o \
./lib/tas/src/lua/ltable.o \
./lib/tas/src/lua/ltablib.o \
./lib/tas/src/lua/ltests.o \
./lib/tas/src/lua/ltm.o \
./lib/tas/src/lua/lundump.o \
./lib/tas/src/lua/lvm.o \
./lib/tas/src/lua/lzio.o 

C_DEPS += \
./lib/tas/src/lua/lapi.d \
./lib/tas/src/lua/lauxlib.d \
./lib/tas/src/lua/lbaselib.d \
./lib/tas/src/lua/lcode.d \
./lib/tas/src/lua/ldblib.d \
./lib/tas/src/lua/ldebug.d \
./lib/tas/src/lua/ldo.d \
./lib/tas/src/lua/ldump.d \
./lib/tas/src/lua/lfunc.d \
./lib/tas/src/lua/lgc.d \
./lib/tas/src/lua/liolib.d \
./lib/tas/src/lua/llex.d \
./lib/tas/src/lua/lmathlib.d \
./lib/tas/src/lua/lmem.d \
./lib/tas/src/lua/loadlib.d \
./lib/tas/src/lua/lobject.d \
./lib/tas/src/lua/lopcodes.d \
./lib/tas/src/lua/lparser.d \
./lib/tas/src/lua/lstate.d \
./lib/tas/src/lua/lstring.d \
./lib/tas/src/lua/lstrlib.d \
./lib/tas/src/lua/ltable.d \
./lib/tas/src/lua/ltablib.d \
./lib/tas/src/lua/ltests.d \
./lib/tas/src/lua/ltm.d \
./lib/tas/src/lua/lundump.d \
./lib/tas/src/lua/lvm.d \
./lib/tas/src/lua/lzio.d 


# Each subdirectory must supply rules for building sources it contributes
lib/tas/src/lua/%.o: ../lib/tas/src/lua/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


