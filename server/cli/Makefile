#
# File:  Makefile for application
#

TOP:=$(dir $(lastword $(MAKEFILE_LIST)))
PJBASE=$(TOP)../../pjproject-2.2.1

CC=gcc
LIBDEST=./

# osx
ifeq ($(OS),osx)
OSX_CFLAGS=-arch i386
LFLAGS=-arch i386
PJSIPTYPE=i386-apple-darwin10.8.0
LIBS= \
	-lpjsua-$(PJSIPTYPE) \
	-lpjsip-ua-$(PJSIPTYPE) \
	-lpjsip-simple-$(PJSIPTYPE) \
	-lpjsip-$(PJSIPTYPE) \
	-lpjmedia-codec-$(PJSIPTYPE) \
	-lpjmedia-videodev-$(PJSIPTYPE) \
	-lpjmedia-$(PJSIPTYPE) \
	-lpjmedia-audiodev-$(PJSIPTYPE) \
	-lpjnath-$(PJSIPTYPE) \
	-lpjlib-util-$(PJSIPTYPE) \
	-lresample-$(PJSIPTYPE) \
	-lmilenage-$(PJSIPTYPE) \
	-lsrtp-$(PJSIPTYPE) \
	-lgsmcodec-$(PJSIPTYPE) \
	-lspeex-$(PJSIPTYPE) \
	-lilbccodec-$(PJSIPTYPE) \
	-lg7221codec-$(PJSIPTYPE) \
	-lportaudio-$(PJSIPTYPE) \
	-lpj-$(PJSIPTYPE) \
	\
	-lm \
	-lpthread \
	-framework CoreAudio \
	-framework CoreServices \
	-framework AudioUnit \
	-framework AudioToolbox \
	-framework Foundation \
	-framework AppKit \
	-framework QTKit \
	-framework QuartzCore \
	-framework OpenGL \
	\
	-L/opt/local/lib \
	-lavformat \
	-lavcodec \
	-lz \
	-lbz2 \
	-ldirac_encoder \
	-ldirac_decoder \
	-lstdc++ \
	-lfaac \
	-lfaad \
	-lmp3lame \
	-lschroedinger-1.0 \
	-lpthread \
	-lorc-0.4 \
	-lspeex \
	-ltheoraenc \
	-ltheoradec \
	-lvorbisenc \
	-lvorbis \
	-logg \
	-lvpx \
	-lx264 \
	-lxvidcore \
	-lswscale \
	-lavutil \
	-lcrypto \
	-lssl 
endif

# linux
ifeq ($(OS),linux)
PJSIPTYPE=x86_64-unknown-linux-gnu
LIBS= \
    -lpjsua-$(PJSIPTYPE) \
    -lpjsip-ua-$(PJSIPTYPE) \
    -lpjsip-simple-$(PJSIPTYPE) \
    -lpjsip-$(PJSIPTYPE) \
    -lpjmedia-codec-$(PJSIPTYPE) \
    -lpjmedia-videodev-$(PJSIPTYPE) \
    -lpjmedia-$(PJSIPTYPE) \
    -lpjmedia-audiodev-$(PJSIPTYPE) \
    -lpjnath-$(PJSIPTYPE) \
    -lpjlib-util-$(PJSIPTYPE) \
    -lresample-$(PJSIPTYPE) \
    -lsrtp-$(PJSIPTYPE) \
    -lgsmcodec-$(PJSIPTYPE) \
    -lspeex-$(PJSIPTYPE) \
    -lilbccodec-$(PJSIPTYPE) \
    -lg7221codec-$(PJSIPTYPE) \
    -lpj-$(PJSIPTYPE) \
    \
    -L/usr/local/lib \
    \
    -lpthread \
    -ldl \
    -lm \
    -lrt
endif

# raspberry pi
ifeq ($(OS),raspberry)
PJSIPTYPE=armv6l-unknown-linux-gnu
LIBS= \
    -lpjsua-$(PJSIPTYPE) \
    -lpjsip-ua-$(PJSIPTYPE) \
    -lpjsip-simple-$(PJSIPTYPE) \
    -lpjsip-$(PJSIPTYPE) \
    -lpjmedia-codec-$(PJSIPTYPE) \
    -lpjmedia-videodev-$(PJSIPTYPE) \
    -lpjmedia-$(PJSIPTYPE) \
    -lpjmedia-audiodev-$(PJSIPTYPE) \
    -lpjnath-$(PJSIPTYPE) \
    -lpjlib-util-$(PJSIPTYPE) \
    -lresample-$(PJSIPTYPE) \
    -lmilenage-$(PJSIPTYPE) \
    -lsrtp-$(PJSIPTYPE) \
    -lgsmcodec-$(PJSIPTYPE) \
    -lspeex-$(PJSIPTYPE) \
    -lilbccodec-$(PJSIPTYPE) \
    -lg7221codec-$(PJSIPTYPE) \
    -lportaudio-$(PJSIPTYPE) \
    -lpj-$(PJSIPTYPE) \
    \
    -L/usr/local/lib \
    -lavformat \
    -lavcodec \
    -lswscale \
    -lavutil \
    -lasound \
    -lcrypto \
    -lssl \
    \
    -lpthread \
    -ldl \
    -luuid \
    -lm
