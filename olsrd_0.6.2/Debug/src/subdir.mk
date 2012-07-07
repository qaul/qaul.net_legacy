################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/build_msg.o \
../src/builddata.o \
../src/duplicate_handler.o \
../src/duplicate_set.o \
../src/fpm.o \
../src/gateway.o \
../src/gateway_default_handler.o \
../src/generate_msg.o \
../src/hashing.o \
../src/hna_set.o \
../src/hysteresis.o \
../src/interfaces.o \
../src/ipc_frontend.o \
../src/ipcalc.o \
../src/link_set.o \
../src/lq_mpr.o \
../src/lq_packet.o \
../src/lq_plugin.o \
../src/lq_plugin_default_ff.o \
../src/lq_plugin_default_ffeth.o \
../src/lq_plugin_default_float.o \
../src/lq_plugin_default_fpm.o \
../src/main.o \
../src/mantissa.o \
../src/mid_set.o \
../src/mpr.o \
../src/mpr_selector_set.o \
../src/neighbor_table.o \
../src/net_olsr.o \
../src/olsr.o \
../src/olsr_cookie.o \
../src/olsr_niit.o \
../src/olsr_spf.o \
../src/packet.o \
../src/parser.o \
../src/plugin_loader.o \
../src/plugin_util.o \
../src/print_packet.o \
../src/process_package.o \
../src/process_routes.o \
../src/rebuild_packet.o \
../src/routing_table.o \
../src/scheduler.o \
../src/tc_set.o \
../src/two_hop_neighbor_table.o 

C_SRCS += \
../src/build_msg.c \
../src/builddata.c \
../src/duplicate_handler.c \
../src/duplicate_set.c \
../src/fpm.c \
../src/gateway.c \
../src/gateway_default_handler.c \
../src/generate_msg.c \
../src/hashing.c \
../src/hna_set.c \
../src/hysteresis.c \
../src/interfaces.c \
../src/ipc_frontend.c \
../src/ipcalc.c \
../src/link_set.c \
../src/lq_mpr.c \
../src/lq_packet.c \
../src/lq_plugin.c \
../src/lq_plugin_default_ff.c \
../src/lq_plugin_default_ffeth.c \
../src/lq_plugin_default_float.c \
../src/lq_plugin_default_fpm.c \
../src/main.c \
../src/mantissa.c \
../src/mid_set.c \
../src/mpr.c \
../src/mpr_selector_set.c \
../src/neighbor_table.c \
../src/net_olsr.c \
../src/olsr.c \
../src/olsr_cookie.c \
../src/olsr_niit.c \
../src/olsr_spf.c \
../src/packet.c \
../src/parser.c \
../src/plugin_loader.c \
../src/plugin_util.c \
../src/print_packet.c \
../src/process_package.c \
../src/process_routes.c \
../src/rebuild_packet.c \
../src/routing_table.c \
../src/scheduler.c \
../src/tc_set.c \
../src/two_hop_neighbor_table.c 

OBJS += \
./src/build_msg.o \
./src/builddata.o \
./src/duplicate_handler.o \
./src/duplicate_set.o \
./src/fpm.o \
./src/gateway.o \
./src/gateway_default_handler.o \
./src/generate_msg.o \
./src/hashing.o \
./src/hna_set.o \
./src/hysteresis.o \
./src/interfaces.o \
./src/ipc_frontend.o \
./src/ipcalc.o \
./src/link_set.o \
./src/lq_mpr.o \
./src/lq_packet.o \
./src/lq_plugin.o \
./src/lq_plugin_default_ff.o \
./src/lq_plugin_default_ffeth.o \
./src/lq_plugin_default_float.o \
./src/lq_plugin_default_fpm.o \
./src/main.o \
./src/mantissa.o \
./src/mid_set.o \
./src/mpr.o \
./src/mpr_selector_set.o \
./src/neighbor_table.o \
./src/net_olsr.o \
./src/olsr.o \
./src/olsr_cookie.o \
./src/olsr_niit.o \
./src/olsr_spf.o \
./src/packet.o \
./src/parser.o \
./src/plugin_loader.o \
./src/plugin_util.o \
./src/print_packet.o \
./src/process_package.o \
./src/process_routes.o \
./src/rebuild_packet.o \
./src/routing_table.o \
./src/scheduler.o \
./src/tc_set.o \
./src/two_hop_neighbor_table.o 

C_DEPS += \
./src/build_msg.d \
./src/builddata.d \
./src/duplicate_handler.d \
./src/duplicate_set.d \
./src/fpm.d \
./src/gateway.d \
./src/gateway_default_handler.d \
./src/generate_msg.d \
./src/hashing.d \
./src/hna_set.d \
./src/hysteresis.d \
./src/interfaces.d \
./src/ipc_frontend.d \
./src/ipcalc.d \
./src/link_set.d \
./src/lq_mpr.d \
./src/lq_packet.d \
./src/lq_plugin.d \
./src/lq_plugin_default_ff.d \
./src/lq_plugin_default_ffeth.d \
./src/lq_plugin_default_float.d \
./src/lq_plugin_default_fpm.d \
./src/main.d \
./src/mantissa.d \
./src/mid_set.d \
./src/mpr.d \
./src/mpr_selector_set.d \
./src/neighbor_table.d \
./src/net_olsr.d \
./src/olsr.d \
./src/olsr_cookie.d \
./src/olsr_niit.d \
./src/olsr_spf.d \
./src/packet.d \
./src/parser.d \
./src/plugin_loader.d \
./src/plugin_util.d \
./src/print_packet.d \
./src/process_package.d \
./src/process_routes.d \
./src/rebuild_packet.d \
./src/routing_table.d \
./src/scheduler.d \
./src/tc_set.d \
./src/two_hop_neighbor_table.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


