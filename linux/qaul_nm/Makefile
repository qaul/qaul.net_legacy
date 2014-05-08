#
# File:  Makefile for application
#

TOP:=$(dir $(lastword $(MAKEFILE_LIST)))
DISTBASE=$(TOP)../..

CC=gcc
LIBDEST=./


# linux
PJSIPTYPE=i686-pc-linux-gnu
LIBS= \
     -ldbus-1 \
     -ldbus-glib-1 \
     `pkg-config --libs webkitgtk-3.0`
#    `pkg-config --libs glib-2.0`


CFLAGS= \
	-I/usr/include/dbus-1.0 \
	`pkg-config --cflags webkitgtk-3.0`
#	`pkg-config --cflags glib-2.0`

SRC= main.c


LIBDIR= \
	
OBJS=$(SRC:.c=.o)


all: $(OBJS)
	@echo application Makefile - linking qaul
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaul $(LFLAGS)

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

