#
# qaulhelper Linux application Makefile
#
# qaul.net is free software
# licensed under GPL (version 3)
#

TOP:=$(dir $(lastword $(MAKEFILE_LIST)))

CC=gcc
LIBDEST=./

LIBS= \
    -lpthread \

SRC=main.c

			
OBJS=$(SRC:.c=.o)


all: $(OBJS)
	@echo application Makefile - linking qaulhelper
	@$(CC) $^ $(LIBDIR) $(LIBS) -o qaulhelper $(LFLAGS)

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
	@rm qaulhelper
