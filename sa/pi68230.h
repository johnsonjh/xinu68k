/* pi.h-- header file for the parallel interface part of the 68230 PI/T */
/*  i.e. parallel interface for the mecb */
/*  See clock.h for the timer part */
/*  Note that although this file is quite complicated, it in fact only
    covers completely "MODE0, SUBMODE1X" operation of the PI, its simplest
    mode of operation (the only mode currently used in CS241.) */

struct picsr {
  unsigned char pgcr;		/* port general control reg */
  char dummy1;
  unsigned char psrr;		/* port service request reg */
  char dummy2;
  unsigned char portA_ddr;	/* port A data direction reg */
  char dummy3;
  unsigned char portB_ddr;	/* port B data direction reg */
  char dummy4;
  unsigned char portC_ddr;	/* port C data direction reg */
  char dummy5;
  unsigned char pivr;		/* port interrupt vector reg */
  char dummy6;
  unsigned char portA_cr;	/* port A control reg */
  char dummy7;
  unsigned char portB_cr;	/* port B control reg */
  char dummy8;
  unsigned char portA_data;	/* port A data reg */
  char dummy9;
  unsigned char portB_data;	/* port B data reg */
  char dummy10;
  unsigned char portA_alt;	/* port A alternate reg */
  char dummy11;
  unsigned char portB_alt;	/* port B alternate reg */
  char dummy12;
  unsigned char portC_data;	/* port C data reg */
  char dummy13;
  unsigned char psr;		/* port status reg */
};

/* the memory-mapped location of PI registers, determined by digital */
/* logic wired onto the mecb--(would be at higher address on bigger sys) */
#define PI ((volatile struct picsr *)0x10001)

/* bit defs for pgcr (port general control register)-- */
/* bits 6-7 are for specifying the "mode", simplest is MODE0 */
/* bit 4 is the Port B enable bit--turn on after other reg's set up */
/* bit 3 is the Port A enable bit--turn on after other reg's set up */
/* bits 0-3 control whether the handshake lines are active low or high */
/* First write pgcr with just the mode bits set, then set up other regs, */
/* then write pgcr with mode bits as well as other bits set up */
/*  for ex. PGCR_DEFAULT|H12ENA to use just port A */
#define PGCR_DEFAULT MODE0	/* 8-bit ports, all four handshake */
				/* lines active low */
#define MODE0 (0<<6)		/* 8-bit ports, i/o direction from ddr */
#define MODE1 (1<<6)		/* A&B make 16 bit port, dir. from ddr */
#define MODE2 (2<<6)		/* 8-bit ports, no ddr function */
#define MODE3 (3<<6)		/* A&B make 16 bit port, no ddr fn */
#define H34ENA (1<<5)		/* Port B (handshake lines H3,4) enable */
#define H34DIS (0<<5)		/* proper val while setting portB_cr */
#define H12ENA (1<<4)		/* Port A (handshake lines H1,2) enable */
#define H12DIS (0<<4)		/* proper val while setting portA_cr */
#define H1234HIGH (0xf)		/* all 4 active high--(for ex.) */

/* bit defs for psrr (port service request reg) */
#define PSRR_DEFAULT 0		/* no DMA requests, no interrupts */
#define VEC_INTS (3<<3)		/* vectored interrupts selected */
/* Using VEC_INTS does not ENABLE them, just selects that method */
/*  over autovectored ints, when and if enabled in the port crs */
/* The mecb is wired up for vectored ints, not autovectored ints */
/* bits 0-2 determine the relative priorities of the 4 possible int's */
/* val 0 specifies H1S highest, then H2S, H3S, H4S */

/* Port data direction registers (ddrs).  First note that port A on
   the mecb is "buffered for output", i.e., has a logic gate on each
   data line which has been put there to strengthen the output signal
   but has the side effect of disallowing input.  Port B is wired directly
   to the 50-pin edge connector, and thus can be used for input or output.
   Port C is essentially useless for i/o.
   In modes 0 and 1, the ddr registers determine which bits of a port
   are used for input and which for output.  A zero in bit i of a ddr
   defines the bit as an input, a one defines it as an output. */

/* pivr--see intvectors.h.  Used only if interrupts are enabled */

