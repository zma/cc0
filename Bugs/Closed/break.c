/*
 * BUG: break statement
 * 
 * DESCRIPTION
 *      The break statement has no effect
 * 
 * EXPECTED
 * 
 * ACTUAL
 *      seen no effect at all
 * 
 * CHANGELOG
 *      May 4, 2012          reported by Zhiqiang
 *      May 5, 2012          fixed by Xiang
 */

void main(){
    long i;

    for (i = 0; i < 10; i = i + 1) {
        *(long*)0x100000208 = i;
        if (i == 5) {
            break;
        }
    }
    commit;
}

