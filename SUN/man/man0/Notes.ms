.TL




Xinu Programmer's Manual


Version 6b


.AU


(Xinu Is Not Unix)



.AI




June 1983



.AB no
This manual contains a description of the Xinu software.
It has been divided into four sections.
The introduction in section 0 describes how to use the Xinu
software to compile, download, execute, and debug a C program.
Section 0 also contains a set of informal implementation notes
that give the flavor of Xinu.
Section 1 gives the details and arguments of
cross-development commands available on the host that
cross-compile, cross-assemble, cross-load, download,
upload, and analyze programs.
Section 2 describes Xinu system procedures that programs call
to invoke operating system services.
Section 3 describes procedures available from the standard
libraries.
From the programmer's point of view,
there is little distinction between library routines and
system calls.
.PP
As in the UNIX manual, each page describes a command, system
call or library routine;
section numbers appear in the page header
as "(digit)" following the name
of the program;
within a section all pages are arranged
in alphabetical order.
References have the same form as headers (e.g., "getc(2)"
refers to the page for "getc" in section 2).
.AE
.LP
.ND
.ds LH INTRO(0)
.ds RH INTRO(0)
.ds CH "Xinu Programmer\'s Manual
.pn 1
.bp
.ds RF %
.ds CF "\n(mo/\n(dy/\n(yr
.ds LF "Version 6b
.B
.ce
A Short Introduction To Xinu and the Cross-Development Software
.R
.sp
.SH
How to Use Xinu
.PP
\f2Architecture.\f1
Xinu comes in two parts: a cross-development environment that runs on
the host machine (usually a Digital Equipment Corp. VAX), and an
independent system that runs on the microcomputer (usually a Digital
Equipment Corporation LSI 11/02).
The microcomputer is connected to the host over an asynchronous
serial line like those used to connect terminals.
From the point of view of the host, the microcomputer is simply
another device that transmits and receives characters; from the
point of view of the micro, the host is merely a console terminal
that transmits and receives characters.
.PP
\f2Overview.\f1
To run a program under Xinu, you create the source file
on the host machine, and invoke cross-development software
to compile and load the program along with the Xinu
system.
Once a complete memory image has been produced, it can be downloaded
onto the microcomputer where it executes independent of the host.
During execution, you invoke a program on the host that
captures characters emitted by the micro and displays them on the
terminal screen, and sends characters typed at the keyboard to
the micro.
Thus, your terminal on the host appears to be connected directly to
the micro.
If the micro crashes, it can be restarted without downloading (provided
the crash did not destroy the program in memory).
To help debug severe crashes, the contents of memory on the micro
can be uploaded to a file on the host where a post-mortem program
can analyze the state and report problems.
.PP
\f2Cross-Development commands.\f1
The cross-development system contains a C compiler, linking loader,
downloader, uploader, and post-mortem debugger as well as a few
miscellaneous commands.
The details can be found in section 1 of this manual.
These commands probably reside in the Xinu "bin" directory on your
system; the directory name must be part of your PATH for you to
execute them.
If they are not in directory /usr/Xinu/bin, consult 
whoever installed Xinu to find out the bin directory name and
add it to your path.
.PP
\f2Compiling programs.\f1
The command \f2cc11\f1 works like the UNIX \f2cc\f1 command.
It invokes the C cross-compiler, cross-assember, and cross-loader
to translate C programs into a memory image.
Like \f2cc\f1, the actions of \f2cc11\f1 depend on the file
names passed to it as arguments -- names ending in ".c" are
assumed to be C programs, those ending in ".s" are assumed to be
assember programs, and those ending in ".o" are assumed to be
previoulsy compiled object programs.
Unless you specify otherwise, \f2cc11\f1 compiles C programs,
assembles assembler programs, and loads object programs to
produce a memory image in file \f2a.out\f1.
Normally, the memory image contains the Xinu operating system
along with your program (you can ask \f2cc11\f1 to
leave out the operating system and just prepare a "stand-alone"
program).
.PP
\f2Downloading.\f1
Command \f2download\f1 reads
file \f2a.out\f1, and loads the memory image into the microcomputer
(it will look for the memory image in a different file if you
instruct it to do so).
Usually, \f2download\f1 is invoked with an argument "-a5" that
causes the microcomputer to delay for five seconds before starting
execution of the downloaded program.
.PP
\f2Interacting with the Micro.\f1
The microcomputer on which Xinu runs is attached to the host like
a peripheral device.
The program \f2odt\f1 "connects" your terminal to the
microcomputer by relaying characters between the terminal
and the device.
Characters that arrive from the micro are sent to your terminal,
while characters typed on your keyboard are sent to the
micro.
\f2Odt\f1 can be invoked at any time, but it is most often used
just after a \f2download\f1 so you can see the output of
the program as it runs.
\f2Odt\f1 will halt the microcomputer for you by "breaking" the console
line if you type the 2-character sequence backslash (\\) followed
by null (CONTROL-@).
To proceed again, type uppercase-P (see the LSI 11 manual for
more information on the "odt" mode).
.PP
\f2Debugging a crash.\f1
If the program running on the micro crashes, the cause of trouble
may not be easy to spot.
Help is available from a post-mortem debugger, \f2pm\f1.
You must first execute command \f2upload\f1
to copy the complete memory image from the micro into a file
on the host.
By default, the image file is named \f2core11\f1.
After the \f2core11\f1 file has been created, run command \f2pm\f1
to cull through it and print out the system status.
\f2Pm\f1 uses both \f2core11\f1 and \f2a.out\f1 to diagnose the problem
(as usual, the actual file names can be changed if you don't happen to
like them).
.SH
An Example
.PP
\f2Create a C program.\f1
For example, here is a C program that
prints the string "Hello world." on the console terminal and exits
(\f2Printf\f1 is a system (library) procedure that prints formatted
strings on the console; other system commands are described in
sections 2 and 3 of this manual):
.in 1.2i
.sp
.nf
/* example C program in file example.c */
main()
{
	printf("Hello world.\\n");
}
.sp
.fi
.in 0i
.PP
\f2Compile and Download.\f1
Cross-compile the program,
download it onto the micro, and connect your terminal to the
micro with the following commands:
.sp
.nf
.in 1.2i
cc11 example.c
download -a5
odt
.sp
.in 0i
.fi
The cross-compiler will compile the C program, and load it along with
the Xinu system, leaving file \f2a.out\f1 in your current directory.
The downloader will copy the image from \f2a.out\f1 into the micro
and start it executing (after a delay of five seconds).
During downloading, you will see a count of the bytes remaining as
blocks are transferred.
Finally, \f2odt\f1 will connect your terminal to the micro (the
5-second delay leaves time for the VAX to start \f2odt\f1).
When the micro begins execution you will see a few Xinu system
startup messages followed by the program output.
When all of your processes complete (in this case, when the
single program terminates), you will see a system termination
message.  The output is:
.sp
.in 1.2i
.nf
Xinu Version 6.09b 3/1/83
57346 real mem
21268 avail mem
clock enabled

