struct Foo {
    int a;
    int *b;
};

struct {
    int (*func)(int, int);
    Foo foo;
} complex_var;
