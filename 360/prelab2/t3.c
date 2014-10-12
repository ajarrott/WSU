#include<stdio.h>
int g[10000];

main()                              
{                                   
	static int a,b,c; 
	a = 1; b = 2; 
	c = a + b;
	printf("c=%d\n", c);
}

// g[10000], a, b are in DATA
// c is in data
