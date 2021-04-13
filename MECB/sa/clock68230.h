/* clock68230.h--same as clock.h for the MECB-only setup */
/* format of the memory mapped registers for timer part of 68230 PI/T */

struct timercsr {
  unsigned char tcsr;		/* timer control reg */
  char dummy1;	
  unsigned char tivr;		/* timer interrupt vector addr */
  char dummy2;
  unsigned char cpr_null;	/* r/w but noop 4th byte for cpr */
  char dummy3;
  unsigned char cpr_high;	/* high byte of counter preload reg */
  char dummy4;
  unsigned char cpr_med;	/* middle byte of counter preload reg */
  char dummy5;
  unsigned char cpr_low;	/* low byte of counter preload reg */
  char dummy6;
  unsigned char cnt_null;	/* r/w but noop 4th byte for counter reg */
  char dummy7;
  unsigned char cnt_high;	/* high byte, counter--read with clock off */
  char dummy8;
  unsigned char cnt_med;	/* mid. byte, counter--read with clock off */
  char dummy9;
  unsigned char cnt_low;	/* low byte, counter--read with clock off */
  char dummy10;
  unsigned char tstat;		/* bit 0 is zero-detect, rest read as 0 */
				/* write 1 to this byte to acknowledge tick */
};

/* the memory-mapped location of timer registers, determined by digital */
/* logic wired onto the mecb--(would be at higher adddress on bigger sys) */
#define TIMER ((struct timercsr *)0x10021)

/* bit defs for tcr (timer control register)-- */
#define CLOCKIE (5<<5)		/* user vectoring, int enabled */
#define CLOCKDE (4<<5)		/* user vectoring, int disabled */
#define LOADFROMCPR (0<<4)	/* when count hits 0 */
#define LETROLLOVER (1<<4)	/* don't use CPR, just let it roll over */
#define PRESCALE32 (0<<1)	/* count every 32 clock cycles */
				/* 1<<1 thru 3<<2 all involve a */
				/* pin on port C, for external clocking */
#define CLOCKON (1<<0)		/* clock running */
#define CLOCKOFF (0<<0)		/* clock not running */

/* Note: since PRESCALE32 is the only internal-only setting for bits 1-2, */
/* we have as a result a timer downcounting every 32 cycles of the 4 Mhz  */
/* crystal, i.e, 8 usecs/downcount, or 125,000 downcounts/second          */

