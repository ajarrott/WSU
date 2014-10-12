// this was a program that we used so we could learn about the locations of
// local variables on the stack

/***************************
 * Anthony Arrott 10853267 *
 *    CPTS360 K.C. Wang    *
 ***************************/

#include <stdio.h>

//nothing changed here
int A(int x, int y)
{
	int d,e,f;
	printf("enter A\n");
	// PRINT ADDRESS OF d, e, f
	printf("d: %x\ne: %x\nf: %x\n", &d, &e, &f);

	d=3; e=4; f=5;
	B(d,e);
	printf("exit A\n");
}

//nothing changed here
int B(int x, int y)
{
	int g,h,i;
	printf("enter B\n");
	// PRINT ADDRESS OF g,h,i
	printf("g: %x\nh: %x\ni: %x\n", &g, &h, &i);

	g=6; h=7; i=8;
	C(g,h);
	printf("exit B\n");
}

int *p, ebp;

// added functionality to get ebp, print it, print list of 100 items from
// &w, and finally print out the ebp linked list
int C(int x, int y)
{
	int u, v, w;
	printf("enter C\n");
	// PRINT ADDRESS OF u,v,w;
	printf("u: %x\nv: %x\nw: %x\n", &u, &v, &w);

	u=9; v=10; w=11;

	//1
	ebp = get_ebp(); // get ebp
	printf("ebp: %x\n", ebp); // print it

	//2
	p = &u; // set p to &w, last variable declared

	int i = 0; // needed to only print 100 items
	
	printf("address\t\tcontents\n");
	while( i < 100 )
	{
		printf("%x\t%x\n", p, *p); // print address and value in address
		p++;			   // increment pointer
		i++;			   // increment i
	}

	printf("-----EBP Linked List-----\n");

	//4
	p = ebp; // set p to address of ebp
	while (*p)
	{
		printf("EBP: %x -> %x\n", p, *p );
		p = *p; // set p to next address in linked list
	}
	printf("ebp: %x -> %x\n", p, *p ); // print last item in list to point to null
	printf("exit C\n");
}

//nothing changed here
main(int argc, char *argv[], char *env[])
{
	int a,b,c;
	printf("enter main\n");

	printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
	printf("&a=%x &b=%x &c=%x\n", &a, &b, &c);

	a=1; b=2; c=3;
	A(a,b);
	printf("exit main\n");
}

