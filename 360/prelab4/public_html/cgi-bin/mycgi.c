// c file utilized by the html file, it is compiled with the
// util.o file provided by my teacher.  This program allowed
// a user to create/remove directories, remove files, copy files
// print file contents, and finally list files in the current directory.

// for this to work property need to chmod u+s mycgi to ensure  the program
// functions propertly.

/***************************
 * Anthony Arrott 10853267 *
 *   CPTS 360 K.C. Wang    *
 ***************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h> 

#define MAX 10000
#define BLKSIZE 4096

typedef struct {
    char *name;
    char *value;
} ENTRY;

ENTRY entry[MAX];

// list files
list(char *cwd)
{
	DIR *dp;		// directory pointer
	struct dirent *ep;	// entry pointer
	char dir[128];		// dirname (if anything other than cwd
	int i = 0;

	if ( strcmp(entry[1].value, "") == 0 )
	{
		dp = opendir(cwd);	// open cwd
		if ( dp == 0 )
		{
			printf("ERROR: could not open cwd <p>");
			return;
		}
	}
	else // only open from cwd on, user should not access anything below this directory
	{
		strcpy(dir, cwd);
		strcat(dir, "/");
		strcat(dir, entry[1].value);

		dp = opendir(dir);
		if ( dp == 0 )
		{
			printf("ERROR: could not open %s<p>", dir);
			return;
		}
	}

	while ( ep = readdir(dp) ) // read dir
	{
		if ( strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, ".." ) == 0 ) {} // do nothing
		else
		{
			printf("%s<p>", ep->d_name); // print dir name
		}
	}
	
	closedir(dp);	// close dir*/
}

// remove file
rfile()
{
	if ( strcmp(entry[1].value, "") == 0)	// empty string case
	{
		printf("no file name specified in filename1 to delete<p>");
		return;
	}

	if ( strcmp(entry[1].value, "mycgi") == 0)
	{
		printf("Don't break my website!<p>");
		return;
	}
	if (remove(entry[1].value) == 0) // removed file == 0 is success
		printf("SUCCESS!<p>");
	else
		printf("Failed<p>");
}

// remove dir
rdir()
{
	if ( strcmp(entry[1].value, "") == 0) // empty string case
	{
		printf("no directory name specified in filename1 to delete<p>");
		return;
	}

	if (rmdir(entry[1].value) == 0) // removed dir == 0 is success
		printf("SUCCESS!<p>");
	else
		printf("Failed<p>");
}

// make dir
mdir()
{	
	if ( strcmp(entry[1].value, "") == 0)	// empty string case
	{
		printf("no filename1 specified to make a directory from<p>");
		return;
	}
	
	if (mkdir(entry[1].value, 0777) == 0) // makedir == 0 is a success
		printf("SUCCESS!<p>");
	else
		printf("Failed<p>");
}

// copy from file1 to file2
copy()
{
	int n, total = 0;
	int fd, gd;
	char buf[BLKSIZE];

	if ( strcmp(entry[1].value, "") == 0) 	// empty string case
	{
		printf("no filename1 specified<p>");
		return;
	}
	if ( strcmp(entry[2].value, "") == 0)	// empty string case
	{
		printf("no filename2 specified<p>");
		return;
	}
	
	fd = open(entry[1].value, O_RDONLY);	// open file
	if (fd < 0)	// failed to open file
	{
		printf("%s does not exist to copy from<p>", entry[1].value);
		return;
	}

	gd = open(entry[2].value, O_WRONLY|O_CREAT, 0644); // open second file, or create it
	if (gd < 0)
	{
		printf("could not create %s<p>", entry[2].value);
		return;
	}

	while (n=read(fd, buf, BLKSIZE)) // read blocks and write them
	{
		write(gd, buf, n);
		total +=n;
	}
	printf("total=%d<p>", total);
	printf("SUCCESS!<p>");

	close(fd); close(gd);
}

// print contents of file
cat()
{
	int fd, n, i;
	char buf[1024];

	if ( strcmp(entry[1].value, "") == 0) // make sure string isn't empty
	{
		printf("no file specified<p>");
		return;
	}
	
	fd = open(entry[1].value, O_RDONLY); // open file

	if (fd < 0) 
	{
		printf("no file exists with the name %s<p>", entry[1].value);
		return;
	}

	while (n = read(fd, buf, 1024))	// read file into buffer
	{
		for(i=0; i<n; i++)
			putchar(buf[i]); // write to browser
	}
	printf("<p>");
}

//main function
main(int argc, char *argv[]) 
{
	int i, m, r;
	char cwd[128];

	m = getinputs();    // get user inputs name=value into entry[ ]
	getcwd(cwd, 128);   // get CWD pathname

	printf("Content-type: text/html\n\n");
	printf("<p>pid=%d uid=%d cwd=%s\n", getpid(), getuid(), cwd);

	printf("<H1>Echo Your Inputs</H1>");
	printf("You submitted the following name/value pairs:<p>");

	for(i=0; i <= m; i++)
		printf("%s = %s<p>", entry[i].name, entry[i].value);
	printf("<p>");

	if ( strcmp(entry[0].value, "mkdir") == 0 ) //mkdir
		mdir();
	else if ( strcmp(entry[0].value, "rmdir") == 0 ) //rmdir
		rdir();
	else if ( strcmp(entry[0].value, "rm") == 0 ) //rm
		rfile(entry[1].value);
	else if ( strcmp(entry[0].value, "cat") == 0 ) //cat
		cat();
	else if ( strcmp(entry[0].value, "cp") == 0 ) //cp
		copy();
	else if ( strcmp(entry[0].value, "ls") == 0 ) //ls
		list(cwd);
	else 
		printf("%s is not a valid command<p>", entry[0].value);

	// create a FORM webpage for user to submit again 
	printf("</title>");
	printf("</head>");
	printf("<body bgcolor=\"#FFFF66\" link=\"#330033\" leftmargin=8 topmargin=8");
	printf("<p>------------------ DO IT AGAIN ----------------\n");

	//printf("<FORM METHOD=\"POST\" ACTION=\"http://cs360.eecs.wsu.edu/~joecamo/cgi-bin/mycgi\">");

	//------ NOTE : CHANGE ACTION to YOUR login name ----------------------------
	printf("<FORM METHOD=\"POST\" ACTION=\"http://cs360.eecs.wsu.edu/~joecamo/cgi-bin/mycgi\">");

	printf("Enter command : <INPUT NAME=\"command\"> <P>");
	printf("Enter filename1: <INPUT NAME=\"filename1\"> <P>");
	printf("Enter filename2: <INPUT NAME=\"filename2\"> <P>");
	printf("Submit command: <INPUT TYPE=\"submit\" VALUE=\"Click to Submit\"><P>");
	printf("</form>");
	printf("------------------------------------------------<p>");

	printf("</body>");
	printf("</html>");
}
