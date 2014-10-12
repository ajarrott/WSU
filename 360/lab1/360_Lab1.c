#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node  
{
	char name[64];		// name of dir/file
	char type;		// either f (file), or d (directory)
	struct node *childPtr;		// child pointer
	struct node *siblingPtr;	// sibling pointer, alphabatized linked list on level
	struct node *parentPtr;		// parent pointer
}NODE;

NODE *root, *cwd;		// root and CWD pointers
char line[128];			
char command[16], pathname[64]; // user input line
char dirname[64], basename[64]; // string holders
char commands[16][16];		// leave enough room for command

void printContents( NODE* n )
{
	printf("name: %s\n", n->name);
	printf("type: %c\n", n->type);
	printf("childPtr: %lu\n", (long)n->childPtr);
	printf("siblingPtr: %lu\n", (long)n->siblingPtr);
	printf("parentPtr: %lu\n", (long)n->parentPtr);
}

// create empty dir node
NODE* createDir(char* dirName)
{
	NODE* newDir = (NODE*)malloc(sizeof(NODE));
	strcpy(newDir->name, dirName);
	newDir->type = 'D';
	// other pointers null
	newDir->childPtr = 0;
	newDir->siblingPtr = 0;
	newDir->parentPtr = 0;

	return newDir;
}

// create empty file node
NODE* createFile(char* fileName)
{
	NODE* newFile = (NODE*)malloc(sizeof(NODE));
	strcpy(newFile->name, fileName);
	newFile->type = 'F';
	// other pointers null
	newFile->childPtr = 0;
	newFile->siblingPtr = 0;
	newFile->parentPtr = 0;

	return newFile;
}

int reverseStr ( char *cd )
{
	char temp;
	char* cdEnd = cd + strlen(cd) - 1; // get to end of cd without null pointer 
	//printf("strlen: %d\n", (int)strlen(cd));
	//printf("cd: %s\n", cd);
	//printf("rStr *cdEnd: %c\n", *cdEnd);
	while ( cdEnd > cd )
	{
		temp = *cd;
		*cd = *cdEnd;
		*cdEnd = temp;
		
		cd++;
		cdEnd--;
		//printf("cd now: %s\n", cd);
		//printf("cdEnd now: %s\n", cdEnd);
	}
	//cd = 0;
	//printf("cd: %s\ncdEnd: %s\n", cd, cdEnd);
	//return cd;
	//free(temp);
	//free(cdEnd);
}


// print working directory backwards by working up list until parentPtr == null
// *c is for returning to print into save, otherwise null
char *pwd( char* c, NODE* n )
{
	char wd[128];
	char *cd = malloc(sizeof(char[64]));

	NODE* f;
	if ( n ) { f = n; }
	else { f = cwd; }
	if ( strcmp(f->name, "/") == 0 )
	{
		if ( n == 0 ) { printf("/\n"); }
		else { strcpy(c, "/"); }
		free(cd);
		return c;
	}

	//printf("f->name: %s\n", f->name);
	//printf("f->parentPtr: %lu\n", (long)f->parentPtr);
	int i = 0, j = 0;
	
	//printf("before while\n");
	while( f->parentPtr )
	{	
		//printf("in while\n");
		
		if ( f->name )
		{
			strcpy(cd, f->name);
			//printf("b4cd: %s\n", cd);
			reverseStr(cd);
			//printf("aftercd: %s\n", cd);
			
		}
		//printf("*cd: %c\n", cd[i]);
		while ( cd[i] )
		{
			wd[j] = cd[i];
			//cd[i] = 0;
			i++; j++;
		}
		wd[j] = '/';
		j++;
		i = 0;
		//printf("wd: %s\n", wd);
		f = f->parentPtr;
		//printf("f->name: %s\n", f->name);
		//getchar();
	}
	wd[j] = 0;
	reverseStr(wd);
	if ( n == 0 )
	{
		printf("%s\n", wd);
		free(cd);
	}
	else
	{
		strcpy(c, wd);
		free (cd);
		return c;
	}
}

int ls (char* pn)
{
	//debug("in ls");
	NODE* f;
	if ( pn == 0 )
	{
		//printf("setting f to root\n");
		f = root;
	}
	else
	{	
		//printf("setting f to cwd\n");
		f = cwd;
	}
	//printf("set correctly!\n");
		
	if ( f->childPtr == 0 ) { printf("No files in the current directory."); }
	else
	{
		//printf("in ls else\n");
		f = f->childPtr;
		
		printf("[%c]%s\t", f->type, f->name);
		//printContents(f);

		while ( f->siblingPtr )
		{
			f = f->siblingPtr;
			printf("[%c]%s\t", f->type, f->name);
			//printContents(f);
		}
	}
	printf("\n");
}

