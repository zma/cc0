.PHONY: all common_lib kmeans-stdin.o clean

all: common_lib kmeans-stdin.o
	$(CC0) $(CC0DEBUG) -o kmeans-stdin.bin kmeans-stdin.o ../kmeans_common/init.o ../kmeans_common/on_finish.o ../kmeans_common/output.o ../kmeans_common/rand.o ../kmeans_common/run.o

common_lib:
	cd ../kmeans_common && $(MAKE)

kmeans-stdin.o:
	$(CC0) $(KMEANS_EXP) $(CFLAGS) $(CC0DEBUG) -o kmeans-stdin.o -c kmeans-stdin.c

clean:
	rm -rf kmeans-stdin.o kmeans-stdin.bin kmeans-stdin.c.tmp
	cd ../kmeans_common && $(MAKE) clean
