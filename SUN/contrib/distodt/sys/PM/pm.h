#include <stdio.h>
#include <sys/types.h>
#include "x.h"

/*
 * pm - external variables, structure definitions
 */

extern	char		*text;
extern	char		*core;
extern	FILE		*txtfd;
extern	FILE		*corefd;
extern	char		*txtfile;
extern	char		*corefile;
extern	struct nlist	*symtab, *esymtab;
extern	struct nlist	*ssymtab, *essymtab;
extern	char		*strtab;
extern	struct nlist	nl[];
extern	struct coreX	c_header;
extern	struct exec	a_out;
extern	int		errflg;
extern	int		allopts, copt, mopt, popt, sopt, topt, ropt;
extern	int		verbose;


typedef	int		INT;

typedef	long		L_INT;
typedef short		boolean;

struct reglist{
	char	*rname;
	INT	roffs;
};

#define	EVEN	~01
#define MAXOFF	1023	/* maximum reasonable displacement from symbol */
#define	MAXARGS	6	/* maximum reasonable number of arguments */
#define	RECLIMIT 20	/* maximum reasonable recursion depth */
#define GARBAGELIMIT 5	/* maximum number of ?garbage? stack frames to print */

/* errflg values */
#define	NOCFN	1

/* where Sun-3 Xinu starts */
#ifdef V8
#define SUN3START 0x0fd04000
#else
#define SUN3START 0x4000
#endif
