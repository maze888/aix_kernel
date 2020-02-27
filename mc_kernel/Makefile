TARGET = mc_kernel

all:
	xlc_r -q64 -w -D_KERNEL -o mc_kernel.o -c mc_kernel.c
	ld -b64 -o $(TARGET) mc_kernel.o -e mc_kernel_init -bI:/usr/lib/kernex.exp -lsys -l csys

install:
	cp $(TARGET) ../bin

clean:
	rm -f *.o $(TARGET) 2> /dev/null
