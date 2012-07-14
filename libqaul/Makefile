#
# File:  Makefile (for library)
#

PJBASE=../pjproject-2.0.1

CC=gcc
LIBDEST=./
CFLAGS= \
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

#CFLAGS=-arch i386 $(CFLAGS)

SRC= qaullib.c qaullib_ipc.c qaullib_webserver.c qaullib_voip.c \
     qaullib_webclient.c qaullib_filesharing.c qaullib_threads.c \
     qaullib_user.c qaullib_user_LL.c \
     captive/qaullib_captive.c captive/qaullib_captive_dhcp.c captive/qaullib_captive_dns.c \
     mongoose/mongoose.c sqlite/sqlite3.c \
     urlcode/urlcode.c bstrlib/bstrlib.c polarssl/sha1.c \
     olsrd/mantissa.c olsrd/hashing.c 
     
SRC_WIN= win32/functions.c win32/pthread.c

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
	@$(CC) -shared -o libqaul.dll $(SRC) $(SRC_WIN) -lwsock32 -lWs2_32 -Wl,--output-def,libqaul.def,--out-implib,libqaul.a
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
