.LP
.ND
\ 
.sp 3
.ce
Introduction to the Xinu System Commands
.sp 3
.PP
The Xinu operating system kernel consists of a set of run-time procedures
to implement operating system services on an LSI 11/02 microcomputer.
The system supports multiple processes, I/O, synchronization
based on counting semaphores, preemptive scheduling, and
communications with other machines.
System calls are implemented as independent procedures.
.PP
All system calls block user processes that cannot be serviced.
For example, if a process waits on a semaphore that is not
available, the system blocks the calling process and runs another.
When the semaphore becomes available, the process resumes
as if there had been
no interruption
(i.e., user processes need not worry about scheduling the processor).
.PP
A process can create new processes, kill processes, or put them
in a state of hibernation (where they cannot use the CPU).
On a given CPU, each process has a unique
process identifier assigned when it is created.
Many of the commands require the caller to supply a process identifier.
A process can determine its identifier as well as those of all
processes it creates.
.PP
Processes sychronize through a set of counting semaphores.
The basic operations on semaphores are implemented as wait() and
signal() procedures.
The procedure wait(x) decrements the count of semaphore x by 1.
If the resulting count is less than zero, the caller is
suspended (on a FIFO queue associated with x) until some other
process signals by executing signal(x).
Semaphores can be created as needed, deleted when they are no
longer necessary, and interrogated to determine their count.
.PP
As in the UNIX operating system,
each system call is a function which returns an integer value;
the distinguished integer SYSERR is returned to indicate that an
error condition prevented successful completion of the command.
.LP
