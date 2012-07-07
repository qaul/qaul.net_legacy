################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/win32/subdir.mk
-include src/unix/subdir.mk
-include src/subdir.mk
-include src/olsr_switch/subdir.mk
-include src/linux/subdir.mk
-include src/common/subdir.mk
-include src/cfgparser/subdir.mk
-include src/bsd/subdir.mk
-include lib/watchdog/src/subdir.mk
-include lib/txtinfo/src/subdir.mk
-include lib/tas/subdir.mk
-include lib/tas/src/subdir.mk
-include lib/tas/src/lua/subdir.mk
-include lib/secure/src/subdir.mk
-include lib/quagga/src/subdir.mk
-include lib/pud/wireformat-java/src/main/c/subdir.mk
-include lib/pud/wireformat/src/subdir.mk
-include lib/pud/src/subdir.mk
-include lib/pud/nmealib/src/subdir.mk
-include lib/pud/nmealib/samples/parse_file/subdir.mk
-include lib/pud/nmealib/samples/parse/subdir.mk
-include lib/pud/nmealib/samples/math/subdir.mk
-include lib/pud/nmealib/samples/generator/subdir.mk
-include lib/pud/nmealib/samples/generate/subdir.mk
-include lib/pgraph/src/subdir.mk
-include lib/p2pd/src/subdir.mk
-include lib/olsrd_qaul/src/subdir.mk
-include lib/nameservice/src/subdir.mk
-include lib/mini/src/subdir.mk
-include lib/mdns/src/subdir.mk
-include lib/jsoninfo/src/subdir.mk
-include lib/httpinfo/src/subdir.mk
-include lib/dyn_gw_plain/src/subdir.mk
-include lib/dyn_gw/src/subdir.mk
-include lib/dot_draw/src/subdir.mk
-include lib/bmf/src/subdir.mk
-include lib/arprefresh/src/subdir.mk
-include gui/win32/Shim/subdir.mk
-include gui/linux-gtk/src/subdir.mk
-include contrib/netsimpcap/src/subdir.mk
-include android/regex/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: olsrd_0.6.2

# Tool invocations
olsrd_0.6.2: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o"olsrd_0.6.2" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) olsrd_0.6.2
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
