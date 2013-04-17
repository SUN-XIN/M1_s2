#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

#define N 5

int main(int argc, char **argv){
	int a,b,c;
	int i;
	for (i=0;i<N;i++) {
		srand(i);
		a=rand()%N;
		b=(a+1)% N;
		c=(a+N-1)% N;
		printf("a= %d, b=%d, c=%d\n",a,b,c);
	}
}