// insert in child of f, bn, type
int insNode ( NODE* f, char* bn, char type )
{
	NODE *newNode;
	int i;

	while(1)
	{
		if ( f->childPtr == 0 )
		{
			if ( type == 'D' )
			{
				newNode = createDir(bn);
				f->childPtr = newNode;
				newNode->parentPtr = f;
				//printf("1made dir %s in %s\n", bn, f->name);
				return;
			}
			else
			{
				newNode = createFile(bn);
				newNode->parentPtr = f;
				f->childPtr = newNode;
				//printf("1made file %s in %s\n", bn, f->name);
				return;
			}
		}
		else // child exists, make sure its not a copy & traverse siblings
		{	
			f = f->childPtr;
			//printf("f->name: %s\n", f->name);
			//printf("bn: %s\n", bn);
			while(1)
			{
				if ( strcmp(bn, f->name) == 0 )
				{
					//printf("type: %c\n", type);
					//printf("f->type: %c\n", f->type);
					if ( type == f->type )
					{
						printf("Cannot create ");
						if ( type == 'F') { printf("file "); }
						else { printf("directory "); }
						printf("because it already exists.\n");
						return;
					}
					else // sibling is not type we are looking for, so ok to include duplicate name of 1 file and 1 dir
					{
						if ( f->siblingPtr != 0 ) { f = f->siblingPtr; }
						else
						{
							if ( type == 'D' )
							{
								newNode = createDir(bn);
								newNode->parentPtr = f->parentPtr;
								f->siblingPtr = newNode;
								//printf("2made dir %s in %s\n", bn, f->parentPtr->name);
								return;
							}
							else
							{
								newNode = createFile(bn);
								newNode->parentPtr = f->parentPtr;
								f->siblingPtr = newNode;
								//printf("2made file %s in %s\n", bn, f->parentPtr->name);
								return;
							}
						}
					}
				}
				else if ( f->siblingPtr != 0 ) { f = f->siblingPtr; } // go to next sibling ptr
				else if ( f->siblingPtr == 0 ) // ok to insert
				{
					if ( type == 'D' )
					{
						newNode = createDir(bn);
						newNode->parentPtr = f->parentPtr;
						f->siblingPtr = newNode;
						//printf("3made dir %s in %s\n", bn, f->parentPtr->name);
						return;
					}
					else
					{
						newNode = createFile(bn);
						newNode->parentPtr = f->parentPtr;
						f->siblingPtr = newNode;
						//printf("3made file %s in %s\n", bn, f->parentPtr->name);
						return;
					}
				}
				else
				{
					//should not get here, and I don't
					//printf("u wot m8!\n");
					break;
				}
			}			
		}
	}
}

// need to make sure passing directory, not file
NODE* findChild ( NODE* f, char* childName, char childType )
{
	//if ( *childName == '\0') { printf("No name child exists!\n"); return 0; }
	//if ( !f ) { printf("No parent node exists!\n"); return 0; }
	int i;

	if (f->childPtr == 0) { printf("No such file or directory.\n"); return 0; }

	if (f->type == 'F') { printf("Cannot find location in a file!\n"); return 0; }
	else
	{
		f = f->childPtr;

		while ( 1 )
		{
			if ( strcmp(childName, f->name) == 0 && f->type == childType) 
			{
				break;
			}
				
			else if ( f->siblingPtr ) { f = f->siblingPtr; }
			else if ( f->siblingPtr == 0 ) { f = 0; break;}
			else { break; printf("wtf?\n"); } //should not get here
		}
		
		return f;
	}
}

