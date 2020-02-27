SUBDIRS = mc_kernel mc_kernel_loader

all:
	@for DIRS in $(SUBDIRS) ; \
	do \
		(cd $$DIRS && make && make install) \
	done

clean:
	@for DIRS in $(SUBDIRS); \
	do \
		(cd $$DIRS && make clean) \
	done
