
/* tftp.h - tftp */

/*
**************************************************************
* TFTP protocol
*    - This file contains the header definitions for the TFTP
*      protocol.
**************************************************************
*/


#define TFTPPORT	69		/* usual tftp server port	*/
#define MAXDATA		512		/* max data in tftp packet	*/
#define MAXUDP		516		/* max udp data in tftp packet	*/
#define TFTPHDR		4		/* tftp data packet header size	*/
#define NETASCII	"netascii"	/* tftp mode netascii		*/
#define OCTET		"octet"		/* tftp mode octet		*/
#define MAIL		"mail"		/* tftp mode mail		*/

/* TFTP opcodes */
#define	TRRQ	001			/* Read Request			*/
#define TWRQ	002			/* Write Request		*/
#define TDATA	003			/* Data packet			*/
#define TACK	004			/* Acknowledgement packet	*/
#define TERROR	005			/* Error packet			*/


/* TFTP error codes */
#define TNOTDEF	000			/* not defined, see err message */
#define TNOTFND	001			/* file not found		*/
#define TACCVIO	002			/* access violation		*/
#define TDKFULL	003			/* disk full, or alloc exceeded */
#define	TILOP	004			/* illegal operation		*/
#define TUNKID	005			/* unkown transfer ID		*/
#define TFEXIST	006			/* file already exists		*/
#define TNOUSER	007			/* No such users		*/


struct tftp {				/* TFTP packet structure	*/
	short	op;			/* tftp opcode			*/
	union	{			/* one of DATA, ACK or ERROR	*/
		short	block;		/* block number - DATA or ACK	*/
		short	errcode;	/* error code - ERROR		*/
		char	fname;		/* first char of filename - RRQ */
	} info;
	union	{			/* one of DATA or ERROR		*/
		char	datastr[MAXDATA];/* data string			*/
		char	errmsg[MAXDATA];/* error message		*/
	} data;
};
