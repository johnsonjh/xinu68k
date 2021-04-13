/*	@(#)sunromvec.h 2.30 85/08/13 SMI	*/

/*
 * Copyright (c) 1985 by Sun Microsystems, Inc.
 */

#ifndef MONSTART
/*
 * sunromvec.h
 *
 * This file defines the entire interface between the PROM Monitor
 * and programs (or kernels) that run under it.
 *
 * The main interface is the vector table at the front of the boot rom.
 *
 * This vector table is the only knowledge the outside world has of this prom.
 * It is referenced by hardware (Reset SSP, PC), and software.
 * Once located, no entry can be re-located unless you change the world
 * that needs it.
 * 
 * The easiest way to reference elements of this vector is to say:
 *
 *	*romp->xxx
 *
 * as in:
 *
 *	(*romp->v_putchar) (c);
 *
 * This is pretty cheap as it only generates   movl 0xNNNNNNNN:l,a0; jsr a0@ .
 * That's 2 bytes longer, and 4 cycles longer, than a simple subroutine call.
 *
 * Various entries have been added at various times.  As of Rev N PROM
 * Monitors, the vector table includes an entry "v_romvec_version" which
 * is an integer defining which entries in the table are valid.  The
 * comments on each entry say which version the entry first appeared in
 * with a prefix like "V1:".  To determine if the PROMs your program is
 * running under contain the entry, you can simply compare the value
 * of v_romvec_version to the constant in the comment field, e.g.
 *
 *	if (romp->v_romvec_version >= 1)
 *		{ reference *romp->v_memorybitmap...}
 *	else
 *		{ running under older prom, punt somehow }
 *
 * Entries which do not contain a "Vn:" string are in Version 0 of the
 * proms (Rev N and onward).
 */

struct sunromvec {
	char		*v_initsp;		/* Initial SSP for hardware */
	void		(*v_startmon)();	/* Initial PC  for hardware */
	int		*v_diagberr;		/* Bus err handler for diags */

/* Configuration information passed to standalone code & Unix */
	struct bootparam	**v_bootparam;	/* Info for bootstrapped pgm */
 	unsigned	*v_memorysize;		/* Total phys memory in bytes */

/* Single-character input and output */
	unsigned char	(*v_getchar)();		/* Get char from input source */
	void		(*v_putchar)();		/* Put char to output sink */
	int		(*v_mayget)();		/* Maybe get char, or -1 */
	int		(*v_mayput)();		/* Maybe put char, or -1 */
	unsigned char	*v_echo;		/* Should getchar echo? */
	unsigned char	*v_insource;		/* Input source selector */
	unsigned char	*v_outsink;		/* Output sink selector */

/* Keyboard input and frame buffer output */
	int		(*v_getkey)();		/* Get next key if one exists */
	void		(*v_initgetkey)();	/* Like it says */
	unsigned int	*v_translation;		/* Kbd translation selector */
	unsigned char	*v_keybid;		/* Keyboard ID byte */
	int		*v_screen_x;		/* V2: Screen x pos (R/O) */
	int		*v_screen_y;		/* V2: Screen y pos (R/O) */
	struct keybuf	*v_keybuf;		/* Up/down keycode buffer */

	char		*v_mon_id;		/* Revision level of monitor */

/* Frame buffer output and terminal emulation */
	void		(*v_fwritechar)();	/* Write a character to FB */
	int		*v_fbaddr;		/* Address of frame buffer */
	char		**v_font;		/* font table for FB */
	void		(*v_fwritestr)();	/* Quickly write string to FB */

/* Reboot interface routine -- resets and reboots system.  No return. */
	void		(*v_boot_me)();		/* eg boot_me("xy()vmunix") */

/* Line input and parsing */
	unsigned char	*v_linebuf;		/* The line input buffer */
	unsigned char	**v_lineptr;		/* Cur pointer into linebuf */
	int		*v_linesize;		/* length of line in linebuf */
	void		(*v_getline)();		/* Get line from user */
	unsigned char	(*v_getone)();		/* Get next char from linebuf */
	unsigned char	(*v_peekchar)();	/* Peek at next char */
	int		*v_fbthere;		/* =1 if frame buffer there */
	int		(*v_getnum)();		/* Grab hex num from line */

/* Phrase output to current output sink */
	int		(*v_printf)();		/* Similar to "Kernel printf" */
	void		(*v_printhex)();	/* Format N digits in hex */

	unsigned char	*v_leds;		/* RAM copy of LED register */
	void		(*v_set_leds)();	/* Sets LED's and RAM copy */

/* nmi related information */
	void		(*v_nmi)();		/* Addr for level 7 vector */
	void		(*v_abortent)();	/* entry for keyboard abort */
	int		*v_nmiclock;		/* counts up in msec */