NODE* findPath ( char* dn )
{	
	int dirCount;
	char* nNode;		// next node name
	char* slash;
	NODE *f;

	
	if ( *dn == '/' ) { f = root; dirCount = 0; } //printf("absolute\n");}
	else { f = cwd; dirCount = 1; } // printf("relative\n"); }

	//printf("dn: %s\n", dn);
	while ( *dn )
	{
		//printf("inwhile:\ndn: %s\n", dn);
		slash = strchr(dn, '/');
		//printf("segfault?\n");
		//printf("slash: %s\n", slash);
		if ( *dn == '/' && dirCount == 0 ) // root case, will be looking for / children
		{
			//printf("if\n");
			dirCount++;
			dn++;
			//printf("newdn: %s\n", dn);
		}
		else if ( *dn == '/' && dirCount > 0 )
		{
			//printf("elseif\n");
			f = findChild( f, nNode, 'D' ); // returns child directory if exists, 0 if not.
			if ( f == 0 )
			{
				printf("Dir %s does not exist in the current context, please check your entry and try again.\n", nNode );
				//printf("f->name: %s\n", f->name);
				return f; // returns null f
			}
			dn++;
			//strcpy (nNode, ""); // clear out nNode's values
		}
		else if ( slash == 0 ) // no / "cd a" case or make dir in dir case
		{
			//printf("elseif2\n");

			f = findChild( f, dn, 'D' ); // returns child directory if exists, 0 if not.
			if ( f == 0 )
			{
				//printf("Dir %s does not exist in the current context, check your entry and try again.\n", dn );
				//printf("f->name: %s\n", f->name);
				return f; // returns null f
			}
			return f; // end while at end of loop in this case
		} 
		else
		{
			//printf("else\n");
			while ( *nNode != '\0' )
			{
				*nNode = '\0';
				nNode++;
			}

			int i = 0;
			strcpy(nNode, dn);
			//printf("nNode: %s\n", nNode);
			//nNode++; dn++;
			while ( nNode[i] != '\0' )
			{
				if ( nNode[i] == '/' ) { nNode[i] = '\0'; break;} // don't increment dn again
				dn++;
				i++;
				//nNode++;
				//printf("dn: %s\n", dn);
				//getchar();
			}
			//printf("*dn: %c\n", *dn);
			//printf("nNode: %s\n", nNode);
			//printf("dirname: %s\n", dirname);
		}
	}

	// now should be in directory to insert node
	// insert node f
	//printf("outOfWhilef->name: %s\n", f->name);
	return f;
	
}

// split path into dir and base
int pathToDB(char* path)
{
	int dirs = 0;
	int i = 0, j = 0;

	//debug("abs, b4 while");

	while( path[i] ) // count directories
	{
		if ( path[i] == '/' ) { dirs++; }
		i++;
	}
	
	//printf("dirs: %d\n", dirs);
	i = 0; // reset i

	// gets dirname and basename from path
	if ( dirs == 0 ) { strcpy(basename, path); } // relative to cwd no / case

	while ( dirs > 0 )
	{
		while( dirs > 0 ) // only copy up to the last /
		{
			dirname[i] = path[i];
			if ( path[i] == '/' ) { dirs--; if ( dirs == 0 ) { break;} }
			i++;
		}
		
		if ( dirs == 0 ) // get basename
		{
			if ( i > 0 ) { dirname[i] = '\0'; } // don't remove / from root
			i++;
			while ( path[i] )
			{
				
				basename[j] = path[i]; 	// get basename into correct variable
				//path[i] = '\0';
				i++;
				j++;
			}
		}
		i++;
	}

	
	//printf("path %s turned into -\n", path);
	//printf("new dirname: %s\n", dirname);
	//printf("new basename: %s\n", basename);
}

int cd (char* pn)
{
	NODE* f;
	//printf("pn: %s\n", pn);
	if ( *pn == 0 ) { cwd = root; return; }
	else if ( strcmp(pn, "..") == 0 )
	{
		if ( cwd->parentPtr ) { cwd = cwd->parentPtr; }
		else { cwd = root; }
		return;
	}
	else
	{
		pathToDB(pn);
		if ( dirname == 0 ) { f = findChild(cwd, basename, 'D'); }
		else
		{
			if ( findPath(dirname) ) { f = findPath(dirname); }
			else { return; }

			//f = findPath(dirname);
			f = findChild(f, basename, 'D');
			if ( f == 0 )
			{
				if ( basename )
				{
					printf("Could not change to directory %s, no such directory.\n", basename);
					return;
				}
				else 
				{ 
					printf("Could not change to directory (null), no directory name referenced.\n");
					return;
				}
			}
		}
		//printf("cwd now: %s\n", f->name);
		cwd = f;
	}
}

