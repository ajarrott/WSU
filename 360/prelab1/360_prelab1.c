// this program reads a provided virtual disk then outputs
// information about the partitions on the virtual disk

/*******************************
 * Anthony Arrott 10853267 HW1 *
 *     K.C. Wang CPTS 360      *
 *******************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

char buf[512]; // 512 byte buffer

#define OFFSET 0x1BE // offset 446

// main struct to get information about vdisk
struct partition {
	unsigned char drive;            /* 0x80 - active */

	unsigned char head;             /* starting head */
	unsigned char sector;           /* starting sector */
	unsigned char cylinder;         /* starting cylinder */

	unsigned char sys_type;         /* partition type */

	unsigned char end_head;         /* end head */
	unsigned char end_sector;       /* end sector */
	unsigned char end_cylinder;     /* end cylinder */

        /* Needed to change variables from long to int
	   because my OS/Processor assumes long is 8 bytes */
	unsigned int start_sector;     /* starting sector counting from 0 */
	unsigned int nr_sectors;       /* nr of sectors in partition */
};

// returns correct cylinder value
int getCyl ( struct partition *p )
{
  // cylinder is the left 2 bits of sector + cylinder.
  int c = (((p->sector >> 6) << 8) + p->cylinder);

  return c;
}

// returns correct sector value
int getSec ( struct partition *p )
{
  // sector actually the first 6 stored bits
  return p->sector & 0x3F; 
}

// prints extended partition information
void printExtInfo( struct partition *p, int ss, int heads )
{
  int c = getCyl(p); // get correct cylinder value

  printf("%7d", c + 1); // print correct cylinder start
  printf("%10d", p->end_cylinder + 1); // print correct cylinder end
  printf("%10d", p->nr_sectors / heads ); // print blocks
  printf("%12x\n", p->sys_type); // print parition type in hex
}

// prints second entry for extended partition and returns
// the next start sector value
int printExtInfo2( struct partition *p, int heads )
{
  int c = getCyl(p); // get correct cylinder value

  printf("%7d", c); // print cylinders
  // print correct value for last entry
  if ( p->cylinder == 0 ) { printf("%10d", p->cylinder); }
  else { printf("%10d", p->end_cylinder + 1); }
  printf("%10d", p->nr_sectors / heads ); // print blocks
  printf("%12x\n", p->sys_type); // print partition type in hex

  return p->start_sector;
}

// gives us the extended partition information
void extExpand( char* name, int offset, int heads )
{
  // initialize variables
  int fd, r;
  int extSector, endSector, ns = 0;
  struct partition *p;

  fd = open(name, O_RDONLY); // open file

  lseek(fd, OFFSET, offset); // get to correct point in file

  r = read(fd, buf, 0); // read information into buf
 
  p = (struct partition *) buf; // set p to the information in buf

  extSector = p->start_sector; // starting extended sector
  ns = extSector; // next sector
  endSector = extSector + p->nr_sectors; // last sector

  printf("----------- LOOK FOR EXTENDED PARTITION -----------\n");
  printf("Ext Partition = %d | Start Sector = %4d\n", (offset / 16), ns);
  printf("         Enter any key to continue");
  getchar();
   
  if ( p->sys_type != 5 ) // make sure extended partition exists
    {
      printf("No extended partitions exist!\n");
      exit(1);
    }

  while(1) // endless while, until finds 0 pointer
    {
      // cylinder is the left 2 bits of sector + cylinder.
      int c = getCyl(p);

      printf("start_sector = %4d | ", ns); // starting sector for current partition
      printf("cylSize = %2d | ", heads * p->end_sector);
      printf("startCyl = %2d\n", p->cylinder); 
      // find next partition at OFFSET + pointer value*512 bytes;
      lseek( fd, (long) ns*512 + 0x1BE, 0);
   
      // read first entry
      r = read(fd, buf, 16);
      p = (struct partition*) buf;

      // print entry info
      printExtInfo(p, extSector, heads);
      
      // read second entry
      r = read(fd, buf, 16);
      p = (struct partition*) buf;
      
      // print info
      ns = printExtInfo2(p, heads);

      // print start sector from 0 @ extended part offset
      printf("p->start_sector = %d\n", ns); 

      if ( ns == 0 ){ break; } // if pointer value is null exit

      ns += extSector; // add beginning sector value to next start sector
    }
  close(fd); // close file
  printf("\n");
  return;
}

// print partition information in Fdisk format
// returns 1 if ext part exists, 0 otherwise
short int printInfoFdisk ( struct partition* p, int heads )
{  
  int c = getCyl(p); // get cylinder correct value
  int blocks = p->nr_sectors / heads; // get blocks
  
  // print formatted information
  printf("%6d", c + 1 ); // corrects value to true start
  printf("   %6d", p->end_cylinder + 1 ); // corrects value to true end
  printf("   %6d", blocks);
  printf(" %6x", p->sys_type); //prints part info in hex

  // if there is an extended partition return 1 (true)
  if ( p->sys_type == 5 ) { return 1; } 
  return 0;
}

