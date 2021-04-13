/* addresses of SLU units, treated as ints by upper layers,
   but cast to appropriate (struct csr *) by lower layer */
/* just for SA getc, putc, etc.--lower routines use slu6850.h
   or slu7201.h */
#define SLUCSR1  0xe50000
#define SLUCSR2  0xe50002
#define SLUCSR3  0xe70000
