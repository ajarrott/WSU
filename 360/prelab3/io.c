// program given to us by the teacher to understand file IO in linux

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

main()
{
	int fd;
	//L1
	printf("this is PROC %d printing to stdout\n", getpid());

	fd = open("myfile", O_WRONLY|O_CREAT, 0644);
	printf("fd = %d\n", fd);

	close(1);
	dup(fd);

	getchar();

	//L2
	printf("where does this line go?\n");
	//L3	
	printf("and this line?\n");
}
