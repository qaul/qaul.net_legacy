#
# File:  Makefile (for library)
#
TOP:=$(dir $(lastword $(MAKEFILE_LIST)))
PJBASE=$(TOP)/../pjproject-2.0.1
PJ_WIN32=1

CC=gcc
LIBDEST=.
#OSX_CFLAGS=-arch i386
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


SRC= qaullib.c qaullib_ipc.c qaullib_webserver.c qaullib_voip.c \
     qaullib_webclient.c qaullib_filesharing.c qaullib_threads.c \
     qaullib_user.c qaullib_user_LL.c \
     captive/qaullib_captive.c captive/qaullib_captive_dhcp.c captive/qaullib_captive_dns.c \
     mongoose/mongoose.c sqlite/sqlite3.c \
     urlcode/urlcode.c bstrlib/bstrlib.c polarssl/sha1.c \
     olsrd/mantissa.c olsrd/hashing.c 
     
SRC_WIN= win32/functions.c win32/pthread.c

LIBDEST_WIN= \
			-L$(PJBASE)/pjlib/lib \
			-L$(PJBASE)/pjlib-util/lib \
			-L$(PJBASE)/pjmedia/lib \
			-L$(PJBASE)/pjnath/lib \
			-L$(PJBASE)/pjsip/lib \
			-L$(PJBASE)/third_party/lib \
			\
			-lpjsua-i686-pc-mingw32 \
			-lpjsip-ua-i686-pc-mingw32 \
			-lpjsip-simple-i686-pc-mingw32 \
			-lpjsip-i686-pc-mingw32 \
			-lpjmedia-audiodev-i686-pc-mingw32 \
			-lpjmedia-codec-i686-pc-mingw32 \
			-lpjmedia-i686-pc-mingw32 \
			-lpjmedia-videodev-i686-pc-mingw32 \
			-lpjnath-i686-pc-mingw32 \
			-lpjlib-util-i686-pc-mingw32 \
			-lresample-i686-pc-mingw32 \
			-lmilenage-i686-pc-mingw32 \
			-lsrtp-i686-pc-mingw32 \
			-lgsmcodec-i686-pc-mingw32 \
			-lspeex-i686-pc-mingw32 \
			-lilbccodec-i686-pc-mingw32 \
			-lg7221codec-i686-pc-mingw32 \
			-lportaudio-i686-pc-mingw32 \
			-lpj-i686-pc-mingw32 \
			\
			-L/lib \
			-lm \
			-lwinmm \
			-m32
			
# Windows
LIB=libqaul.lib

OBJS=$(SRC:.c=.o)
OBJS_WIN=$(SRC:.c=.o) $(SRC_WIN:.c=.o)

all:
	@echo "make ( osx | linux | win )"

osx: $(OBJS)
	@echo lib Makefile - archiving libqaul.a
	@$(AR) r libqaul.a $(OBJS)

linux: $(OBJS)
	@echo lib Makefile - archiving libqaul.a
	@$(AR) r libqaul.a $(OBJS)
	
win: $(OBJS_WIN)
	@echo lib Makefile - archiving libqaul.lib
	@$(CC) -shared -o libqaul.dll $(CFLAGS) $(SRC) $(SRC_WIN) $(LIBDEST_WIN) -lwsock32 -lWs2_32 -lole32 -ldsound -Wl,--output-def,libqaul.def,--out-implib,libqaul.a
	@lib /machine:i386 /def:libqaul.def
	@copy .\libqaul.dll "..\win\Debug\"
	@copy .\libqaul.dll "..\win\Release\"
	
$(LIB): $(OBJS)
	@echo lib Makefile - archiving $(LIB)
	@$(AR) r $(LIB) $(LIBOBJ)

.c.o:
	@echo lib Makefile - compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "make clean"
	@echo rm $(OBJS)
	@rm $(OBJS)
	@rm libqaul.a

clean_win:
	@echo "make clean_win"
	@rm $(OBJS_WIN)
	@rm libqaul.a libqaul.dll libqaul.def
	@rm libqaul.lib libqaul.exp
