// This program was to show us how to make our own char *argv[]
// then run our b.out progarm from within a.out

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pid;

char line[128];
char token[32][64];
char cwd[128], command[128];

main(int argc, char *argv[ ])
{
	//1. DRAW A DIAGRAM TO SHOW EXACTLY WHAT IS argv:


	// DEFINE myargv as AN ARRAY OF 32 char pointers:
	char* myargv[32];

	//2. Write code to           |-------- n=5--------| 
	//input a line, e.g.    b.out this is a test
	//tokenize line into inidividual strings as 
	//   token[0], token[1],....,token[n-1]

	int n = 0;
	char* temp;

	printf("enter a line: ");
	gets(line);

	temp = strtok(line, " ");
	while (temp)
	{
		strcpy (token[n], temp);
		n++;
		temp = strtok(0, " ");
	}
	
	//3. // Write code to let myargv[i] point to token[i], (0 <= i < n)
	//myargv[n] = 0;

	int i = 0;

	while ( i < n )
	{
		myargv[i] = token[i];
		i++;
	}
	myargv[n] = 0;

	/*i = 0;
	while (myargv[i])
	{
		printf("myargv[%d]: %s\n", i, myargv[i++]);
	}*/

	pid = getpid();
	printf("proc %d in a.out exec to b.out\n", pid);

	getcwd(cwd,128);     // cwd contains the pathname of CWD  
	printf("cwd = %s\n", cwd);

	// WRITE CODE to let command = CWD/b.out

	strcat(cwd, "/b.out");

	printf("cwd now = %s\n", cwd);

	strcpy(command, cwd);
	//command = cwd;
	//printf("command now = %s\n", command);

	execve(command, &myargv);

	printf("execve failed\n");
}
