#
# qaul.net GTK Linux application Makefile
#
# qaul.net is free software
# licensed under GPL (version 3)
#

TOP:=$(dir $(lastword $(MAKEFILE_LIST)))
DISTBASE=$(TOP)../..
PJBASE=$(DISTBASE)/pjproject-2.2.1

CC=gcc
LIBDEST=./


# linux
PJSIPTYPE=i686-pc-linux-gnu
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
    -lm \
    \
    `pkg-config --libs dbus-1` \
	`pkg-config --libs webkitgtk-3.0`
#    `pkg-config --libs gtk+-3.0`
#    `pkg-config --libs glib-2.0`

#	-lmilenage-$(PJSIPTYPE) \
#
#
#

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
	-I$(PJBASE)/third_party/srtp/include \
	\
	`pkg-config --cflags dbus-1` \
	`pkg-config --cflags webkitgtk-3.0`
#	`pkg-config --cflags gtk+-3.0`
#	`pkg-config --cflags glib-2.0`

SRC= $(DISTBASE)/libqaul/qaullib.c $(DISTBASE)/libqaul/qaullib_ipc.c $(DISTBASE)/libqaul/qaullib_webserver.c $(DISTBASE)/libqaul/qaullib_voip.c \
     $(DISTBASE)/libqaul/qaullib_webclient.c $(DISTBASE)/libqaul/qaullib_threads.c \
     $(DISTBASE)/libqaul/qaullib_user.c $(DISTBASE)/libqaul/qaullib_user_LL.c \
     $(DISTBASE)/libqaul/qaullib_topo_LL.c \
     $(DISTBASE)/libqaul/qaullib_appevent_LL.c \
     $(DISTBASE)/libqaul/qaullib_filesharing.c $(DISTBASE)/libqaul/qaullib_file_LL.c \
     $(DISTBASE)/libqaul/qaullib_exediscovery.c \
     $(DISTBASE)/libqaul/qaullib_udp_communication.c \
     $(DISTBASE)/libqaul/captive/qaullib_captive.c $(DISTBASE)/libqaul/captive/qaullib_captive_dhcp.c $(DISTBASE)/libqaul/captive/qaullib_captive_dns.c \
     $(DISTBASE)/libqaul/mongoose/mongoose.c $(DISTBASE)/libqaul/sqlite/sqlite3.c \
     $(DISTBASE)/libqaul/urlcode/urlcode.c $(DISTBASE)/libqaul/polarssl/sha1.c \
     $(DISTBASE)/libqaul/olsrd/mantissa.c $(DISTBASE)/libqaul/olsrd/hashing.c \
     network.c main.c

LIBDIR= \
	-L$(PJBASE)/pjlib/lib \
	-L$(PJBASE)/pjlib-util/lib \
	-L$(PJBASE)/pjmedia/lib \
	-L$(PJBASE)/pjnath/lib \
	-L$(PJBASE)/pjsip/lib \
	-L$(PJBASE)/third_party/lib	\
	
OBJS=$(SRC:.c=.o)


all: $(OBJS)
	@echo application Makefile - linking qaul
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul $(LFLAGS)
	@echo copy olsrd executable into this directory
	@cp ../../olsrd_0.6.2/olsrd ./
	@echo copy olsrd_qaul plugin into this directory
	@cp ../../olsrd_0.6.2/lib/olsrd_qaul/olsrd_qaul.so.0.1 ./
	@echo copy www folder into this directory
	@cp -R ../../www ./

$(EXE): $(OBJS)
	@echo application Makefile - linking $<
	@echo $(CC) $^ $(LIBDIR) $(LIBS) -o $@
	@$(CC) $^ $(LIBDIR) $(LIBS) -o $@ $(LFLAGS)
	
.c.o:
	@echo lib Makefile - compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "make clean"
	@echo rm $(OBJS)
	@rm $(OBJS)
	@rm qaul
