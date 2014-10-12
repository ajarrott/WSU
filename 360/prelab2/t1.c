#include<stdio.h>
int g;

main()                              
{                                   
	int a,b,c; 
	a = 1; b = 2; 
	c = a + b;
	printf("c=%d\n", c);
}     

// g - UNITIALIZED GLOBAL
// a, b, c

//g is in BSS
//a, b, c are in data
