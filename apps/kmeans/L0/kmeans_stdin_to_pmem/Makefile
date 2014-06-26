.PHONY: all common_lib kmeans-stdin-to-pmem.o clean

export CFLAGS=-DFROM_PMEM

all: common_lib kmeans-stdin-to-pmem.o
	$(CC0) $(CC0DEBUG) -o kmeans-stdin-to-pmem.bin kmeans-stdin-to-pmem.o ../kmeans_common/init.o ../kmeans_common/on_finish.o ../kmeans_common/output.o ../kmeans_common/rand.o ../kmeans_common/run.o

common_lib:
	cd ../kmeans_common && $(MAKE)

kmeans-stdin-to-pmem.o:
	$(CC0) $(KMEANS_EXP) $(CFLAGS) $(CC0DEBUG) -o kmeans-stdin-to-pmem.o -c kmeans-stdin-to-pmem.c

clean:
	rm -rf kmeans-stdin-to-pmem.o kmeans-stdin-to-pmem.bin kmeans-stdin-to-pmem.c.tmp
	cd ../kmeans_common && $(MAKE) clean
