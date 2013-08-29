char g_ch[1000];
char g_str[100];

long strcmp(char* s1, char* s2)
{
strcmp_j1:

    if (*s1 == '\0') {
        if (*s2 == '\0') {
            return 0;
        }
        else {
            // bug here: -1 causes error
            // return -1;
            return 1;
        }
    }
    if (*s1 > *s2) {
        return; // 1;
    }
    
    if (*s1 < *s2) {
        return ; // -1
    }

    s1 = s1 + 1;
    s2 = s2 + 1;

    goto strcmp_j1;
}

long char_cmp(char a, char b)
{
    if (a < b) {
        return ; //-1;
    }
    else if (a == b) {
        return ; //0;
    }
    else {
        return ; //1;
    }
}

void main()
{
    long tmp;

    g_ch[0] = 'a';
    g_ch[1] = 't';
    g_ch[2] = '\0';

    g_str[0] = g_ch[0];
    g_str[1] = g_ch[1];
    g_str[2] = g_ch[2];

    tmp = 0;

    tmp = char_cmp('a', 'b');

    commit;
}