	int		*v_fbtype;		/* FB type: see <sun/fbio.h> */

/* Assorted other things */
	unsigned	v_romvec_version;	/* Version # of Romvec */ 
	struct globram	*v_gp;			/* monitor global variables */
	struct zscc_device 	*v_keybzscc;	/* Addr of keyboard in use */

	int		*v_keyrinit;		/* ms before kbd repeat */
	unsigned char	*v_keyrtick; 		/* ms between repetitions */
	unsigned	*v_memoryavail;		/* V1: Main mem usable size */
	long		*v_resetaddr;		/* where to jump on a reset */
	long		*v_resetmap;		/* pgmap entry for resetaddr */
						/* Really struct pgmapent *  */
	void		(*v_exit_to_mon)();	/* Exit from user program */

	unsigned char	**v_memorybitmap;	/* V1: &{0 or &bits} */
	void		(*v_setcxsegmap)();	/* Set seg in any context */
	void		(**v_vector_cmd)();	/* V2: Handler for 'v' cmd */
	int		dummy1z;
	int		dummy2z;
	int		dummy3z;
	int		dummy4z;
};

/*
 * THE FOLLOWING CONSTANT MUST BE CHANGED ANYTIME THE VALUE OF "PROM_BASE"
 * IN cpu.addrs.h IS CHANGED.  IT IS CONSTANT HERE SO THAT EVERY MODULE
 * WHICH NEEDS AN ADDRESS OUT OF SUNROMVEC DOESN'T HAVE TO INCLUDE cpu.addrs.h.
 */
#define	romp	((struct sunromvec *)0x0FEF0000)
/*
 * Note that the value of romp will be truncated based on the running
 * hardware:
 *		Sun-2	0x00EF0000
 *		Sun-3	0x0FEF0000
 * This was deliberately done for Sun-3 so that programs using this
 * header file (with Sun-3 support) will continue to run in Sun-2 systems.
 */

/*
 * The currently possible values for *romp->v_insource and *romp->v_outsink:
 * Note that these may be extended in the future.  Your program should
 * cope with this gracefully (e.g. by continuing to vector thru the
 * PROM I/O routines if these are set in a way you don't understand).
 */
#define	INKEYB		0		/* Input from parallel keyboard */
#define	INUARTA		1		/* Input or output to Uart A */
#define	INUARTB		2		/* Input or output to Uart B */

#define	OUTSCREEN	0		/* Output to frame buffer */
#define	OUTUARTA	1		/* Input or output to Uart A */
#define	OUTUARTB	2		/* Input or output to Uart B */

/*
 * Structure set up by boot command to pass arguments to program booted.
 */
struct bootparam {
	char		*bp_argv[8];	/* string arguments */
	char		bp_strings[100];/* string table for string arguments */
	char		bp_dev[2];	/* device name */
	int		bp_ctlr;	/* controller # */
	int		bp_unit;	/* unit # */
	int		bp_part;	/* partition/file # */
	char		*bp_name;	/* file name, points into bp_strings */
	struct boottab	*bp_boottab;	/* Points to table entry for dev */
};


/*
 * This table entry describes a device.  It exists in the PROM; a
 * pointer to it is passed in bootparam.  It can be used to locate
 * PROM subroutines for opening, reading, and writing the device.
 * NOTE: When using this interface, only ONE device can be open at once.
 * You can't open a tape and a disk.  Sorry.
 */
struct boottab {
	char	b_dev[2];		/* Two char name of dev */
	int	(*b_probe)();		/* probe() --> -1 or found ctlr # */
	int	(*b_boot)();		/* boot(bp) --> -1 or start address */
	int	(*b_open)();		/* open(iobp) --> -1 or 0 */
	int	(*b_close)();		/* close(iobp) --> -1 or 0 */
	int	(*b_strategy)();	/* strategy(iobp,rw) --> -1 or 0 */
	char	*b_desc;		/* Printable string describing dev */
	struct devinfo *b_devinfo;	/* Info to configure device */
};

/* This defines the set of map entry types */
enum MAPTYPES {
	MAP_MAINMEM, MAP_OBIO, MAP_MBMEM, MAP_MBIO,
	MAP_VME16A16D, MAP_VME16A32D,
	MAP_VME24A16D, MAP_VME24A32D,
	MAP_VME32A16D, MAP_VME32A32D,
};

/*
 * This table entry gives information about the resources needed by a
 * device.  Maybe it should be merged into the struct boottab, but
 * then the struct boottab for each dev should be merged into that
 * dev's foo.c file rather than some global config file.
 */