int rmNode( NODE* f, char* bn, char type )
{
	NODE *prnt, *sib;

	if ( f->childPtr == 0 )
	{
		printf("Cannot remove file that does not exist\n");
		return;
	}

	prnt = f;
	f = f->childPtr;
	while ( 1 )
	{
		if ( ( strcmp(bn, f->name) == 0 ) && ( f->type == type ) ) // found node
		{
			if ( f->childPtr ) // files cannot have child pointers
			{
				printf("Cannot remove dir %s while files exist inside\n", bn);
				return;
			}
			
			// need to find sibling to delete, and then relink nodes
			// if there are no siblings need to delete and set parent->childPtr to null
			// if deleted direct child of parent, need to reset child pointer to the next sibling

			else if ( prnt->childPtr == f ) // first child of parent case
			{
				if ( f->siblingPtr )
				{
					sib = f->siblingPtr; // use sib as a temp
					prnt->childPtr = sib;
					free (f);
					return;
				}
				else
				{
					prnt->childPtr = 0;
					free (f);
					return;
				}
			}
			else // fix sibling ptrs
			{
				if ( f->siblingPtr ) // sib follows behind f
				{
					sib->siblingPtr = f->siblingPtr;
					free (f);
					return;
				}
				else // no siblings left in list
				{
					sib->siblingPtr = 0;
					free (f);
					return;
				}
			}
		}
		else
		{
			if ( f->siblingPtr == 0 )
			{
				printf("No such file or directory.\n");
				return;
			}
			sib = f; // have sib follow f
			f = f->siblingPtr; // set f to next sibling
		}
	}
}

int rm(char* path)
{
	NODE* f;
	pathToDB(path);
	
	f = findPath(dirname);
	
	if ( f == 0 ) { return; }
	
	rmNode(f, basename, 'F');
}

int rmdir(char* path)
{
	NODE* f;
	pathToDB(path);
	
	f = findPath(dirname);

	if ( f == 0 ) { return; }
	
	rmNode(f, basename, 'D');
}

int mkdir(char* path)
{
	NODE* f;
	pathToDB(path);

	printf("dirname: %s\nbasename: %s\n", dirname, basename);

	// need to update to make sure file's done act like directories
	f = findPath(dirname);

	if ( f == 0 ) { return; }
	/*else if ( f->type == 'F' )
	{
		printf("Files are not directories, cannot insert %s\n", basename);
		return;
	}*/

	insNode(f, basename, 'D');
}

int creat(char* path)
{
	NODE* f;
	pathToDB(path);

	f = findPath(dirname);

	if ( f == 0 ) { printf("Files are not directories, cannot insert %s.\n", basename); return; }
	//else if ( f->type == 'F' )
	//{
	//	printf("Files are not directories, cannot insert %s\n", basename);
	//	return;
	//}

	insNode(f, basename, 'F');
}

void initialize()
{
	//root = (NODE *)malloc(sizeof(NODE));
	//
	//strcpy(root->name, "/");
	//root->type = 'F';
	root = createDir("/");
	
	cwd = root;

	//printContents( root );
	//printContents( cwd );

	strcpy(commands[0], "menu\0");
	strcpy(commands[1], "mkdir\0");
	strcpy(commands[2], "rmdir\0");
	strcpy(commands[3], "cd\0");
	strcpy(commands[4], "ls\0");
	strcpy(commands[5], "pwd\0");
	strcpy(commands[6], "creat\0");
	strcpy(commands[7], "rm\0");
	strcpy(commands[8], "save\0");
	strcpy(commands[9], "reload\0");
	strcpy(commands[10], "quit\0");

	/* DEBUG */
	//findPath(root->name);

	
}

void argSplit(char* line, char* cmd, char* path)
{
	int i = 0;	
	//printf("line: %s\n", line);
	while(line[i])
	{
		if (line[i] == ' ' && line[i+1]) { i = -1; break; } // two commands exist set i < 0
		i++;
	}
	
	if ( i < 0 ) { sscanf(line, "%s %s", cmd, path); }	// read two commands
	else if ( i == 0 ) { printf("No command entered!\n"); return; } // i never incremented, no command exists
	else { sscanf(line, "%s", cmd); }			// no space, therefore command only read
	/* FOR DEBUG */
	//printf("new command: %s\nnew argument: %s\n", cmd, path);	
}

int findCommand(char* ln)
{
	int check, i = 0, j = 0;

	fgets(ln, 128, stdin);
	ln[strlen(ln)] = 0; // get rid of \r;
	size_t c = strlen(ln) - 1;
	if ( ln[c] == '\n' ) { ln[c] = '\0'; }
	//printf("%s read\n", ln);

	argSplit(ln, command, pathname);

	while ( i < 64 )
	{
		check = strcmp(command, commands[i]);
		if ( check == 0 ) 
		{
			return i;
		}
		i++;
	}
	printf("Command not recognized, enter menu for a list of options.\n");
	return -1;
}

int printTree(FILE *fp, NODE* n)
{
	if ( n == 0 ) { return; }
	char *c = (char*)malloc(sizeof(char[128]));
	pwd(c, n);

	printf("%c %s wrote\n", n->type, c);
	fprintf(fp, "%c %s\n", n->type, c);
	printTree ( fp, n->childPtr );
	printTree ( fp, n->siblingPtr ); 
	free(c);
}