Hello world.

All user processes have completed.
.sp
.fi
.in 0i
.PP
\f2Re-run the program.\f1
To re-run the program without downloading the micro again, type:
.nf
.sp
.in 1.2i
\&\\CONTROL-@
1000G
.sp
.fi
.in 0i
The 2-character sequence backslash (\\) null (control-@) causes
\f2odt\f1 to halt the LSI 11 and place it in "ODT" mode.
The LSI 11 responds with an at-sign prompt.
The sequence "1000G" starts the machine executing at location 1000
(octal).
To get out of \f2odt\f1, kill the process by typing the "DELETE" key.
Note that killing \f2odt\f1 does not stop the micro -- it merely
disconnects your terminal.
.PP
\f2Upload the memory.\f1
You may want to see what processes are (were) running.
To retrieve the memory image and analyze it, run the commands:
.sp
.nf
.in 1.2i
.sp
.ne 2
upload
pm
.sp
.in 0i
.fi
Warning: \f2upload\f1 destroys the contents of memory on the
micro as it executes,
so the micro cannot be restarted again after uploading.
Also note that if you interrupt (i.e. kill) the uploader
and then restart it, the image it retrieves will be incorrect.
.PP
\f2Interpreting pm.\f1
\f2Pm\f1 reports things like whether the program text has been
changed and the status of each process.
If the output from \f2pm\f1 seems unreasonable, check for the
following common errors.
If significant portions of the program have been changed, it may mean
a stack overflow occurred; totally meaningless process
information often indicate that the
overflow extended into the process table.
Having only one or two bad process states in an otherwise
meaningful set may indicate that the context switch ended up
with no ready or current processes; this
only happens if you modify the system code or add your own
device driver.
When experimenting with device drivers, look carefully at
the status of the null process after a crash -- if you
find it sleeping, waiting,
receiving, or suspended then you probably have a lower-half
driver routine that removes the null process from the current/ready
lists.
.PP
\f2System Termination.\f1
Xinu may not always print the system termination message
even if all your processes exit, because it interprets
the term "user process" to mean "all processes except the
null process."
This can be confusing because the network software
starts processes that never terminate (they continue
forwarding frames even if the CPU is otherwise idle).
Also remember that the tty driver will continue to echo
characters even if there are no processes running to
consume them.
.PP
\f2Hints on restarting.\f1
The LSI 11 ODT command 1000G sets the program counter to 1000 and
starts execution with interrupts \f2enabled\f1.
Xinu disables interrupts immediately after it starts executing
to avoid being interrupted before the system is ready.
If an interrupt occurs before the LSI 11 can execute
the first instructon, it may cause the system to
crash (ungracefully).
If your processor gives you trouble with the "G" command,
then type the following
three lines to restart Xinu:
.sp
.in 1.2i
.nf
RS/xxxxxx 340
R7/xxxxxx 1000
P
.sp
.in 0i
.fi
The LSI 11 will print octal values in place of xxxxxx.
Note: no carriage return is used after
the "P" command (consult the LSI 11 manual for more information).
.pn 1
.LP
.ND
.ds LH NOTES(0)
.ds RH NOTES(0)
.ds CH "Xinu Programmer\'s Manual
.bp
.ds RF %
.ds CF "\n(mo/\n(dy/\n(yr
.ds LF "Version 6b
.B
.ce
Xinu Implementation Notes
.R
.ce
Updated 1/82, 3/82, 11/82.
.PP
These are the notes kept during implementation; they are not
designed as an accurate introduction to Xinu.
In particular, deferred operations implemented with \f2pmark\f1
have disappeared from the book version even though they remain
in version 5.
.LP
Some quick ideas:
.IP - 3
There are multiple processes.
.IP - 3
A process is known by its process id.
.IP - 3
The process id is an index into the process table.
.IP - 3
There are counting semaphores.
.IP - 3
A semaphore is known by its index in the semaphore table.
.IP - 3
There is a line time clock.
.IP - 3
The system schedules processes based on priority.
.IP - 3
The system supports I/O.
.IP - 3
For tty I/O characters are queued
on input and output.
The normal mode includes echoing, erasing backspace, etc.
.IP - 3
There is a frame-level data link communications package to
make a ring of LSI 11's.
.IP - 3
There is a file system that supports concurrent growth of
files without preallocation; it has only a single-level
directory sturcture.
.IP - 3
There is a one-word message passing mechanism.
.IP - 3
There is support for self-initializing routines (memory
marking) that  makes the system serially reusable without
requiring the kernel to explicitly call initialization routines.
.IP - 3
Processes can create processes, kill processes, suspend processes,
restart processes, and change the priority of processes.
.IP - 3
There is no real memory management, but there are primitives for
acquiring and returning memory from the global pool, and a way
to suballocate smaller pools of fixed-size buffers.
.IP - 3
There is a configuration program, config, to generate a Xinu system according
to specifications given.
.LP
Discussion of implementation:
.sp
.IP 0. 3
Files.
The system sources are organized as a set of procedures.
For the most part, there is a file for each system call
(e.g., chprio.c for the system call chprio).
In addition to the system call procedures, a file may contain
utility functions needed by that system call.
Files which do not correspond to system calls are:
.sp
.RS
.IP "Configuration" 3
The file of device and constant information as edited by the
user to describe the hardware; the config program takes this
file and produces conf.c and conf.h files.
.IP "conf.h" 3
Generated constants including I/O and size constants; do not
edit directly.
.IP "conf.c" 3
Generated file of initialized variables; do not edit directly.
.IP "kernel.h" 3
General symbolic constants; misc defined macros.
.IP "proc.h" 3
Process table entry structure declaration; state constants.
.IP "sem.h" 3
Semaphore table entry structure declaration; semaphore constants.
.IP "io.h" 3
General user-level I/O definitions.
.IP "slu.h" 3
Serial Line Unit CSR layout; I/O constants for slu's.
.IP "tty.h" 3
tty line discipline control block, buffers, excluding sizes.
.IP "dlc.h" 3
line discipline control block for asynchronous device used as
network interface.
.IP "disk.h" 3
disk driver control block.
.IP "dtc.h" 3
Digital Technology Corp. SASI disk interface hardware register layouts.
.IP "xebec.h" 3
Xebec Corp. SASI disk controller register layouts.
.IP "frame.h" 3
Xinu ring network frame format definitions.
.IP "bufpool.h" 3
Buffer pool constants and format.
.IP "mark.h" 3
Memory marking table declarations.
.IP "mem.h" 3
Memory management free list format declarations.
.IP "ports.h" 3
Definitions for communications ports (queued interprocess rendevous points).
.IP "sleep.h" 3
Definitions for real-time delay software.
.IP "dir.h" 3
Layout of disk directory block.
.IP iblock.h" 3
Layout of index block (i-block).
.IP "file.h" 3
Definitions of variables and constants used by the local file
system routines.
.IP "q.h" 3
q data structure declaration (see below); defined macros for
q predicates.
.IP "queue.c" 3
q manipulation routines.
.IP "resched.c" 3
Almost the inner most routine (rescheduler).
It selects the next process to run from the ready queue and
fixes up the state.
Calls ctxsw to switch contexts.
.IP "cxtsw.s" 3
The routine that actually changes the executing process into
another one.
A very small piece of assembler code with only one trick: when a
process is saved, the execution address at which it restarts is
actually the instruction following the call to ctxsw.
.IP "lowcore.s" 3
The loaded version of the low part of memory (interrupt vectors).
All interrupt vectors are initialized by the loader to point to
panic routines, and overwritten for valid devices at startup.
.IP "ioint.s" 3
I/O interrupt dispatchers.
.IP "startup.s" 3
Actual entry point (start) with code to set up C run-time
environment and call high-level initialization.
.IP "initialize.c" 3
All external (global) variables,
the null process (process 0, see below),
and the high-level system initialization routine
(e.g., to craft the process table entry for process 0).
.IP "userret.c" 3
The routine to which user processes return (i.e. exit) if they
ever do.
Care should be taken so that userret never exits; it must kill the
process that runs it because there is no legal return frame on
the stack.
.IP "sizmem.s" 3
Utility procedure to size main memory.
.LP
.RE
.sp
.IP 1. 3
Process states.
Each process has a state given by the pstate field in the process
table entry.
The state values are given by symbolic constants PRxxxx.
PRFREE means that the process entry is unused.
PRREADY means that the process is linked into the ready list
and is eligible for the CPU.
PRWAIT means that the process is waiting on a semaphore (given by
psem).
PRSUSP means that the process is in hibernation; it is not on any list.
PRSLEEP means that the process is in the queue of sleeping
processes waiting to be awakened.
PRCURR means that the process is (the only one) currently running.
The currently running process is NOT on the ready list.
In addition to the actual state, there is a "mark" field (pmark) which 
indicates pending state changes.
PMDEAD indicates that the process has been killed and should be
removed as soon as it reaches the ready queue.
PMSUSP indicates that the process has been suspended and should
move to the suspended state as soon as it reaches the ready
queue.
PMNONE indicates no pending action.
.IP 2. 3
Semaphores.
Semaphores reside in the array semaph.
Each entry in the array corresponds to a semaphore and has a
count (scount), and state (sstate).
The state is SFREE if the semaphore slot is unassigned,
SUSED if the semaphore is in use.
If the count is -p then the sqhead and sqtail fields point to
a FIFO queue of p processes waiting for the semaphore.
If the count is nonnegative p then no processes are waiting.
More about the head and tail pointers below.
.IP 3. 3
Suspension.
Suspended processes are forbidden from using the CPU.
They may remain on semaphore/sleep queues until they
are to be moved to the ready queue.
A call to ready(p), where p has been marked suspended, will
NOT place it on the ready queue.
It will merely result in p being placed in the suspended state.
Suspending a process that is already on the ready queue will
remove it.
Suspending the current processes forces it to give up the CPU.
.IP 4. 3
Sleeping.
When a process calls sleep(n) it will be delayed n seconds.
This is achieved by placing the process on a queue of jobs
ordered by wakeup time and relinquishing the CPU.
Every 60th of a second, an external line-time clock will
interrupt the CPU and cause a clock interrupt routine to
be called.
To avoid extra overhead, 5 such interrupts are ignored before
one is processed.
Thus, the granularity of clock counts is 1/10 of a second.
The interrupt handler maintains a clock and moves processes
back to the ready queue when their wakeup time has been
reached.
Notice that a process may put itself, but no one else, to
sleep.
.IP 5. 3
Queues and ordered lists.
There is one data structure for all heads, tails, and
elements of queues or lists of processes: q[].
The first NPROC entries in q (0 to NPROC-1) correspond to the
NPROC processes.
If one wants to link process i onto a queue or list, then one
uses q[i].qnext and q[i].qprev as the forward and backward
pointers.
.sp
The remaining entries in q are used for the heads and tails of
lists.
The integer nextqueue always points to the next available
entry in q to assign.
When initialize builds the heads and tails of various lists,
it assigns entries in q sequentially.
Thus, the sqhead and sqtail fields of a semaphore are really
the indices of the head and tail of the list in q.
The advantage of keeping all heads and tails in the same
data structure is that enqueueing, dequeuing, testing for
empty/nonempty, and removing from the middle (eg., when a process
is killed) are all handled by a small set of simple
procedures (files queue.c and q.h).
An empty queue has the head and tail pointing to each other.
Since all real items have index less than NPROC, testing whether
a list is empty becomes trivial.
In addition to FIFO queues, q also contains ordered lists based
on an integer kept in the qkey field.
For example, processes are inserted in the ready list (head at
position q[rdylist]) based on their priority.
They are inserted in the sleep list based on wakeup time.
Ordered lists are always in ascending order with the inserted
item stuck in BEFORE those with an equal key.
Thus, processes are removed from the ready list from the tail
to get the highest priority process.
Also, processes of equal priority are scheduled round robin.
Since the sleep queues are serviced from the smallest to
largest keys, items are removed from the head of the queue
(equal keys do not matter for sleeps).
.IP 6. 3
Process 0.
Process 0 is a null process that is always available to run
or is running.
Care must be taken so that process 0 never executes code that
could cause its suspension (e.g. it cannot wait for a semaphore).
Since Process 0 may be running during interrupts, this means that
interrupt code may never wait for a semaphore.
Process 0 initializes the system, creates the first user process,
starts it executing the main program, and
goes into an infinite loop waiting until an interrupt.
Because its priority is lower than that of any other process, the
null process loop executes only when no other process is ready.
It uses a pause instruction while waiting to avoid taking bus cycles
just in case dma devices are running.
.LP