/* bit defs for port control regs--for MODE0 in pgcr */
#define SUBMODE0 (0<<6)		/* double-buf input/single-buf output */
#define SUBMODE1 (1<<6)		/* double-buf output/instantaneous input */
#define SUBMODE1X (2<<6)	/* bit i/o: single-buf out/instant. input */

/* bit defs for rest of port A control reg, case SUBMODE1X--
   just handshake line config's, so ignore these and use PACR_DEFAULT
   unless you want to use actively these extra two lines.  If you do
   want to use them, be aware that H1 on the mecb is wired directly
   to the 50-pin edge connector but H2 is buffered for output, so we
   cannot use it for input unless we wire over the buffer gate U1B.
   H1 can be used only for input in submode 1X by 68230 design. */
/* In SUBMODE1X, bits 0 and 4 are not used and can have either value */
#define H1IE (1<<1)		/* interrupt on H1 (input) enabled */
#define H2IE (1<<2)		/* interrupt on H2 (input) enabled--
				   not useful on an unmodified mecb */
#define H2OUT (1<<5)		/* make H2 an output line (it's buffered that
				 way on mecb anyway)--use H2OUTLEVEL to
				 control logic level */
#define H2OUTLEVEL (1<<3)	/* 0 for H2 negated, 1 for H2 asserted--
				   using active high/low as spec. in pgcr */

#define PACR_DEFAULT (SUBMODE1X|H2OUT) /* no interrupts, handshake line H1 */
				/* is an edge-sensitive input line with */
				/* H1S in pasr set following asserted edge; */
				/* H2 is an output handshake line */

/* Bit defs for rest of port B control reg, case SUBMODE1X-- */
/* as with port A, H3 is direct-wired but H4 is buffered for output on mecb.*/
/* Bits are in same places as pacr but have conventionally different names--*/

#define H3IE (1<<1)		/* interrupt on H3 (input) enabled */
#define H4IE (1<<2)		/* interrupt on H4 (input) enabled--
				   not useful on an unmodified mecb */
#define H4OUT (1<<5)		/* make H4 an output line (it's buffered that
				   way on mecb anyway)--use H4OUTLEVEL to
				   control logic level */
#define H4OUTLEVEL (1<<3)	/* 0 for H4 negated, 1 for H4 asserted */

#define PBCR_DEFAULT (SUBMODE1X|H4OUT) /* no interrupts, handshake line H3 */
				/* is an edge-sensitive input line with */
				/* H3S in psr set following asserted edge; */
				/* H4 is an output handshake line */

/* bit defs for psr (port status reg) */

#define H4_SHIFT 7		/* H4 instantaneous level, disregarding */
#define H4 (1<<H4_SHIFT)	/*  pin sense def in pgcr,i.e., absolute--
				    not useful for unmodified mecb */
#define H3_SHIFT 6		/* H3 instant. level, disregarding */
#define H3 (1<<H3_SHIFT)	/*  pin sense def in pgcr, i.e., absolute */
#define H2_SHIFT 5		/* H2 instant. level, ... */
#define H2 (1<<H2_SHIFT)	/*  --not useful on an unmodified mecb */
#define H1_SHIFT 4		/* H1 instant. level, ... */
#define H1 (1<<H1_SHIFT)
#define H4S_SHIFT 3		/* H4 status: for MODE0,SUBMODE1X, set by */
#define H4S (1<<H4S_SHIFT)	/*  asserted edge of H4 if config'd for input
				    and cleared by writing 1 to H4S bit--
				    not useful for unmodified mecb */
#define H3S_SHIFT 2		/* H3 status: for MODE0,SUBMODE1X, set by */
#define H3S (1<<H3S_SHIFT)	/*  asserted edge of H3 input, and cleared */
				/*  by writing 1 to H3S bit */
#define H2S_SHIFT 1		/* H2 status: for MODE0,SUBMODE1X, set by*/
#define H2S (1<<1)		/* asserted edge of H2 if config'd for input
				    and cleared by writing 1 to H2S bit
				    --not useful on an unmodified mecb */
#define H1S_SHIFT 0		/* H1 status: for MODE0,SUBMODE1X, set by */
#define H1S (1<<0)		/*  asserted edge of H1 input, and cleared */
				/*  by writing 1 to H1S bit */

	    /* See intvectors.h for PI interrupt vector setup  */
