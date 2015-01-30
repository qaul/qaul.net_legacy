#
# qaul.net is free software
# licensed under GPL (version 3)
#

##
## qaul.net cli application Makefile for OpenWRT
## 
## You need to have the OpenWRT Toolchain installed in the parent directory.
##

# set build_root environment
PATH:=$(PWD)/../OpenWrt-Toolchain-ar71xx-for-mips_34kc-gcc-4.8-linaro_uClibc-0.9.33.2/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/bin:$(PATH)
STAGING_DIR=$(PWD)/../OpenWrt-Toolchain-ar71xx-for-mips_34kc-gcc-4.8-linaro_uClibc-0.9.33.2/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2
export STAGING_DIR

# the application name
APPNAME=qaul

SRC= \
	../../libqaul/qaullib_voip_dummy.c \
	../../libqaul/qaullib.c ../../libqaul/qaullib_ipc.c ../../libqaul/qaullib_webserver.c \
	../../libqaul/qaullib_webclient.c ../../libqaul/qaullib_threads.c \
	../../libqaul/qaullib_user.c ../../libqaul/qaullib_user_LL.c \
	../../libqaul/qaullib_topo_LL.c \
	../../libqaul/qaullib_appevent_LL.c \
	../../libqaul/qaullib_messaging.c ../../libqaul/qaullib_msg_LL.c ../../libqaul/qaullib_validate.c \
	../../libqaul/qaullib_filesharing.c ../../libqaul/qaullib_file_LL.c \
	../../libqaul/qaullib_exediscovery.c \
	../../libqaul/qaullib_udp_communication.c \
	../../libqaul/captive/qaullib_captive.c ../../libqaul/captive/qaullib_captive_dhcp.c ../../libqaul/captive/qaullib_captive_dns.c \
	../../libqaul/mongoose/mongoose.c ../../libqaul/sqlite/sqlite3.c \
	../../libqaul/urlcode/urlcode.c ../../libqaul/polarssl/sha1.c \
	../../libqaul/olsrd/mantissa.c ../../libqaul/olsrd/hashing.c \
	../../cli/main.c

LIBS= -L$(STAGING_DIR)/include \
	-lpthread \
	-ldl \
	-lm

# strip the binary
CFLAGS=-Os -s

OBJS=$(SRC:.c=.o)

all: $(OBJS)
	@echo application Makefile - linking $(APPNAME)
	@$(CC) $^ $(LIBDIR) $(LIBS) -o $(APPNAME) $(LFLAGS)

$(EXE): $(OBJS)
	@echo application Makefile - linking $<
	@echo $(CC) $^ $(LIBDIR) $(LIBS) -o $@
	@$(CC) $^ $(LIBDIR) $(LIBS) -o $@ $(LFLAGS)
	
.c.o:
	@echo compile $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "make clean"
	@echo rm $(OBJS)
	@rm $(OBJS)
	@rm $(APPNAME)
