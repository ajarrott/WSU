# this contains the main function for my program so it can call the myprintf function
# in assembly and then call "mymain" from the t.c file and run myprintf there
# get_ebp is necessary to get an undefined amount of items on the stack in the myprintf function

###########################
# Anthony Arrott 10853267 #
#   CPTS 360 K.C. Wang    #
###########################

#--------------------- s.s file --------------------------
         .global main, mymain, myprintf, get_ebp
main:    
         pushl  %ebp		# put ebp on stack
         movl   %esp, %ebp	# point SP at ebp
	
# (1). Write ASSEMBLY code to call myprintf(FMT)
#      HELP: How does mysum() call printf() in the class notes.

	pushl	b		# setup stack
	pushl	a		# b | a | fmt
	pushl	$fmt
	call	myprintf	# call myprintf
	addl 	$12, %esp	# bring stack back up

	movl 	%ebp, %esp	# go back to original stack pointer
	popl 	%ebp


# (2). Write ASSEMBLY code to call mymain(argc, argv, env)
#      HELP: When crt0.o calls main(int argc, char *argv[], char *env[]), 
#            it passes argc, argv, env to main(). 
#            Draw a diagram to see where are argc, argv, env?

	pushl 	%ebp		# get ebp on stack
	mov	%esp, %ebp	# point SP at ebp

#    HIGH  16	  12      8	 4	0      -4    LOW
#	---------------------------------------------
#	| envp | argv | argc | retPC | ebp | locals |
#	---------------------------------------------
	
	# push items in correct order on stack to be read by mymain
	pushl	16(%ebp)	# envp 
	pushl	12(%ebp)	# argv
	pushl	8(%ebp)		# argc
	call	mymain		# call mymain
	
	addl	$12, %esp	# bring stack back up

	movl	%ebp, %esp	# go back to original sp
	popl	%ebp


# (3). Write code to call myprintf(fmt,a,b)	
#      HELP: same as in (1) above
	
	pushl  %ebp		# push ebp on stack
        movl   %esp, %ebp	# point sp to ebp
	
	pushl	b		# push b
	pushl	a		# push a
	pushl	$fmt		# push fmt
	call	myprintf	# call printf
	addl 	$12, %esp	# bring stack back up

# (4). Return to caller
        movl  	%ebp, %esp	# go back to original sp
	popl 	%ebp
	ret

# helper function for myprintf to get any finite number of user definined variables 
get_ebp:
	 movl %ebp, %eax
         ret

#---------- DATA section of assembly code ---------------
	.data
FMT:	.asciz "main() in assembly call mymain() in C\n"
a:	.long 1234
b:	.long 5678
fmt:	.asciz "a=%d b=%d\n"
#---------  end of s.s file ----------------------------
