#
# File:  Makefile for application
#
CC=gcc
#CFLAGS=-arch i386
#CFLAGS=-g

## for memdbg
#CFLAGS=`memdgb-config --cflags`
#LIBS=`memdgb-config --clibs`

SRC= ../libqaul/qaullib.c ../libqaul/qaullib_user.c ../libqaul/qaullib_ipc.c ../libqaul/qaullib_webserver.c \
     ../libqaul/qaullib_webclient.c ../libqaul/qaullib_filesharing.c ../libqaul/qaullib_threads.c \
     ../libqaul/qaullib_user_LL.c \
     ../libqaul/captive/qaullib_captive.c ../libqaul/captive/qaullib_captive_dhcp.c ../libqaul/captive/qaullib_captive_dns.c \
     ../libqaul/mongoose/mongoose.c ../libqaul/sqlite/sqlite3.c \
     ../libqaul/urlcode/urlcode.c ../libqaul/bstrlib/bstrlib.c ../libqaul/polarssl/sha1.c \
     ../libqaul/olsrd/mantissa.c ../libqaul/olsrd/hashing.c \
     main.c
     
SRC_WIN= ../libqaul/win32/functions.c ../libqaul/win32/pthread.c

OBJS=$(SRC:.c=.o)
OBJS_WIN=$(SRC:.c=.o) $(SRC_WIN:.c=.o)


all: 
	@echo "make ( osx | linux | win )"

osx: $(OBJS)
	@echo application Makefile - linking qaul
	@echo $(CC) $^ $(LIBDIR) $(LIBS) -o qaul
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul $(LFLAGS)

linux: $(OBJS)
	@echo application Makefile - linking qaul
	@echo $(CC) $^ $(LIBDIR) $(LIBS) -o qaul
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul -lpthread -ldl -lm
	#@$(CC) `memdbg-config --clibs` $^ $(LIBDIR) $(LIBS) -o qaul -lpthread -ldl -lm -lMD

win: $(OBJS_WIN)
	@echo application Makefile - linking qaul.exe
	@echo $(CC) $^ $(LIBDIR) $(LIBS) -o qaul.exe
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul.exe -lwsock32 -lWs2_32

$(EXE): $(OBJS)
	@echo application Makefile - linking $<
	@echo $(CC) $^ $(LIBDIR) $(LIBS) -o $@
	@$(CC) $^ $(LIBDIR) $(LIBS) -o $@ $(LFLAGS)

.c.o:
	@echo application Makefile - compiling $<
	@echo $(CC) $(CFLAGS) -c $< -o $@
	@$(CC) $(CFLAGS) -c $< -o $@
	#@$(CC) `memdbg-config --cflags` $(CFLAGS) -c $< -o $@

clean:
	@echo "make clean"
	@echo rm $(OBJS) qaul
	@rm $(OBJS)
	@rm qaul
	
clean_win:
	@echo "make clean"
	@echo rm $(OBJS_WIN) qaul.exe
	@rm $(OBJS_WIN)
	@rm qaul.exe