// reads in correct partition information, and sends it to be printed
// in fdisk form, returns offset value for extended partition info
int printFdisk( char *name, int heads )
{
  int fd, r;
  struct partition *p;

  int partNum = 0; // number of partitions counted
  int numParts = 4; // total possible partitions

  int offset; // needed for extended partitions

  fd = open(name, O_RDONLY); // open and read file to correct position
  lseek(fd, (long) OFFSET, 0);

  while ( partNum < numParts )
    {
      // only print menu on first pass
      if (partNum == 0)
	{
	    printf("------- LINUX FDISK FORM -------\n");
	    printf("start_cyl end_cyl  blocks   type\n");
	}
      r = read(fd, buf, 16); // get info in p
      p = (struct partition *) buf;
      // if 1 returned need offset to expand on extended partition information
      if ( printInfoFdisk(p, heads) == 1 ) { offset = (partNum + 1) * 16; }
      partNum++;
      printf("\n");
    }

  close(fd); // close file

  return offset;
}

// prints raw information about disks
// return start sector information
void printInfoRaw ( struct partition *p )
{
  int s = getSec(p); // correct sector and cyl information
  int c = getCyl(p);

  printf("%2d", p->drive);
  printf("%4d", p->head);
  printf("%4d", s);         
  printf("%4d", c);         
  printf("%5x", p->sys_type);
  printf("%5d", p->end_head);
  printf("%4d", p->end_sector);
  printf("%4d", p->end_cylinder + 1); // ends at 1 more that the value
  printf("%8d", p->start_sector);
  printf("%8d\n", p->nr_sectors);
}

// prints raw information about the partition
void printRaw(char* name)
{
  int fd, r;
  int partNum = 0; // partition counter
  int numParts = 4; // max of 4
  struct partition *p;
   
   fd = open(name, O_RDONLY);
   lseek(fd, (long) OFFSET, 0);
   while ( partNum < numParts )
     {
       if(partNum == 0) { printf("------------------- RAW FORM -------------------\n");	 }

       // each main parition's info is + 16 from the info
       r = read(fd, buf, 16);
       // cast string as a partition to get information from bytes
       p = (struct partition*) buf;

       printInfoRaw(p); // print info in p
 
       partNum += 1; // increment partNum counted
     }

   close(fd); // close file
}

// gets some information necessary for calculates
// from the disk
int diskInfo ( char* name )
{
  int fd, r;
  struct partition *p;

  int heads[16]; // necessary variables
  int prevHead;
  int numHeads = 0;
  int i = 0;

  while ( i < 16 ) // initialize information to 0
    {
      heads[i] = 0;
      i++;
    }
  i = 0;

  fd = open(name, O_RDONLY); // open file and get information
  lseek(fd, (long) OFFSET, 0);
  r = read(fd, buf, 16);
  p = (struct partition *) buf;

  while ( i < 4 ) // max amount of partitions in MBR
  {
    if ( heads[i] == 0 && (prevHead != (p->head + 1)) ) // don't add already named head
      {
	heads[i] = p->head + 1; // add head value + 1 for ease of checking for 'null'
	prevHead = p->head + 1; 
	r = read(fd, buf, 16);
	i++;
      }
    else 
      {
	i++;
	if ( i < 4 ) { r = read(fd, buf, 16); } // otherwise get next partition info
	                                        // don't want to read too much
      }
  }

  i = 0; // reinitialize i
  while ( heads[i] != 0 ) // if nonzero, head exists
    {
      numHeads++;
      i++;
    }
  
  // print drive information
  printf("heads: %3d\t", numHeads);
  printf("sectors: %4d\t", p->end_sector);
  printf(" cylSize: %4d\n", p->end_sector * numHeads);

  close(fd);
  
  return numHeads; // needed for other calculations in the program
}

// make sure filename was typed correctly
void vdiskReal(char* name)
{
  if ( (access(name, O_RDONLY)) == -1 || name == '\0' )
    {
      printf("Disk %s does not exist\n", name);
      exit(1);
    }
  return;
}

// run functions in an order that makes sense
main(int argc, char *argv[])
{
  char vdiskName[64]; // buffer for filename
   
  int numHeads; // initialize necessary variables
  int offset = 0;
 
  strcpy(vdiskName, argv[1]); // copy string name into vdiskName
  vdiskReal(vdiskName); // check file

  numHeads = diskInfo(vdiskName); // get numheads, and print initial disk info

  printRaw(vdiskName); // print raw disk info
  offset = printFdisk(vdiskName, numHeads); // get offset for extended part (if it exists)

  extExpand(vdiskName, offset, numHeads); // prints extended partition information (if any)
  return 0;
}
