#define A_SIZE 1000
standalone long a[A_SIZE];
standalone long b[A_SIZE];

void add_runner(long start, long *p, long *q, long len) {
	long i;

	for (i = start; i < start + len; i = i + 1) {
		p[i] = p[i] + q[i];
	}
	commit;
}

void start() {
	long i;

	for (i = 0; i < A_SIZE; i = i + 1) {
		a[i] = 0;
		b[i] = 1;
	}

	for (i = 0; i < A_SIZE; i = i + 100) {
		runner add_runner(i, &a[0], &b[0], 100)
			using a[i,,i+100], b[i,,i+100];
	}
	commit;
}

void main() {
	runner start()
		using a[0,,A_SIZE], b[0,,A_SIZE];
	commit;
}
