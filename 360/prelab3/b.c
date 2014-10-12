// program ran by a.out

#include <stdio.h>

main(int argc, char *argv[ ])
{
	int i = 0;
	printf("proc %d in b.out: argc=%d\n", getpid(), argc);

	while ( i < argc )
		printf("argv[%d] = %s\n", i, argv[i++]);

	printf("proc %d in b.out exit\n", getpid());
}
