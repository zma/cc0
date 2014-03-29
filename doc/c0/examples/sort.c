#define A_SIZE 100

long a[A_SIZE];

long partition(long *v, long length) {
	// implementation is omitted
	return 0;
}

void qsort(long *v, long start, long length) {
	long ipivot; 
	
	if (length < 2)
		commit;

	ipivot = partition(&v[start], length);

	runner qsort(v, start, ipivot - start)
		using v[start,,ipivot];
	runner qsort(v, ipivot + 1, length - ipivot - 1)
		using v[ipivot + 1 ,, start + length - 1];

	commit;
}

void start() {
	long i;
	long rand;
	
	rand = 3141592621;

	for (i = 0; i < A_SIZE; i = i + 1) {
		// pseudo rand generator
		rand = rand * rand;
		a[i] = rand;
	}

	runner qsort(&a[0], 0, A_SIZE)
		using a[0 ,, A_SIZE];
	commit;
}

void main() {
	runner start()
		using a[0 ,, A_SIZE];
	commit;
}
