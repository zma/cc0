.PHONY: all clean

all: init on_finish output rand run

clean:
	rm -rf init.o on_finish.o output.o rand.o run.o init.c.tmp on_finish.c.tmp output.c.tmp rand.c.tmp run.c.tmp

init:
	$(CC0) $(KMEANS_EXP) $(CFLAGS) $(CC0DEBUG) -o init.o -c init.c

on_finish:
	$(CC0) $(KMEANS_EXP) $(CFLAGS) $(CC0DEBUG) -o on_finish.o -c on_finish.c

output:
	$(CC0) $(KMEANS_EXP) $(CFLAGS) $(CC0DEBUG) -o output.o -c output.c

rand:
	$(CC0) $(KMEANS_EXP) $(CFLAGS) $(CC0DEBUG) -o rand.o -c rand.c

run:
	$(CC0) $(KMEANS_EXP) $(CFLAGS) $(CC0DEBUG) -o run.o -c run.c
