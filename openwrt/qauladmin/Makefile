#
# qaul.net admin application Makefile
#
# qaul.net is free software
# licensed under GPL (version 3)
#

# set build_root environment
PATH:=$(PWD)/../OpenWrt-Toolchain-ar71xx-for-mips_34kc-gcc-4.8-linaro_uClibc-0.9.33.2/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/bin:$(PATH)
STAGING_DIR=$(PWD)/../OpenWrt-Toolchain-ar71xx-for-mips_34kc-gcc-4.8-linaro_uClibc-0.9.33.2/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2
export STAGING_DIR

CC=mips-openwrt-linux-uclibc-gcc

APPNAME=qauladmin
SRC=main.c
LIBS= -L$(STAGING_DIR)/include -lcrypt

OBJS=$(SRC:.c=.o)

all: $(OBJS)
	@echo application Makefile - linking $(APPNAME)
	@$(CC) $^ $(LIBDIR) $(LIBS) -o $(APPNAME) $(LFLAGS)
	@cp $(APPNAME) ../image_files/qaul/bin/

$(EXE): $(OBJS)
	@echo application Makefile - linking $<
	@echo $(CC) $^ $(LIBDIR) $(LIBS) -o $@
	@$(CC) $^ $(LIBDIR) $(LIBS) -o $@ $(LFLAGS)
	
.c.o:
	@echo make $(APPNAME)
	@echo ""
	@echo lib Makefile - compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "make clean"
	@echo rm $(OBJS)
	@rm $(OBJS)
	@rm $(APPNAME)
