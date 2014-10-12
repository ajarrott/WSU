// this program emulates the shell using execve and by forking
// child processes to run program designated by the user

/***************************
 * Anthony Arrott 10853267 *
 *   K.C. Wang CptS 360    *
 ***************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//definitions
#define MAX 256

#define READ 0
#define WRITE 1
#define APPEND 2
#define NONE 3
#define ERROR 4

#define MORE 5
#define NOMORE 6

//globals
char *myargv[MAX];
char path[MAX][MAX];
char fn[128];
char pipehead[MAX];
char pipetail[MAX];
int pd[2];

// cd changes directory to home if nothing is entered or to a relative or absolute path
int cd(char* env[])
{
	char temp[MAX];
	char dir[MAX]; 
	char *home;
	int i = 0;

	if ( myargv[1] ) // if an argument exists
	{
		if(myargv[1][0] == '/') // absolute case
		{
			if ( chdir(myargv[1]) < 0 )
			{
				fprintf(stderr, "could not change directory %s\n", myargv[1]);
			}
		}
		else // relative case
		{
			getcwd(dir, MAX); // get current directory
			strcat(dir, "/"); // concatenate a /
			strcat(dir, myargv[1]); // concatenate the argument

			if ( chdir(dir) < 0 ) // change dir
			{
				printf("could not change directory %s\n", dir);
			}
		}
	}
	else // no argument
	{
		while(strncmp(env[i],"HOME=", 5)) // find HOME= in env
		{
			i++;
		}

		strcpy(temp, env[i]); // copy env to temp
		home = strtok(temp, "HOME="); // get rid of HOME=, store in home

		if ( chdir(home) < 0 ) // change dir to $HOME
		{
			printf("could not change directory! no dir specified case\n");
		}
	}
}

// setmyargs creates myargv
int setmyargs(char *entry)
{
	int i = 1;
	char* cmd;

	cmd = strtok(entry, " "); // first item is the command
	myargv[0] = cmd;
	myargv[i] = strtok(NULL, " "); // everything else is an argument

	while ( myargv[i] ) // stops when strtok returns null
	{
		i++;
		myargv[i] = strtok(NULL, " ");
	}

	//myargv[i+1] = 0; // last argument is null
}

// setpipe sets the pipehead and pipetail global variables
int setpipe(char *entry)
{
	char *cmd;
	if (entry)
	{
		cmd = strtok(entry, "|"); // put head in pipehead
		strcpy(pipehead, cmd);

		cmd = strtok(NULL, ""); // put the rest in pipetail
		if ( cmd ) // make sure cmd is not null
		{
			strcpy(pipetail, cmd); // copy the rest of the string into pipetail
			return MORE;
		}
		else // otherwise make sure pipetail is null
		{
			pipetail[0] = 0;
			return NOMORE;
		}
	}
	else
	{
		return NONE;
	}
}

// for recursion in mypipe(), checks to see if there are anymore pipes in the entry
int checkpipe(char *entry)
{
	int i = 0;
	while ( entry[i] )
	{
		if ( entry[i] == '|')
			return MORE;
		i++;
	}
	return NOMORE;
}

// IO is used by a switch in scall to find if there is any IO redirection submitted by the user
// it returns the type for the switch to open the file as
int IO()
{
	int i = 0;
	while ( myargv[i] )	// while arguments exist
	{
		if(strcmp(myargv[i], "<") == 0) // input
		{
			if ( myargv[i+1] )	// see if another argument exists
			{
				strcpy(fp, myargv[i+1]); // next item should be a filename
			}
			else
			{
				fprintf(stderr, "no file for input specified\n");
				return ERROR;
			}

			myargv[i] = 0; // get rid of <
			myargv[i+1] = // get rid of filename

			return READ;

		}
		else if ( ( strcmp(myargv[i], ">") == 0 ) && strcmp(myargv[i], ">>") != 0) // output
		{
			if ( myargv[i+1] )	// see if another argument exists
			{
				strcpy(fp, myargv[i+1]); // next item should be a filename
			}
			else
			{
				fprintf(stderr, "no file for output specified\n");
				return ERROR;
			}

			myargv[i] = 0; // get rid of >
			myargv[i+1] = 0; // get rid of filename

			return WRITE;
		}
		else if ( strcmp(myargv[i], ">>") == 0) // append
		{
			//printf("append\n");
			if ( myargv[i+1] )	// see if another argument exists
			{
				strcpy(fp, myargv[i+1]); // next argument should be a filename
			}
			else
			{
				fprintf(stderr, "no file for append specified\n");
				return ERROR;
			}

			myargv[i] = 0; // get rid of >>
			myargv[i+1] = 0; // get rid of filename

			return APPEND;	// let switch know type is append
		}
		i++;
	}
	return NONE;	// did not find any IO redirection in the string
}

// getpath finds the path from *env[] and puts it into the path[MAX] global variable without PATH= or :
int getpath(char* env[])
{
	int i = 0;
	char temp[MAX];
	char *tok;

	while(strncmp(env[i],"PATH=", 5)) { i++; } // find PATH= in env

	strcpy(temp, env[i]); 		// copy env to temp to prevent messing up env

	tok = strtok(temp, "PATH="); 	// get PATH out of temp
	tok = strtok(tok, ":");		// get first token before :
	strcpy(path[0], tok);		// copy first token into path[0] gbl

	i = 0;				// need i to be 0 again

	while ( 1 )
	{
		i++;
		tok = strtok(NULL, ":");	// get next token
		if ( !tok )			// if null break
		{
			path[i][0] = 0;		// set next path var to null
			break;
		}
		strcpy(path[i], tok);		// copy into path
	}
}

// my system call function, uses execve to execute a process
int scall(char* argv[], char* env[])
{
	int i = 0;

	char filename[MAX];
	char *cmd;	

	if ( strncmp(myargv[0], "/", 1) == 0) // if /processname then it is an absolute case, run process
	{
		strcpy(filename, myargv[0]);
		if ( execve(filename, myargv, env) < 0 ) // if it does not execute print message for user
			fprintf(stderr, "could not execute %s\n", myargv[0]);
	}
	else //otherwise possible system command
	{	
		while( strcmp(path[i], "") ) // while path has an entry
		{
			strcpy(filename, path[i]); 	// copy path[i] to filename
			strcat(filename, "/");		// concatenate /
			strcat(filename, myargv[0]);	// concatenate command name

			switch( IO() ) // see if there is input/output/append to the next file
			{
				case READ:	// read case
					close(0);	// close stdin
					open(fp, O_RDONLY);	// open fp for reading
					break;
				case WRITE:	// write case 
					close(1);
					open(fp, O_WRONLY|O_CREAT, 0744); // open fp for writing or create it
					break;
				case APPEND:	// append case
					close(1);
					open(fp, O_WRONLY|O_APPEND, 0744); // open fp for writing or appending
					break;
				case NONE:	// no IO redirectoin
					break;				
				case ERROR:	// error, return, do not do anything else
					return;
			}
			execve(filename, myargv, env); // execute filename
			i++;
			
		}
		fprintf(stderr, "Command not found\n");
	}
}

// runs command in myargv[0] with arguments in myargv[>0]
int runcmd(char* env[])
{
	int pid, status;
	int i = 0;

	getpath(env); // getpath from env

	if (strncmp(myargv[0], "exit", 4) == 0) // exits program
	{
		exit(1);
	}
	else if ( strncmp(myargv[0], "quit", 4) == 0) // added because i kept typing quit instead of exit
	{
		exit(1);
	}
	else if ( strncmp(myargv[0], "cd", 2) == 0) // change directory
	{
		cd(env);
	}
	else
	{
		pid = fork(); // fork a child to run a process

		if (pid < 0){ perror("fork failed)"); exit(1); }
 
		else if (pid) // parent
		{
			wait(pid, &status); // wait for return;
		}
		else
		{
			scall(myargv, env); // system call myargv
			while ( strcmp(myargv[i], "") ) { myargv[i] = 0; i++; } // clear myargv
			exit(1); // child dies
		}
	}			
}

// runs pipe writer first then pipe reader so output is correct
// toadd: recursion for multiple pipes
mypipe(char* env[])
{
	int r, pid, status;
	r = pipe(pd);  // system call to create a pipe' pd[0]=READ, pd[1]=WRITE

	pid = fork();
	if ( pid < 0 ) // make sure fork succeeded
	{
		fprintf(stderr, "fork failed!\n");
	}
	else if (pid)// parent
	{
		close(pd[1]); // close write end				
		close(0);     // close 0
		dup(pd[0]);   // replace 0 with pd[0]
		
		setmyargs(pipetail);		// set args for end of pipe
		waitpid(pid, &status, 0);	// wait for child to die before running parents command
		runcmd(env);	// run command
	}
	else	// child
	{
		close(pd[0]); 	// close read end			
		close(1);	// close 1
		dup(pd[1]);	// replace 1 with pd[1]

		setmyargs(pipehead); 	// set arguments for pipehead
		runcmd(env);		// run command
		
		exit(2); 
	}
	close(pd[0]); close(pd[1]); // close both pipes
}

// main function of the program
int main(int argc, char* argv[], char* env[])
{
	char entry[MAX]; // user input
	int pid, status; // pid for fork and status for wait

	while(1)
	{
		printf("enter a command: "); 
		fgets(entry, sizeof(entry), stdin); // get input from user

		if ( entry[0] == '\n' ) continue;

		entry[strlen(entry)-1] = 0; // set newline character read to null character

		switch(setpipe(entry))
		{
			case MORE:		// more items in the pipe
				pid = fork();
				if (pid)	// PARENT
				{ 
					wait(pid, &status, 0);
				}
				else 
				{
					mypipe(env);
					exit(2);
				}
				break;
			case NOMORE:		// no more items in the pipe
				setmyargs(pipehead); 	// set args for pipehead
				runcmd(env);		// runcmd
				break;
			case NONE:		// no items in the pipe
				break;
		}

		/*if ( pipetail[0] != 0 )
		{
			strcpy(entry, pipetail); // copy tail into entry
		}*/
	}
}
