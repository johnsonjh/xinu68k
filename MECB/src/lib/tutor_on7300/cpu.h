/* there are 180 bytes of registers on a 68020 w/68881      */
/* many of the fpa registers are 12 byte (96 bit) registers */

/* for 68000/68010 w/o 68881, there are 17 4byte +1 2byte = 70, */
/* but we round up to mult of 4-- */
#define NUMREGBYTES 72
enum regnames {D0,D1,D2,D3,D4,D5,D6,D7, 
               A0,A1,A2,A3,A4,A5,A6,A7, 
               PS,PC
              };
/* PS has SR in high word */
#define PS_TBIT 0x8000


