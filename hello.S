.text                           # section declaration


    .global _start


_start:
#sys_write
	  movq    $1,%rax            # system call number (sys_write)
	  movq    $1,%rdi             # first argument: file handle (stdout)
	  movq    $msg,%rsi           # second argument: pointer to message to write
	  movq    $length,%rdx          # third argument: message length
	  syscall               		# call kernel

                                # and exit

	  movq    $60,%rax             # system call number (sys_exit)
	  movq    $1,%rdi             # first argument: exit code
	  syscall             # call kernel

.data                           # section declaration

msg:
	.ascii    "Hello world!\n"   # our dear string
	length = . - msg                 # length of our dear string