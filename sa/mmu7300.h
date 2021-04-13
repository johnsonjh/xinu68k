/* mmu7300.h -- Registers relevant to the memory management unit 
   of the ATT7300 */

/* general status reg-- */
/* parity errors, MMU errors and processor bus errors cause this
   info to be frozen, describing the error, until the clear-status
   signal is received by writing to address 4C0000 */

#define GSR ((volatile unsigned short *)0x410000)
#define GSR_CLEAR_STAT  ((volatile unsigned short *)0x4c0000)
/* bits for GSR-- */
/* 0=write, 1- read cycle */
#define READ_WRITE (1<<14)
/* 1 = nonprocessor cycle */
#define NPC (1<<13)
/* 0 = page fault */
#define PAGE_FAULT (1<<12)
/* 1 = parity interrupts enabled */
#define PARITY_IE (1<<11)
/* 0 = user access to memory above 4MB */
#define USER_WILD_ACCESS (1<<8)

/* bus status reg */
/* latched during NMI or BERR, not cleared on reset */
#define BSR ((volatile unsigned short *)0x430000)
/* 0 = MMU error, page fault, user write to write-protected page, or
   user access to kernel area, 0-512K */
#define MMUERR (1<<15)
/* bits 10-14 for DMA cycles */
/* 0=upper byte access */
#define UDS (1<<9)
/* 0 = lower byte access */
#define LDS (1<<8)
/* processor address bit 23 */
#define PA23 (1<<7)
#define PA22 (1<<6)
/* logical address bits 16-21 are in bits 0-5 */

/* bus status reg 1 */
/* has address bits 0-15 latched during NMI or BERR */
/* not cleared on reset */
#define BSR1 ((volatile unsigned short *)0x440000)

/* general control reg--bit 15 of each of these reg's */
/* write-only register */
#define GCRBIT (1<<15)
/* 1 = error enable (level 7 ints, bus errors) */
#define GCR_ERRENA ((volatile short *)0xe40000)
/* 1 = parity error circuit and interrupts are enabled */
#define GCR_PARITY_IE ((volatile short *)0xe41000)
/* 1 = memory-write with forced parity error--for diagnostics */
#define GCR_BAD_PARITY_WRITE ((volatile short *)0xe42000)
/* 0 = force processor into ROM addr sp, for reset cycle handling */
#define GCR_ROMLMAP ((volatile short *)0xe43000)
/* 0 = modem connected to line 1 */
#define GCR_L1_MODEM ((volatile short *)0xe44000)
/* 0 = modem connected to line 2 */
#define GCR_L2_MODEM ((volatile short *)0xe45000)
/* 0 = dial network conn. to line 1 */
#define GCR_DNCONN ((volatile short *)0xe46000)

/* two-byte PTE, for 4K page */
typedef short PTE;
/* status bits in PTE--(not clear in hardware manual, but determined
   from circuit diagram, sheet 16 and experiment) */
#define PAGE_WRITE_ENABLE (1<<15)
/* bits 13 and 14 together form a 2-bit page status field-- */
#define PTE_PAGE_STATUS (3<<13)
/* codes in page status field-- */
#define PAGE_NOT_PRESENT (0<<13)
#define PAGE_PRESENT_NOT_ACCESSED (1<<13)
#define PAGE_ACCESSED (2<<13)
#define PAGE_DIRTY (3<<13)
/* the 10-bit page frame number is in the least significant bits of the PTE */

#define PAGE_TABLE ((volatile PTE *)0x400000)
