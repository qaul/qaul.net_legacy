
all: olsrd/olsrd olsrd/lib/httpinfo/olsrd_httpinfo.so.0.1

olsrd/olsrd: $(wildcard olsrd/*/*.[cho])
	make -C olsrd OS=android DEBUG=0

olsrd/lib/httpinfo/olsrd_httpinfo.so.0.1: olsrd/olsrd $(wildcard olsrd/lib/*/*.[cho])
	make -C olsrd OS=android DEBUG=0 libs

clean:
	make -C olsrd OS=android libs_clean
	make -C olsrd OS=android clean
