standalone long data[4194304];

// for testing create snapshot speed
// snapshot size is number of elements in data * 8
//  32M:    4194304
//  128M:   16777216
//  512M:   67108864
//  2G:     268435456
//  8G:     1073741824

void null_runner()
{
    *(long*)0x100000208 = *(long*)0x100000120;
    commit;
}

void main(){
    *(long*)0x100000208 = *(long*)0x100000120;
    runner {null_runner(); }
	commit;
}

