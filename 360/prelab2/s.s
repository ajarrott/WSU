# provided get_ebp function to get the ebp from the stack

        .global get_ebp 
get_ebp: 
         movl %ebp, %eax
         ret

