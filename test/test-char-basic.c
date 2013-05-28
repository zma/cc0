char g_ch[1000];
char g_str[100];

void output_id()
{
    *(char*)0x100000208 = *(char*)0x100000120;
    return;
}

void output_char(char c)
{
    *(char *)(0x100000208) = c;
    return;
}

void output_str(char* s)
{
output_str_j1:
    if (*s != 0) {
        output_char(*s);
        s = s + 1;
        goto output_str_j1;
    }

    return;
}

void main()
{
    char tmp;

    char a;
    char b;

    g_ch[0] = (char)'a';
    g_ch[1] = (char)'b';
    g_ch[2] = (char)0;

    output_str(g_ch);

    tmp = (char)'a';

    a = (char)'a';
    b = (char)'b';

    if (a > b) {
        output_char((char)'g');
    }
    else {
        output_char((char)'l');
        output_char((char)'e');
    }

    commit;
}

