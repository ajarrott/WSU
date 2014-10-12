// provided program to show us how the pipe works in c
// we were required to flip the reader and writer.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX 1024
main()
{

	char entry[MAX];
	int r, n, pid, i;
	int c, status;
	char var[4096];
	int stdoutc, stdinc, exitValue;
	int pd[2];

	r = pipe(pd);  // system call to create a pipe' pd[0]=READ, pd[1]=WRITE
	printf("pd=[%d, %d]\n", pd[0], pd[1]);
	pid = fork();
	
	if (pid){ // PARENT:
		printf("\nPARENT %d WAITS FOR CHILD %d TO DIE\n", getpid(),pid);
		printf("closing pipe\n");
		stdoutc = dup(1);		
		close(pd[0]);
		close(1);
		dup(pd[1]);
		
		printf("printing to stream\n");

		pid=waitpid(pid, &status, 0);
		
		close(pd[1]);
		dup(1);
		//open(pd[0]);
		//dup(stdoutc);


		printf("stdout restored!\n");
		printf("Parent %d: DEAD CHILD=%d, status=%04x\n", getpid(), pid, status);
	}
	else{// child:
		printf("this is child proc %d\n", getpid());
		stdinc = dup(0);
		//dup2(pd[0],0);
		close(pd[1]); // READER MUST close pd[1]
		close(0);  
		dup(pd[0]);   // replace 0 with pd[0]

		printf("from child\n");
		//while( ( c = getc(stdin)) != EOF) { putchar(c); }
		fgets(var, 4096, stdin);
		fputs(var, stdout);		
	
		close(pd[0]);
		dup(0);
		//dup(stdinc);
		printf("after while\n");
		exit (1);
	}
	printf("test stdin/out: ");
	var[0] = getchar();
	putchar(var[0]);
	//fputs(var, stdout);
}
