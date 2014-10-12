// this was a program I wrote to emulate ls in linux

/***************************
 * Anthony Arrott 10853267 *
 *   CPTS 360 K.C. Wang    *
 ***************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<dirent.h>

#define BUFSIZE 1024
typedef unsigned short u16;
typedef unsigned long u32;
char buf[BUFSIZE];
char cwd[128];

//int  stat(const char *filename, struct stat *buf);


int getstats(char* dirName)
{
	struct stat st;	//stat struct
	struct tm *time; // time struct

	u16 i = 0; // to read unsigned 16 and 32 bit integers
	u32 j = 0;
	short linkFlag = 0; // check for if there is a link to print at the end
	char linkBuf[128]; // string buffers
	char timeBuf[128];

	while ( i < 128 ) linkBuf[i++] = 0; // get rid of garbage in linkBuf
	while ( j < 128 ) timeBuf[j++] = 0; // get rid of garbage in timeBuf
	i = j = 0;

	if ( dirName[0] == '/' )
	{
		strcpy(cwd, dirName);
	}
	else
	{
		getcwd(cwd, 128);	// get cwd and set it up for current file
		strcat(cwd, "/");
		strcat(cwd, dirName);
	}

	if ( lstat(cwd, &st) < 0 )	//read stats of file
	{
		printf("ERROR: could not open %s", cwd);
		exit(2);
	}

	/*printf(" File_type\t Permissions\t Uid\t Size\t Creation Time\n");
	printf("-----------\t-------------\t-----\t------\t---------------\n");*/
	i = st.st_mode;
	//printf("st_mode = %d\n", i);

	//printf("0120000 & i = %d\n", i & 0120000);
	if ( (i & 0100000) == 0100000 && (i & 0020000) != 0020000 ) printf("-");
	else if ( (i & 0040000) == 0040000) printf("d");
	else if ( (i & 0120000) == 0120000) 
	{
		printf("l");
		linkFlag = 1;
	}
	else printf("ERROR\n"); 

	//printf("  "); // for formatting

	// see if each type of user has read (r), write (w), or executable(x) access
	// if not place a - instead

	//owner
	if ( i & ( 1 << 8 ) ) putchar('r');
	else putchar('-');
	if ( i & ( 1 << 7 ) ) putchar('w');
	else putchar('-');
	if ( i & ( 1 << 6 ) ) putchar('x');
	else putchar('-');

	//group
	if ( i & ( 1 << 5 ) ) putchar('r');
	else putchar('-');
	if ( i & ( 1 << 4 ) ) putchar('w');
	else putchar('-');
	if ( i & ( 1 << 3 ) ) putchar('x');
	else putchar('-');

	//user
	if ( i & ( 1 << 2 ) ) putchar('r');
	else putchar('-');
	if ( i & ( 1 << 1 ) ) putchar('w');
	else putchar('-');
	if ( i & ( 1 << 0 ) ) putchar('x');
	else putchar('-');

	i = st.st_uid;		// print original creators UID integer
	printf(" %d", i);

	i = st.st_gid;
	printf(" %d", i);

	j = st.st_size;		// print size in bytes
	printf(" %d", i);

	time = localtime(&(st.st_mtime));	// get time 
	strftime(timeBuf, 128, "%b %d %H:%M", time); // put formatted time in a string
	printf(" %s", timeBuf);		// print time

	// if there is a link
	if ( linkFlag == 1 )
	{
		strcpy(buf, dirName); // put name of the link into buf
		strcat(buf, " -> ");  // concatenate arrow ( link points to actual file )
		readlink(cwd, &linkBuf, 128); // read link into linkBuf
		strcat(buf, linkBuf); // concatenate actual link name

		printf(" %s\n", buf); // print whole string
	}
	else
	{
		printf(" %s\n", dirName);
	}

}

int main(int argc, char* argv[])
{
	if ( argc < 2 )
	{
		printf("please enter a filename!\n"); // need to have a filename sent over
		exit(1);
	}
	
	getstats(argv[1]); // get stats of the file
}
