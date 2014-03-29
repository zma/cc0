long a;
long b[10];

void func() {
	a = 2;
	b[3] = 5;
	commit;
}

void watcher_func() {
	a = 0;
	b[3] = 0;
	commitd;
}

void main() {
	runner func()
		using a, b[0,, 10];
	runner watcher_func()
		watching a, b[0,, 10];
	commit;
}
