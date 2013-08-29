/*
 * BUG: standalone
 * 
 * DESCRIPTION
 *      The 'standalone' modifier has no effect on arrays.
 * 
 * EXPECTED
 *      The variable e should occupy seperate memory pages and align at the page boundary. Which means, both e and f should start at a new page.
 * 
 * ACTUAL
 *      The variable e is allocated in the same page as a, and f is allocated immediately after e.
 * 
 * CHANGELOG
 *      April 11, 2012          reported by Zhonghua
 *      April 15, 2012          fixed by Xiang
 *      April 16, 2012          verified by Zhiqiang
 *      April 17, 2012          reopened by Zhiang, after the new runner grammar branch merged
 *      April 18, 2012          fixed by Xiang
 */

long a;
standalone long e[512]; 
long f;

void main(){
        commit;
}

