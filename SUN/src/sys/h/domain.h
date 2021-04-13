/* domain.h - dn_cat */

#ifndef	NSERVER
#define	NSERVER		"128.10.2.8:53"	/* Server IP address & UDP port	*/
#endif

/* Definitions of bits in the operation and parameter field */

#define	DN_QR		0100000		/* Query (0) or request (1) bit	*/
#define	DN_OPCDE	0074000		/* operation  code for query:	*/
					/*  0 => standard query		*/
					/*  1 => inverse query,		*/
					/*  2 => completion query,	*/
					/*  3 => completion (1 answer),	*/
					/*  4-15 => reserved.		*/
#define	DN_AA		0002000		/* 1 if authoritative answer	*/
#define	DN_TR		0001000		/* 1 if message truncated	*/
#define	DN_RD		0000400		/* 1 if recursion desired	*/
#define	DN_RA		0000200		/* 1 if recursion available	*/
#define	DN_RESVD	0000160		/* reserved			*/
#define	DN_RESP		0000017		/* response code:		*/
					/*  0 => no errors in query	*/
					/*  1 => format error in query	*/
					/*  2 => server failure		*/
					/*  3 => name does not exist	*/
 
struct	dn_mesg	{			/* domain system message format	*/
	short	dn_id;			/* message id			*/
	short	dn_opparm;		/* operation and parmameter bits*/
	short	dn_qcount;		/* # entries in question seciton*/
	short	dn_acount;		/* # RRs in answer section	*/
	short	dn_ncount;		/* # RRs in nameserver section	*/
	short	dn_dcount;		/* # RRs in additional section	*/
	char	dn_qaaa[1];		/* start of rest of the message	*/
	/* remaining fields of the domain name message are of variable	*/
	/* length, and consist of (1) a question section, (2) an answer	*/
	/* section, (3) an authority section (which says where to find	*/
	/* answers when they cannot be supplied), and (4) an addition	*/
	/* information section.  Entries in these are Resource Records.	*/
};

struct	dn_qsuf	{			/* question section name suffix	*/
	short	dn_type;		/* type of this name		*/
	short	dn_clas;		/* class of this name		*/
};

#define	DN_MLEN		128		/* message length (small query)	*/
#define	dn_cat(t,f)	{*t++ =(char)strlen(f);strcpy(t,f);t+=strlen(f);}

/* Query type codes */

#define	DN_QTHA		1		/* Host address			*/
#define	DN_QTNS		2		/* Authoratative name server	*/
#define	DN_QTMD		3		/* Mail destination (obsolete)	*/
#define	DN_QTMF		4		/* Mail forwarder (obsolete)	*/
#define	DN_QTCN		5		/* Canonical name for an alias	*/
#define	DN_QTSZ		6		/* Start of zone of authority	*/
#define	DN_QTMB		7		/* Mailbox domain name		*/
#define	DN_QTMG		8		/* Mail group member		*/
#define	DN_QTMR		9		/* Mail rename domain name	*/
#define	DN_QTNL		10		/* Null resource record		*/
#define	DN_QTWK		11		/* Well-known service descriptor*/
#define	DN_QTPR		12		/* Domain name pointer		*/
#define	DN_QTHI		13		/* Host information		*/
#define	DN_QTMI		14		/* Mailbox or mail list info.	*/
#define	DN_QTMX		15		/* Mail, replaces MD & MF	*/

/* Query class codes */

#define	DN_QCIN		1		/* The DARPA Internet		*/
#define	DN_QCCS		2		/* CSNET (now obsolete)		*/
#define	DN_QCHA		3		/* Chaos network		*/

#define	DN_CMPRS	0300		/* Compressed format is pointer	*/
#define	DN_CPTR		037777		/* Compressed format bits of ptr*/
#define	DN_RLEN		10		/* resource record heading len.	*/
