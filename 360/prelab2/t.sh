# this is a shell script I wrote to quickly figure out the size of a.out
# after compiling the files that had some little differences
#!/bin/sh

echo "t1.c"
cc -static -m32 t1.c
ls -l a.out
size a.out

echo "t2.c"
cc -static -m32 t2.c
ls -l a.out
size a.out

echo "t3.c"
cc -static -m32 t3.c
ls -l a.out
size a.out

echo "t4.c"
cc -static -m32 t4.c
ls -l a.out
size a.out

echo "t5.c"
cc -static -m32 t5.c
ls -l a.out
size a.out

echo "t6.c"
cc -static -m32 t6.c
ls -l a.out
size a.out

