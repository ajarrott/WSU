// program provided by teacher so we can understand how forking in c works

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main()
{
	int pid;
	printf("THIS IS %d  MY PARENT=%d\n", getpid(), getppid());
	      // WRITE CODE TO PRINT process pid and its ppid ); 

	pid = fork();   // fork syscall; parent returns child pid, 
		      //               child returns 0
	if (pid < 0){   // fork() may fail. e.g. no more PROC in Kernel
	  perror("fork faild");
	  exit(1);
	}
	 // PARENT EXECUTES THIS PART 
	if (pid){
	  printf("THIS IS %d  MY PARENT=%d\n", pid, getppid());
	} 
	 // child executes this part
	else{
	  printf("I am    %d  my parent=%d\n", getpid(), getppid());
	       // WRITE CODE TO PRINT pid and ppid);
	}
}
