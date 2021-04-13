/* mmu.h -- types needed for interaction with the Sun3 mmu for V7 xinu */


/* information about the MMU pages */
#define PGSIZ			0x2000	/* size of a single page	*/
#define PGMASK			PGSIZ-1
#define SEGSIZ			0x20000	/* size of a segment		*/


/* We start giving out DMA space just after the page used by the monitor*/
#define DMA_VM_START 0x0ff20000

/* Currently, we use the number of bytes for DMA which follow		*/
#define DMA_NBYTES 0x8000

/* before passing a dma address to a dma device (like the LANCE chip), 	*/
/* you must mask off the following high order bits.			*/
#define DMA_MASK		0x00ffffff



/* The structure of the page protection bits in the MMU			*/
union pte {				/* definition of page tbl entry	*/
    long value;				/* value as a whole, 32 bits    */
    struct {
	unsigned hvalid     : 1;	/* Valid,      1 = Valid page   */
	
	unsigned prot       : 2;	/* protection:			*/
       					/* 00 - nowrite, user   	*/
       					/* 01 - nowrite, super		*/
       					/* 10 - write,   user		*/
       					/* 11 - write,	 super		*/
	
	unsigned cache      : 1;	/* Cache,      1 = Don't cache  */
	unsigned type       : 2;	/* Type:       00, 01, 10, 11   */
	unsigned ref        : 1;	/* Accessed,   1 = been accessed*/
	unsigned mod        : 1;	/* Modified,   1 = been modified*/
	
	unsigned unused     : 5;	/* unused			*/

	unsigned pfn        : 19;	/* Frame number			*/
    } bits;				/* normal usage of pte		*/
};


/* Page types */
#define ONBOARD_MEM	0	/* "Onboard" (P2 bus) */
#define ONBOARD_IO	1	/* Onboard devices */
#define VME_16		2	/* VMEbus 16 bit data space */
#define VME_32		3	/* VMEbus 32 bit data space */


/************************************************************************/
/*									*/
/*		FOR TALKING TO THE MMU HARDWARE				*/
/*									*/
/************************************************************************/

typedef unsigned long	Bit32;
typedef unsigned short	Bit16;
typedef unsigned char	Bit8;
typedef long		sBit32;
typedef short		sBit16;
typedef char		sBit8;

/* Routines to read and write function-code-3 space, found in mmu.s */
extern Bit8 Fc3ReadBit8(/*addr*/);		/* read an 8-bit quantity */
extern Bit16 Fc3ReadBit16(/*addr*/);		/* read a 16-bit quantity */
extern Bit32 Fc3ReadBit32(/*addr*/);		/* read a 32-bit quantity */
extern void Fc3WriteBit8(/*addr, value*/);	/* write an 8-bit quantity */
extern void Fc3WriteBit16(/*addr, value*/);	/* write a 16-bit quantity */
extern void Fc3WriteBit32(/*addr, value*/);	/* write a 32-bit quantity */

#define SPACE(space)(space<<28)

/* - MMU entries for virtual address v in the current address space */
#define PageMapAddr(v)		(SPACE(0x1)|(((Bit32)(v))&0x0fffe000))
#define SegMapAddr(v)		(SPACE(0x2)|(((Bit32)(v))&0x0ffe0000))


/* Round an address up to the nearest page boundary */
#define uptopage(addr)\
  ( ((((Bit32) (addr)) - 1) & (~(PGSIZ-1))) + PGSIZ )
/* Round an address up to the nearest segment boundary */
#define uptoseg(addr)\
  ( ((((Bit32) (addr)) - 1) & (~(SEGSIZ-1))) + SEGSIZ )
/* Round an address down to the nearest page boundary */
#define downtopage(addr)\
  ( ((Bit32) (addr)) & ~(PGSIZ-1) )
/* Round an address down to the nearest segment boundary */
#define downtoseg(addr)\
  ( ((Bit32) (addr)) & ~(SEGSIZ-1) )

/* Read and write MMU registers */
#define SetPageMap(addr, value)\
  Fc3WriteBit32(PageMapAddr(addr), (Bit32)(value))
#define SetSegMap(addr, value)\
  Fc3WriteBit8(SegMapAddr(addr), (Bit8)(value))
#define GetPageMap(addr)\
  Fc3ReadBit32(PageMapAddr(addr))
#define GetSegMap(addr)\
  Fc3ReadBit8(SegMapAddr(addr))
#define SetContext(cxt)\
  Fc3WriteBit8(CONTEXT_REGISTER, (Bit8)(cxt))
#define GetContext()\
  Fc3ReadBit8(CONTEXT_REGISTER)
#define GetBusErrorReg()\
  Fc3ReadBit8(BUS_ERROR_REGISTER)
