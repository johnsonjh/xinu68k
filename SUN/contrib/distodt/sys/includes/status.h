
/* 
 * status.h - Header file for status program
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Feb 25 1988
 * Copyright (c) 1988 Jim Griffioen
 */



#define GENERIC_ERR	1		/* generic error constant	*/
#define GENERIC_OK	0		/* generic ok constant		*/

#define BIGBUFSIZE 	40000		/* size of a large buffer       */

#define SMALLSTR	32		/* generic small string length	*/
#define MEDSTR		128		/* generic medium str length	*/
#define LONGSTR		512		/* generic long str length	*/
#define XLONGSTR	1024		/* generic xtra-long str length	*/
#define XSTIMEOUT	1		/* x-small time-out period	*/
#define STIMEOUT	2		/* small time-out period	*/
#define MTIMEOUT	5		/* medium time-out period	*/
#define LTIMEOUT	10		/* long time-out period		*/
#define XLTIMEOUT	240		/* x-long time-out period	*/
#define S_MAXTRIES	2		/* small maximum number retries	*/
#define M_MAXTRIES	5		/* medium number of retries	*/
#define L_MAXTRIES	10		/* large number of retries	*/
#define S_PAUSE_TIM	1		/* small pause between retries	*/
#define M_PAUSE_TIM	2		/* medium pause between retries	*/
#define L_PAUSE_TIM	5		/* large pause between retries	*/


