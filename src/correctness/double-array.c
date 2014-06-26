#include "stdio.h"

long array[1000];

void init() {
	double m[10];
	long tmp;

	//bug1: double array
	m[0]=1.5;
	output_q((long)m[0]);
	output_char(C_n);

	tmp = (long)m[0];
	output_q(tmp);
	output_char(C_n);

	commit;
}

void main(){
	runner init()
		using array[0,,1000];

	commit;
}
