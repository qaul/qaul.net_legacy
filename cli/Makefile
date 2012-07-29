#
# File:  Makefile for application
#

TOP:=$(dir $(lastword $(MAKEFILE_LIST)))
PJBASE=$(TOP)/../pjproject-2.0.1

CC=gcc
LIBDEST=./

# osx
ifeq ($(OS),osx)
OSX_CFLAGS=-arch i386
LFLAGS=-arch i386
LIBS= \
	-lpjsua-i386-apple-darwin10.8.0 \
	-lpjsip-ua-i386-apple-darwin10.8.0 \
	-lpjsip-simple-i386-apple-darwin10.8.0 \
	-lpjsip-i386-apple-darwin10.8.0 \
	-lpjmedia-codec-i386-apple-darwin10.8.0 \
	-lpjmedia-videodev-i386-apple-darwin10.8.0 \
	-lpjmedia-i386-apple-darwin10.8.0 \
	-lpjmedia-audiodev-i386-apple-darwin10.8.0 \
	-lpjnath-i386-apple-darwin10.8.0 \
	-lpjlib-util-i386-apple-darwin10.8.0 \
	-lresample-i386-apple-darwin10.8.0 \
	-lmilenage-i386-apple-darwin10.8.0 \
	-lsrtp-i386-apple-darwin10.8.0 \
	-lgsmcodec-i386-apple-darwin10.8.0 \
	-lspeex-i386-apple-darwin10.8.0 \
	-lilbccodec-i386-apple-darwin10.8.0 \
	-lg7221codec-i386-apple-darwin10.8.0 \
	-lportaudio-i386-apple-darwin10.8.0 \
	-lpj-i386-apple-darwin10.8.0 \
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
LIBS= \
	
endif

# windows
ifeq ($(OS),win)
LIBDEST_WIN= \
		-I../libqaul \
		-L../libqaul \
		-lqaul

SRC=main.c
else
CFLAGS= $(OSX_CFLAGS) \
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

SRC= ../libqaul/qaullib.c ../libqaul/qaullib_ipc.c ../libqaul/qaullib_webserver.c ../libqaul/qaullib_voip.c \
     ../libqaul/qaullib_webclient.c ../libqaul/qaullib_filesharing.c ../libqaul/qaullib_threads.c \
     ../libqaul/qaullib_user.c ../libqaul/qaullib_user_LL.c \
     ../libqaul/captive/qaullib_captive.c ../libqaul/captive/qaullib_captive_dhcp.c ../libqaul/captive/qaullib_captive_dns.c \
     ../libqaul/mongoose/mongoose.c ../libqaul/sqlite/sqlite3.c \
     ../libqaul/urlcode/urlcode.c ../libqaul/bstrlib/bstrlib.c ../libqaul/polarssl/sha1.c \
     ../libqaul/olsrd/mantissa.c ../libqaul/olsrd/hashing.c \
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
	@echo "make ( OS=osx | OS=linux | OS=win )"
	@echo "make clean ( OS=win )"
else
all: $(OBJS)
ifeq ($(OS),osx)
	@echo application Makefile - linking qaul
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul $(LFLAGS)
	@echo copy olsrd executable into this directory
	@cp ../olsrd-0.6.0/olsrd ./
	@echo copy olsrd_qaul plugin into this directory
	@cp ../olsrd-0.6.0/lib/olsrd_qaul/olsrd_qaul.so.0.1 ./
endif
ifeq ($(OS),linux)
	@echo application Makefile - linking qaul
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul -lpthread -ldl -lm
	@echo copy olsrd executable into this directory
	@cp ../olsrd_0.6.2/olsrd ./
	@echo copy olsrd_qaul plugin into this directory
	@cp ../olsrd_0.6.2/lib/olsrd_qaul/olsrd_qaul.so.0.1 ./
endif
ifeq ($(OS),win)
	@echo application Makefile - linking qaul.exe
	@cp ../libqaul/libqaul.dll ./
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul.exe $(CFLAGS) $(LIBDEST_WIN)
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