endif

# windows
ifeq ($(OS),win)
LIBDEST_WIN= \
	-I../libqaul \
	-L../libqaul \
	-lqaul

SRC=main.c
else
CFLAGS= \
	$(OSX_CFLAGS) \
	-I$(PJBASE)/pjlib/include \
	-I$(PJBASE)/pjlib-util/include \
	-I$(PJBASE)/pjmedia/include \
	-I$(PJBASE)/pjnath/include \
	-I$(PJBASE)/pjsip/include \
	-I$(PJBASE)/third_party/portaudio/include \
	-I$(PJBASE)/third_party/resample/include \
	-I$(PJBASE)/third_party/speex/include \
	-I$(PJBASE)/third_party/srtp/crypto/include \
	-I$(PJBASE)/third_party/srtp/include	

SRC= ../../libqaul/qaullib.c ../../libqaul/qaullib_ipc.c ../../libqaul/qaullib_webserver.c ../../libqaul/qaullib_voip.c \
     ../../libqaul/qaullib_webclient.c ../../libqaul/qaullib_threads.c \
     ../../libqaul/qaullib_user.c ../../libqaul/qaullib_user_LL.c \
     ../../libqaul/qaullib_topo_LL.c \
     ../../libqaul/qaullib_appevent_LL.c \
     ../../libqaul/qaullib_filesharing.c ../../libqaul/qaullib_file_LL.c \
     ../../libqaul/qaullib_exediscovery.c \
     ../../libqaul/qaullib_udp_communication.c \
     ../../libqaul/captive/qaullib_captive.c ../../libqaul/captive/qaullib_captive_dhcp.c ../../libqaul/captive/qaullib_captive_dns.c \
     ../../libqaul/mongoose/mongoose.c ../../libqaul/sqlite/sqlite3.c \
     ../../libqaul/urlcode/urlcode.c ../../libqaul/polarssl/sha1.c \
     ../../libqaul/olsrd/mantissa.c ../../libqaul/olsrd/hashing.c \
     $(SRC_WIN) \
     main.c

LIBDIR= \
	-L$(PJBASE)/pjlib/lib \
	-L$(PJBASE)/pjlib-util/lib \
	-L$(PJBASE)/pjmedia/lib \
	-L$(PJBASE)/pjnath/lib \
	-L$(PJBASE)/pjsip/lib \
	-L$(PJBASE)/third_party/lib	
endif
			
OBJS=$(SRC:.c=.o)


ifndef OS
all:
	@echo "make ( OS=osx | OS=linux | OS=win | OS=raspberry )"
	@echo "make clean [ OS=win ]"
else
all: $(OBJS)
ifeq ($(OS),osx)
	@echo application Makefile - linking qaul
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul $(LFLAGS)
	@echo copy www folder into this directory
	@cp -R ../../www ./
endif
ifeq ($(OS),linux)
	@echo application Makefile - linking qaul
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul $(LFLAGS)
	@echo copy www folder into this directory
	@cp -R ../../www ./
endif
ifeq ($(OS),raspberry)
	@echo application Makefile - linking qaul
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul $(LFLAGS)
	@echo copy www folder into this directory
	@cp -R ../../www ./
endif
ifeq ($(OS),win)
	@echo application Makefile - linking qaul.exe
	@cp ../libqaul/libqaul.dll ./
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul.exe $(CFLAGS) $(LIBDEST_WIN)
	@echo copy www folder into this directory
	@cp -R ../../www ./
endif
endif

$(EXE): $(OBJS)
	@echo application Makefile - linking $<
	@echo $(CC) $^ $(LIBDIR) $(LIBS) -o $@
	@$(CC) $^ $(LIBDIR) $(LIBS) -o $@ $(LFLAGS)
	
.c.o:
	@echo lib Makefile - compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(OS),win)
	@echo "make clean OS=win"
	@echo rm $(OBJS)
	@rm $(OBJS)
	@rm qaul.exe
else
	@echo "make clean"
	@echo rm $(OBJS)
	@rm $(OBJS)
	@rm qaul
endif
