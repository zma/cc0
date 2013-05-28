long a;
long b;
standalone long c; //should be 4K aligned.
long d;  //should be 4K aligned, because c should occupy the whole page
standalone long e[512]; //should be 4K aligned.
long f; //should be 4K aligned, due to e

void main(){
        // the variable "page" should be 4K aligned.
        commit;
}