struct devinfo {
	unsigned	d_devbytes;	/* Bytes occupied by dev in IO space */
	unsigned	d_dmabytes;	/* Bytes needed by dev in DMA memory */
	unsigned	d_localbytes;	/* Bytes needed by dev for local info */
	unsigned	d_stdcount;	/* How many standard addresses */
	unsigned long	*d_stdaddrs;	/* The vector of standard addresses */
	enum MAPTYPES 	d_devtype;	/* What map space dev is in */
	unsigned	d_maxiobytes;	/* Size to break big I/O's into */
};

/*
 * This defines the memory map interface between the PROM Monitor
 * and the Unix kernel.
 */

/*
 * The PROM Monitor requires that nobody mess with parts of virtual
 * memory if they expect any PROM Monitor services.  These rules
 * apply to all the virtual memory between MONSTART and MONEND:
 *
 *   *	Do not write to these addresses.
 *   *  Do not read from (depend on the contents of) these addresses, except
 *	as documented here or in <mon/sunromvec.h>.
 *   *	Do not remap these addresses.
 *   *	Do not change or double-map the pmegs that these addresses
 *	map through.
 *   *	Do not change or double-map the main memory that these
 *	addresses map to.
 *   *	You are free to change or double-map I/O devices which these
 *	addresses map to.
 *   *	These rules apply in all map contexts.
 * 
 * Other than that, you can do whatever you want with them.
 */
#if defined(SUN2) || defined(sun2)
#define	MONSTART	0x00E00000
#define	MONEND		0x00F00000
#endif

#if defined(SUN3) || defined(sun3)
#define	MONSTART	0x0FE00000
#define	MONEND		0x0FF00000
#endif

/*
 * The one page at MONSHORTPAGE must remain mapped to the same piece
 * of main memory.  The pmeg used to map it there can be changed if
 * desired.  This page should not be read from or written in.  It is
 * used for ROM Monitor globals, since it is addressable with short
 * absolute instructions.  (We give a 32-bit value below so the
 * compiler/assembler will use absolute short addressing.  The hardware
 * will accept either 0 or 0xF as the top 4 bits.)
 */
#define	MONSHORTPAGE	0xFFFFE000
#define	MONSHORTSEG	0xFFFE0000

/*
 * For virtual addresses outside the above range, you can remap
 * the addresses as desired (but see MONSHORTPAGE).
 *
 * Any pmeg not referenced by segments between MONSTART and MONEND
 * can be used freely (but see MONSHORTPAGE).
 *
 * When a standalone program is booted, available main memory
 * will be mapped contiguously, using ascending physical page addresses,
 * starting at virtual location 0.  The complete set of available
 * memory may not be mapped due to a lack of PMEGs.  The complete set
 * is defined by v_memorysize, v_memoryavail, and v_memorybitmap.
 * We guarantee that at least MAINMEM_MAP_SIZE page map entries
 * will be available for mapping starting from location 0.
 * 
 * When a standalone program is booted, at least DVMA_MAP_SIZE bytes
 * of segment map entries will be allocated, so that pages can be mapped
 * into DVMA space.
 */
#if defined(SUN2) || defined(sun2)
#define	MAINMEM_MAP_SIZE	0x00600000
#define	DVMA_MAP_SIZE		0x00080000
#endif

#if defined(SUN3) || defined(sun3)
#define	MAINMEM_MAP_SIZE	0x00600000
#define	DVMA_MAP_SIZE		0x00080000
#endif

/*
 * The following are included for compatability with previous versions
 * of this header file.  Names containing capital letters have been
 * changed to conform with "Bill Joy Normal Form".  This section provides
 * the translation between the old and new names.  It can go away once
 * Sun-1 applications have been converted over.
 */
#define	RomVecPtr	romp
#define	v_SunRev	v_mon_id
#define	v_MemorySize	v_memorysize
#define	v_EchoOn	v_echo
#define	v_InSource	v_insource
#define	v_OutSink	v_outsink
#define	v_InitGetkey	v_initgetkey
#define	v_KeybId	v_keybid
#define	v_Keybuf	v_keybuf
#define	v_FBAddr	v_fbaddr
#define	v_FontTable	v_font
#define	v_message	v_printf
#define	v_KeyFrsh	v_nmi
#define	AbortEnt	v_abortent
#define	v_RefrCnt	v_nmiclock
#define	v_GlobPtr	v_gp
#define	v_KRptInitial	v_keyrinit
#define	v_KRptTick	v_keyrtick
#define	v_ExitOp	v_exit_to_mon
#define	v_fwrstr	v_fwritestr
#define	v_linbuf	v_linebuf

#endif MONSTART
