/* avoid #elif since not available in pre-ANSII C--          */
/* pass in SLUCHIP value via compiler args, for ex.,         */
/*   gcc -m68000 -c -DSLUCHIP=6850                           */
/*   or                                                      */
/*   gcc -m68000 -c -DSLUCHIP=7201                           */
/* Then the following will select the chip-specific headers  */
/* If SLUCHIP is not defined, we get the original headers    */
/*  slu.h and sluaccess.h                                    */
/*  and the original function names ttywrite, etc.           */

#ifdef SLUCHIP

#if SLUCHIP==6850
#include <slu6850.h>
#include <sluaccess6850.h>
/* the device-specific function names--used in sources ttywrite.c, etc. */
#define TTYWRITE ttywrite6850
#define TTYPUTC ttyputc6850
#define TTYINIT ttyinit6850
#define TTYIIN ttyiin6850
#define TTYOIN ttyoin6850
#define TTYCNTL ttycntl6850
/* extra code needed to discriminate between input and output interrupts-- */
#define SLUINT sluint6850
#endif  /* SLUCHIP==6850 */

#if SLUCHIP==7201
#include <slu7201.h>
#include <sluaccess7201.h>
/* the device-specific function names--used in sources ttywrite.c, etc. */
#define TTYWRITE ttywrite7201
#define TTYPUTC ttyputc7201
#define TTYINIT ttyinit7201
#define TTYIIN ttyiin7201
#define TTYOIN ttyoin7201
#define TTYCNTL ttycntl7201
/* extra code needed to discriminate between input and output interrupts-- */
#define SLUINT sluint7201
#endif  /* case SLUCHIP==7201 */

#else  /* SLUCHIP not defined: only one, default SLU chip--original case */
#include <slu6850.h>
#include <sluaccess6850.h>
#define TTYWRITE ttywrite
#define TTYPUTC ttyputc
#define TTYINIT ttyinit
#define TTYIIN ttyiin
#define TTYOIN ttyoin
#define TTYCNTL ttycntl
/* extra code needed to discriminate between input and output interrupts-- */
#define SLUINT sluint
#endif

