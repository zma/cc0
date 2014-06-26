.PHONY: all kmeans_stdin kmeans_stdin_to_pmem kmeans_from_pmem clean 

export KMEANS_EXP=-DTINY_EXP

all: kmeans_stdin kmeans_stdin_to_pmem kmeans_from_pmem
	echo -e "\nYou have successfully built kmeans\n"

kmeans_stdin:
	cd kmeans_stdin && $(MAKE)

kmeans_stdin_to_pmem:
	cd kmeans_stdin_to_pmem && $(MAKE)

kmeans_from_pmem:
	cd kmeans_from_pmem && $(MAKE)

clean:
	cd kmeans_stdin && $(MAKE) clean
	cd kmeans_stdin_to_pmem && $(MAKE) clean
	cd kmeans_from_pmem && $(MAKE) clean
