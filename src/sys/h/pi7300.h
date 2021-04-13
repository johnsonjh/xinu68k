/* pi7300.h   header for the ATT7300's parallel interface */

/* The pi is ready to use with no special initialization.  Basically
   it has only one mode of operation, so there is nothing to specify.
   After writing a data byte to PI_DATA, the LP_STROBE bit must be toggled
   from high to low and back to high to "strobe the data to the printer",
   i.e., this high-low-high signal is passed out through the port (on pin 1) 
   to the printer and tells it that a byte is ready for printing.
   Then the printer acknowledges acceptance of the byte by toggling
   the Acknowledge signal from high to low and back to high, on pin 10.
   Although undocumented in the register description, this signal can
   be seen in bit 9 of the GSR (found by reading the circuit diagram,
   sheet 6, unit 24D, the latch for the GSR).  More importantly for us,
   the low->high transition causes an interrupt, autovectored, level 2.
   The interrupt is acknowledged to the hardware by writing to data reg.
   (It doesn't have to be the "next byte", since the printer will ignore
   data written to the port if there is no strobe signal.) */ 

/* the 8-bit data port, output only-- */
#define PI_DATA ((volatile unsigned char *)0x4f0000)
/* Note: MCR is also used for clock interrupt control--should have package
   controlling updates to it like we have for the console control reg */
#define MCR ((volatile unsigned short *)0x4a0000)
/* one bit in MCR for lp--data strobe */
#define LP_STROBE (1<<13)
/* status register--input-only bits */
#define PI_STATUS ((volatile unsigned short *)0x470000)
/* bits in status reg--not needed to do the basic output operation,
   but needed for features such as stopping when the paper runs out */
/* from pin 11 of the interface: 1 if busy (printing, set offline, or in error
   condition)-- */
#define LP_BUSY (1<<7)
/* from pin 13 of the interface, 1 if printer online, but may be 1 all the 
   time, so not terribly useful--use LP_BUSY or LP_ERROR instead */
#define LP_SELECT (1<<6)
/* from pin 12 of the interface: 1 if the printer is out of paper */
#define LP_PAPEREND (1<<5)
/* from pin 32 of the interface: 0 if the printer is out of paper, offline,
    or in error state */
#define LP_ERROR (1<<4)
/* The rest of the bits relate to other devices */

/* The PI interrupts using autovector for level 2 */
/* (is this the right type spec??) */
#define PI_INTVEC_ADDR ((int (**)())(0x68))