int reload()
{
	FILE *fp;
	fp = fopen("filesystem", "r"); // basic information, need file filesystem
	if (fp == 0) { printf("filesystem does not exist in current directory!\nCannot reload.\n"); return; }

	char buf[32];
	//char* newLine = malloc(sizeof(char[128]));
	int i;
	
	fgets( buf, sizeof(buf), fp ); // eat first line, root will always be created
	while ( feof(fp) != EOF )
	{
		//buf = malloc(sizeof(char[64]));
		fgets( buf, sizeof(buf), fp );
		i = 0;
		//printf("buf[0] = %c\n", buf[0]);
		//printf("buf = %s", buf);
		if (buf[i] == 'D')
		{
			while ( buf[i+2] != '\n' ) { buf[i] = buf[i+2]; i++; } // move everything over one space
			buf[i] = 0;
			//strcpy(line, "mkdir");
			//strcat(line, buf);

			printf("newLine: %s\n", buf);
			//if ( strcmp(buf,"/") == 0 ){continue;} // do nothing for root case
			mkdir(buf);
		}
		else if (buf[i] == 'F')
		{
			//printf("before while\n");
			while ( buf[i+2] != '\n' ) { buf[i] = buf[i+2]; i++; } // move everything over one space
			//printf("after while\n");
			buf[i] = 0;
			//strcpy(line, "creat");
			//strcat(line, buf);
	
			printf("line: %s\n", buf);
			creat(buf);
		}
		else
		{
			//printf("Error reading file.\nQuitting.\n"); break;
			break;
		}
		i = 0;
		while (buf[i]) { buf[i] = 0; i++; }
		clearGbls();
	}
	//fprintf(fp, "%c %s", type, dir);
	fclose (fp); 
	//free(buf);
	//free(newLine);
}

int save()
{
	FILE *fp = fopen("filesystem", "w+"); // basic information, need file filesystem
	
	printTree(fp, root);
	fprintf(fp, "\n");

	fclose (fp); 
}

int debug(char* str)
{
	printf("debug %s\n", str);
	printf("command: %s\n", command);
	printf("pathname: %s\n", pathname);
	printf("dirname: %s\n", dirname);
	printf("basename: %s\n", basename);
} 

int quit()
{
	printf("Saving tree to filesystem before exit.\n");
	save();
	
	exit(0);
}


void menu()
{
	printf("------------------------------------- MENU -------------------------------------\n");
	printf("menu\t\t\t- Prints this menu.\n");
	printf("mkdir\tpathname\t- make a new directory for the pathname.\n");
	printf("rmdir\tpathname\t- remove the directory, if it is empty.\n");
	printf("ls\t[pathname]\t- list the directory contents of pathname or CWD\n");
	printf("\t\t\t  [F] is a file, [D] is a directory.\n");
	printf("cd\t[pathname]\t- change CWD to pathname, or to / if no pathname.\n");
	printf("pwd\t\t\t- print the (absolute) pathname of CWD.\n");
	printf("creat\tpathname\t- create a FILE node.\n");
	printf("rm\tpathname\t- remove a FILE node.\n");
	printf("save\tfilename\t- save the current file system tree in a file.\n");
	printf("reload\tfilename\t- re-initialize the file system tree from a file.\n");
	printf("quit\t\t\t- save the file system tree, then terminate the program.\n");
	printf("--------------------------------------------------------------------------------\n");
}

// clear all global variables for new entry
int clearGbls()
{
	int i = 0;
	while ( i < 64 )
	{
		if ( i < 16) {command[i] = '\0';}
		pathname[i] = '\0';
		dirname[i] = '\0';
		basename[i] = '\0';
		i++;
	}
}	

int findCmd()
{
	int ID = findCommand(line);
	switch(ID)
	{ 
		case 0 : menu();     		break;
		case 1 : mkdir(pathname);    	break;
		case 2 : rmdir(pathname);    	break;
		case 3 : cd(pathname);       	break;
		case 4 : ls(pathname);       	break;
		case 5 : pwd(0, 0);		break;
		case 6 : creat(pathname);	break;
		case 7 : rm(pathname);		break;
		case 8 : save();		break;
		case 9 : reload();		break;
		case 10 : quit();		break;
		default : 			break;
        }
	ID = 0;
	clearGbls();
}

int main()
{
      	initialize();      /* initialize the / DIR of the tree */
	
	while(1)
	{
		printf("input a command: ");
		findCmd();
      	}
}
