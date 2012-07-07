include ../Makefile.inc

#
# Settings
#

SAMPLES = generate generator math parse parse_file
SMPLS = $(SAMPLES:%=../build/samples/%)
SMPLOBJ = $(SAMPLES:%=%/main.o)

LIBS = -lm -L../lib -lnmea
INCS = -I ../include


#
# Targets
#

all: all-before samples

remake: clean all

samples: $(SMPLS)

../build/samples/%: %/main.o
	@echo "[LD] $@"
	@$(CC) $(CCFLAGS) $< $(LIBS) -o $@

%/main.o: %/main.c Makefile ../Makefile.inc
	@echo "[CC] $<"
	@$(CC) $(CCFLAGS) $(INCS) -c $< -o $@


#
# Phony Targets
#

.PHONY: all-before clean

all-before:
	@mkdir -p ../build/samples

clean:
	rm -f $(SMPLOBJ) $(SMPLS)
