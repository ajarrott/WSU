// This is called from the assembly code also contained in this folder
// it is a myprintf function that runs essentially the same way that printf
// in c runs

/***************************
 * Anthony Arrott 10853267 *
 *   CPTS 360 K.C. Wang    *
 ***************************/
#include <stdio.h>

typedef unsigned int u32;

#define BASE 10

char *table = "0123456789ABCDEF";

// helper function for octal printing
int rpo(u32 x)
{
	char c; // char to print
	if (x)
	{
		c = table[x % (BASE - 2)]; // get value in table for base 8
		rpo(x / (BASE - 2)); // recursive call to function
		putchar(c); // print chars in order
	}
} 

// helper function for hex printing
int rpx(u32 x)
{
	char c;
	if (x)
	{
		c = table[x % (BASE + 6)]; // get value in table for base 16
		rpx(x / (BASE + 6)); // recursive call to function
		putchar(c); // print chars in order
	}
} 

// helper function for unsigned printing
int rpu(u32 x)
{
	char c;
	if (x)
	{
		c = table[x % BASE]; // get value in table for base 10
		rpu(x / BASE); // recursive call to function
		putchar(c); // print chars in order
	}
} 

// print unsigned number
int printu(u32 x)
{
	if (x==0)
		putchar('0');
	else
		rpu(x);
	//putchar(' '); not necessary, adds a space that make output look awkward
}

// print integer
int printd(int x)
{
	if (x==0)
		putchar('0');
	else
	{
		if ( x < 0 )
		{
			putchar('-'); // print -
			x = -x; // make x positive
		}
		printu((u32) x); // now positive print unsigned int
	}
}

// print octal number
int printo(u32 x)
{
	putchar('0'); // octal numbers have a 0 preceeding the number
	if (x==0)
		putchar('0');
	else
		rpo((u32) x); // needs to be unsigned for octal
}

// print hex number
int printx(u32 x)
{
	putchar('0'); // hex numbers have 0x preceeding the number
	putchar('x');
	if (x==0)
		putchar('0');
	else
		rpx((u32) x); // needs to be unsigned for hex
}

// print string
int prints(char* c)
{
	while ( *c )
	{
		putchar(*c++); // print each character until null is read
	}
}

// myprintf function, prints string and any required % values
int myprintf(char *fmt, ...)
{
	char *cp = fmt; //char string pointer
	int *ip = (int*)get_ebp(); // get ebp pointer
	char* temp; // used specifically for strings
	
	/*                         3     2      1      0
	high address | d | c | b | a | *fmt | retPC | ebp | low address
	                           ^ start here */

	ip += 3; // get to first argument after fmt

	while(*cp) // while string
	{
		if ( *cp == '%' ) // exit character to print type
		{
			temp = (char*) *ip; // cast *ip as a char for string printing
			*cp++; // look at next char after %
			switch(*cp)
			{
				case 'c': // char case
					putchar(*ip);
					break;
				case 's': // string case
					prints(temp);
					break;
				case 'd': // int case
					printd(*ip);
					break;
				case 'x': // hex case
					printx(*ip);
					break;
				case 'o': // octal case
					printo(*ip);
					break;
				case 'u': // unsigned case
					printu(*ip);
					break;
				case '%': // % case, so %% prints %
					putchar('%');
					break;
			}
			*cp++; // next char needed after %X, where X is the case type
			*ip++; // point to next item on stack
		}
		else
		{
			putchar(*cp++);	// just print string
		}	
	}
}

// mymain
mymain(int argc, char *argv[ ], char *env[ ])
{
	int i; // needed for arg and env

	myprintf("in mymain(): argc=%d\n", argc);

  	for (i=0; i < argc; i++)
	{
      		myprintf("argv[%d] = %s\n", i, argv[i]);
	}
  
	myprintf("Enter a key to continue: ");
	getchar();
	// WRITE CODE TO PRINT THE env strings
	i = 0; // reset i to 0
	while ( *env )
	{
		myprintf("env[%d] = %s\n", i++, *env++); // print env strings
	}

	myprintf("---------- testing YOUR myprintf() ---------\n");
	myprintf("this is a test\n");
	myprintf("testing a=%d b=%x c=%c s=%s\n", 123, 123, 'a', "testing");
	myprintf("string=%s, a=%d  b=%u  c=%o  d=%x\n",
		 "testing string", -1024, 1024, 1024, 1024);
	myprintf("mymain() return to main() in assembly\n"); 
}
