TARGET = mc_kernel_loader

all:
	xlc_r -q64 -w -c mc_kernel_loader.c
	xlc_r -q64 -w -c mc_kernel_loader_util.c
	xlc_r -q64 -o $(TARGET) mc_kernel_loader.o mc_kernel_loader_util.o

install:
	cp $(TARGET) ../bin

clean:
	rm -f *.o $(TARGET) 2> /dev/null
