# define DEFBRK 257
# define COLON 258
# define OCTAL 259
# define INTEGER 260
# define IDENT 261
# define CSR 262
# define IVEC 263
# define OVEC 264
# define IINT 265
# define OINT 266
# define INIT 267
# define OPEN 268
# define CLOSE 269
# define READ 270
# define WRITE 271
# define SEEK 272
# define CNTL 273
# define IS 274
# define ON 275
# define GETC 276
# define PUTC 277

# line 4 "config.y"
#include <stdio.h>

#define	NIL	(dvptr)0

#define	CONFIGC		"../sys/conf.c"		/* name of .c output	*/
#define	CONFIGH		"../h/conf.h"		/* name of .h output	*/
#define	CONFHREF	"<conf.h>"		/* how conf.h referenced*/
#define	CONFIGIN	"Configuration"		/* name of input file	*/

FILE	*confc;
FILE	*confh;

char	*dbstr;
extern	char *malloc();
int	ndevs = 0;
int	currname = -1;
int	currtname = -1;
int	currdname = -1;
int	brkcount = 0;

struct	syment	{			/* symbol table			*/
	char	*symname;
	int	symoccurs;
	} symtab[250];

int	nsym = 0;
int	lookup();
int	linectr = 1;
char	*doing = "device type declaration";
char	*s;
struct	dvtype {
	char		*dvname;	/* device name (not used in types)*/
	char		*dvtname;	/* type name			*/
	int		dvtnum;		/* symbol table index of type	*/
	char		*dvdevice;	/* device name			*/
	int		dvcsr;		/* Control Status Register addr	*/
	int		dvivec;		/* input interrupt vector	*/
	int		dvovec;		/* Output interrupt vector	*/
	char		dviint[20];	/* input interrupt routine	*/
	char		dvoint[20];	/* output interrupt routine	*/
	char		dvinit[20];	/* init routine name		*/
	char		dvopen[20];	/* open routine name		*/
	char		dvclose[20];	/* close routine name		*/
	char		dvread[20];	/* read routine name		*/
	char		dvwrite[20];	/* write routine name		*/
	char		dvcntl[20];	/* control routine name		*/
	char		dvseek[20];	/* seek routine name		*/
	char		dvgetc[20];	/* getc routine name		*/
	char		dvputc[20];	/* putc routine name		*/
	int		dvminor;	/* device number 0,1,...	*/
	struct dvtype	*dvnext;	/* next node on the list	*/
	};
typedef	struct	dvtype	*dvptr;
	dvptr	ftypes = NIL;		/* linked list of device types	*/
	dvptr	devs = NIL;		/* linked list of device decls.	*/
	dvptr	lastdv = NIL;
	dvptr	currtype = NIL;

char	*ftout[] = 
	       {"struct\tdevsw\t{\t\t\t/* device table entry */\n",
	        "\tint\tdvnum;\n",
		"\tchar\t*dvname;\n",
		"\tint\t(*dvinit)();\n",
		"\tint\t(*dvopen)();\n",
		"\tint\t(*dvclose)();\n",
		"\tint\t(*dvread)();\n",
		"\tint\t(*dvwrite)();\n",
		"\tint\t(*dvseek)();\n",
		"\tint\t(*dvgetc)();\n",
		"\tint\t(*dvputc)();\n",
		"\tint\t(*dvcntl)();\n",
		"\tint\tdvcsr;\n",
		"\tint\tdvivec;\n",
		"\tint\tdvovec;\n",
		"\tint\t(*dviint)();\n",
		"\tint\t(*dvoint)();\n",
		"\tchar\t*dvioblk;\n",
		"\tint\tdvminor;\n",
		"\t};\n\n",
		"extern\tstruct\tdevsw devtab[];",
		"\t\t/* one entry per device */\n\n",
		NULL};
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#ifndef YYSTYPE
#define YYSTYPE int
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 170 "config.y"

#include "lex.yy.c"
main(argc, argv)
	int	argc;
	char	*argv[];
{
	int	n, i, j, l, fcount;
	dvptr	s;
	int	verbose = 0;
	char	*p;
	char	c;

	if (argc>1 && (strcmp("-v",argv[1])==0)) {
		argc--;
		argv++;
		verbose++;
	}
	if (argc>2) {
		fprintf(stderr,"use: config [-v] [file]\n");
		exit(1);
	}
	if (verbose)
		printf("Opening input file...\n");
	if (argc == 2) {
		if (freopen(argv[1], "r", stdin) == NULL) {
			fprintf(stderr,"Can't open %s\n",argv[1]);
			exit(1);
		}
	} else {	/* try to open Configuration file */
		if (freopen(CONFIGIN, "r", stdin) == NULL) {
			fprintf(stderr,"Can't open %s\n", CONFIGIN);
			exit(1);
		}
	}

	/* Parse the Configuration file */

	if (verbose)
		printf("Parsing configuration specs...\n");
	if ((n=yyparse()) != 0)
		exit(n);

	/* write config.h and config.c */

	if (verbose)
		printf("Opening output files...\n");
	if ( (confc=fopen(CONFIGC,"w") ) == NULL) {
		fprintf(stderr, "Can't write on %s\n", CONFIGC);
		exit(1);
	}

	if ( (confh=fopen(CONFIGH,"w") ) == NULL) {
		fprintf(stderr, "Can't write on %s\n", CONFIGH);
		exit(1);
	}
	fprintf(confh,
		"/* conf.h (GENERATED FILE; DO NOT EDIT) */\n");
	fprintf(confc,
		"/* conf.c (GENERATED FILE; DO NOT EDIT) */\n");
	fprintf(confc, "\n#include %s\n", CONFHREF);
	fprintf(confh, "\n#define\tNULLPTR\t(char *)0\n");


	if (verbose)
		printf("Writing output...\n");
	fprintf(confh,"\n/* Device table declarations */\n");
	for (i=0 ; (p=ftout[i])!=NULL ; i++)
		fprintf(confh, "%s", p);

	/* write device declarations and definitions; count type refs. */

	fprintf(confh, "\n/* Device name definitions */\n\n");
	for (i=0,s=devs; s!=NIL ; s=s->dvnext,i++) {
		fprintf(confh, "#define\t%-12s%d\t\t\t/* type %-8s */\n",
			s->dvname, i, s->dvtname);
		s->dvminor = symtab[s->dvtnum].symoccurs++;
	}

	/* write count of device types */

	fprintf(confh,"\n/* Control block sizes */\n\n");
	for (i=0 ; i<nsym ; i++)
		if (symtab[i].symoccurs > 0) {
			fprintf(confh, "#define\tN%s\t%d\n",
				symtab[i].symname, symtab[i].symoccurs);
		}
	if (ndevs > 0)
		fprintf(confh, "\n#define\tNDEVS\t%d\n\n", ndevs);

	/* empty symbol table, collect, and write names of all I/O routines */

	nsym = 0;
	for (s=devs; s!=NIL ; s=s->dvnext) {
		lookup(s->dvinit,strlen(s->dvinit));
		lookup(s->dvopen,strlen(s->dvopen));
		lookup(s->dvclose,strlen(s->dvclose));
		lookup(s->dvread,strlen(s->dvread));
		lookup(s->dvwrite,strlen(s->dvwrite));
		lookup(s->dvseek,strlen(s->dvseek));
		lookup(s->dvcntl,strlen(s->dvcntl));
		lookup(s->dvgetc,strlen(s->dvgetc));
		lookup(s->dvputc,strlen(s->dvputc));
		lookup(s->dviint,strlen(s->dviint));
		lookup(s->dvoint,strlen(s->dvoint));
		
	}
	fprintf(confh,
		"/* Declarations of I/O routines referenced */\n\n");
	for (i=0 ; i<nsym ; i++)
		fprintf(confh, "extern\tint\t%s();\n", symtab[i].symname);


	/* produce devtab (giant I/O switch table) */

	fprintf(confc, "\n/* device independent I/O switch */\n\n");
	if (ndevs > 0) {
		fprintf(confc, "struct\tdevsw\tdevtab[NDEVS] = {\n");
		fprintf(confc, "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
			"/*  Format of entries is:",
			"device-number, device-name,",
			"init, open, close,",
			"read, write, seek,",
			"getc, putc, cntl,",
			"device-csr-address, input-vector, output-vector,",
			"iint-handler, oint-handler, control-block, minor-device,",
			"*/");
	}
	for (fcount=0,s=devs ; s!=NIL ; s=s->dvnext,fcount++) {
		fprintf(confc, "\n/*  %s  is %s  */\n\n",s->dvname,s->dvtname);
		fprintf(confc, "%d, \"%s\",\n", fcount, s->dvname);
		fprintf(confc, "%s, %s, %s,\n",
			s->dvinit, s->dvopen, s->dvclose);
		fprintf(confc, "%s, %s, %s,\n",
			s->dvread, s->dvwrite, s->dvseek);
		fprintf(confc, "%s, %s, %s,\n",
			s->dvgetc, s->dvputc, s->dvcntl);
		fprintf(confc, "0%06o, 0%03o, 0%03o,\n",
			s->dvcsr, s->dvivec, s->dvovec);
		fprintf(confc, "%s, %s, NULLPTR, %d",
			s->dviint, s->dvoint, s->dvminor);
		if ( s->dvnext != NIL )
			fprintf(confc, ",\n");
		else
			fprintf(confc, "\n\t};");
	}

	/* Copy definitions to output */

	if (brkcount == 2 && verbose)
		printf("Copying definitions to %s...\n", CONFIGH);
	if (brkcount == 2 )
		while ( (c=input()) != 0)	/* lex input routine */
			putc(c, confh);

	/* guarantee conf.c written later than conf.c for make */

	fclose(confh);
	fprintf(confc, "\n");
	fclose(confc);

	/* finish up and write report for user if requested */

	if (verbose) {
	    printf("\nConfiguration complete. Number of devs=%d:\n\n",ndevs);
	    for (s=devs; s!=NIL ; s=s->dvnext)
	      printf(
	        "Device %s (on %s) csr=0%-7o, ivec=0%-3o, ovec=0%-3o, minor=%d\n",
		s->dvname, s->dvdevice, s->dvcsr, s->dvivec, s->dvovec,
		s->dvminor);
	}
	

}


yyerror(s)
	char *s;
{
	fprintf(stderr,"Syntax error in %s on line %d\n",
		doing,linectr);
}


/*  lookup  --  lookup a name in the symbol table; return position */

lookup(str,len)
	char	*str;
	int	len;
{
	int	i;
	char	*s;

	if (len >= 20) {
		len = 19;
		fprintf(stderr,"warning: name %s truncated\n",str);
		}
	s = malloc(len+1);
	strncpy(s,str,len);
	s[len] = '\000';
	for (i=0 ; i<nsym ; i++)
		if (strcmp(s,symtab[i].symname) == 0){
			return(i);
		}
	symtab[nsym].symname = s;
	symtab[nsym].symoccurs = 0;
	return(nsym++);
}

atoi(str,len)
	char *str;
	int len;
{
	int i;
	char c;

	for (i=0; len > 0 ; len--) {
		c = *str++;
		i = 10*i + (c - '0');
		}
}
otoi(str,len)
	char *str;
	int len;
{
	int i;
	char c;

	for (i=0; len > 0 ; len--) {
		c = *str++;
		if (c > '7')
			fprintf(stderr,"invalid octal digit on line %d\n",
				linectr);
		else
			i = 8*i + (c - '0');
		}
}

/* newattr -- add a new attribute spec to current type/device description	*/

newattr(tok,val)
	int	tok;			/* token type (attribute type)	*/
	int	val;			/* symbol number of value	*/
{
	char	*c;
	dvptr	s;

	if (devs == NIL)		/* doing types			*/
		s = currtype;
	else
		s = lastdv;
	if (val>=0 && val<nsym) {
		c = symtab[val].symname;
		if (strlen(c) > 20 ) {
			fprintf(stderr,"Internal overflow\n");
			exit(1);
		}
	} else
		c = NULL;

	switch (tok) {

	case CSR:	s->dvcsr = val;
			break;
	case IVEC:	s->dvivec = val;
			break;
	case OVEC:	s->dvovec = val;
			break;
	case IINT:	strcpy(s->dviint,c);
			break;
	case OINT:	strcpy(s->dvoint,c);
			break;
	case READ:	strcpy(s->dvread,c);
			break;
	case WRITE:	strcpy(s->dvwrite,c);
			break;
	case GETC:	strcpy(s->dvgetc,c);
			break;
	case PUTC:	strcpy(s->dvputc,c);
			break;
	case OPEN:	strcpy(s->dvopen,c);
			break;
	case CLOSE:	strcpy(s->dvclose,c);
			break;
	case INIT:	strcpy(s->dvinit,c);
			break;
	case SEEK:	strcpy(s->dvseek,c);
			break;
	case CNTL:	strcpy(s->dvcntl,c);
			break;
	default:	fprintf(stderr, "Internal error 1\n");
	}
}

/* cktname  --  check type name for duplicates */

cktname(symid)
	int symid;
{
	dvptr	s;
extern	dvptr	ftypes;
	char	*name;

	name = symtab[symid].symname;
	for (s=ftypes; s!=NIL ; s=s->dvnext) {
		if (s->dvtname == name) {
			fprintf(stderr,"Duplicate type name %s on line %d\n",
				name,linectr);
			exit(1);
			}
		}
	return(symid);
}

/* mktype  --  make a node in the type list and initialize to defaults	*/

mktype(deviceid)
	int deviceid;
{
	dvptr	s,p;
	char	*tn,*dn;

	p = NIL;
	tn = symtab[currtname].symname;
	dn = symtab[deviceid].symname;
	for (s = ftypes; s!=NIL ; s=s->dvnext) {
		if (s->dvtname == tn && s->dvdevice==dn) {
		   fprintf(stderr,
		     "Duplicate device %s for type %s on line %d\n",
		     dn, tn, linectr);
		   exit(1);
		}
		p = s;
	}
	currtype = s = (dvptr) malloc( sizeof(struct dvtype));
	if (ftypes != NIL) {
		p->dvnext = s;
	}
	else {
		ftypes = s;
	}
	initattr(s, currtname, deviceid);
}

/* initialize attributes in a type declaration node to typename...	*/

initattr(fstr, tnum, deviceid)
	dvptr	fstr;
	int	tnum;
	int	deviceid;
{
	char	*typnam;

	typnam = symtab[tnum].symname;
	fstr->dvname = NULL;
	fstr->dvtname = typnam;
	fstr->dvtnum = tnum;
	fstr->dvdevice = symtab[deviceid].symname;
	fstr->dvcsr = 0;
	fstr->dvivec = 0;
	fstr->dvovec = 0;
	strcpy(fstr->dviint,typnam);
	strcat(fstr->dviint,"iin");
	strcpy(fstr->dvoint,typnam);
	strcat(fstr->dvoint,"oin");
	strcpy(fstr->dvinit,typnam);
	strcat(fstr->dvinit,"init");
	strcpy(fstr->dvopen,typnam);
	strcat(fstr->dvopen,"open");
	strcpy(fstr->dvclose,typnam);
	strcat(fstr->dvclose,"close");
	strcpy(fstr->dvread,typnam);
	strcat(fstr->dvread,"read");
	strcpy(fstr->dvwrite,typnam);
	strcat(fstr->dvwrite,"write");
	strcpy(fstr->dvcntl,typnam);
	strcat(fstr->dvcntl,"control");
	strcpy(fstr->dvseek,typnam);
	strcat(fstr->dvseek,"seek");
	strcpy(fstr->dvgetc,typnam);
	strcat(fstr->dvgetc,"getc");
	strcpy(fstr->dvputc,typnam);
	strcat(fstr->dvputc,"putc");
	fstr->dvminor = 0;
}

/* mkdev  --  make a node on the device list */

mkdev(nameid, typid, deviceid)
	int	nameid, typid, deviceid;
{
	dvptr	s;
	char	*devn,*tn,*dn;
	int	found;

	s = (dvptr) malloc(sizeof(struct dvtype));
	s->dvnext = NIL;
	if (devs == NIL) {
		devs = s;
		lastdv = s;
	} else {
		lastdv->dvnext = s;
		lastdv = s;
	}
	ndevs++;
	tn = symtab[typid].symname;
	devn = symtab[nameid].symname;
	if (deviceid >= 0)
		dn = symtab[deviceid].symname;
	else
		dn = NULL;
	found = 0;
	for (s=ftypes ; s != NULL ; s=s->dvnext)
		if (s->dvtname == tn && (dn==NULL || s->dvdevice==dn)) {
			strdup(lastdv,s,sizeof(struct dvtype));
			found=1;
			break;
		}
	if (found==0) {
		fprintf(stderr,
		  "Bad type or device name in declaration of %s on line %d\n",
		  devn, linectr);
		exit(1);
	}
	lastdv->dvnext = NIL;
	lastdv->dvname = devn;
}


/* chdname  -- check for duplicate device name */

ckdname(devid)
	int devid;
{
	dvptr	s;
extern	dvptr	devs;
	char	*name;

	name = symtab[devid].symname;
	for (s=devs; s!=NIL ; s=s->dvnext) {
		if (s->dvname == name) {
			fprintf(stderr,"Duplicate device name %s on line %d\n",
				name,linectr);
			exit(1);
			}
		}
	return(devid);
}

strdup(tostr,fromstr,len)
	char	*tostr, *fromstr;
	int	len;
{
	for( ; len > 0 ; len--)
		*tostr++ = *fromstr++;
}
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 36
# define YYLAST 79
short yyact[]={

  24,  25,  26,  27,  28,  31,  29,  30,  34,  35,
  36,  37,  57,  19,  32,  33,  16,   5,   9,  17,
   8,   9,  42,  41,  15,  13,  18,  56,  12,  11,
  10,  23,  14,  40,   7,   6,   3,  22,   4,  20,
  38,   2,  21,   1,   0,   0,   0,  39,  45,  46,
  47,  48,  49,  50,  51,  52,  53,  54,  55,  43,
  44,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  58 };
short yypact[]={

-1000,-1000,-1000,-240,-243,-1000,-1000,-259,-239,-1000,
-1000,-1000,-261,-1000,-259,-1000,-243,-1000,-262,-243,
-1000,-262,-1000,-1000,-237,-237,-237,-243,-243,-243,
-243,-243,-243,-243,-243,-243,-243,-243,-263,-262,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-243,-1000 };
short yypgo[]={

   0,  43,  41,  38,  36,  35,  34,  32,  24,  26,
  20,  31,  33,  30,  29,  28,  27 };
short yyr1[]={

   0,   1,   2,   4,   4,   5,   7,   7,   8,   6,
  10,   9,   9,  11,  11,  11,  11,  11,  11,  11,
  11,  11,  11,  11,  11,  11,  11,  12,  12,   3,
   3,  13,  14,  15,  16,  16 };
short yyr2[]={

   0,   2,   2,   0,   2,   2,   2,   3,   2,   2,
   1,   0,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   1,   1,   0,
   2,   2,   4,   1,   0,   2 };
short yychk[]={

-1000,  -1,  -2,  -4,  -3, 257,  -5,  -6, -10, 261,
 -13, -14, -15, -10,  -7,  -8, 275, 258,  -9, 274,
  -8,  -9, -10, -11, 262, 263, 264, 265, 266, 268,
 269, 267, 276, 277, 270, 271, 272, 273, -10,  -9,
 -12, 260, 259, -12, -12, -10, -10, -10, -10, -10,
 -10, -10, -10, -10, -10, -10, -16, 275, -10 };
short yydef[]={

   3,  -2,  29,   0,   1,   2,   4,   0,   0,  10,
  30,  11,   0,  33,   5,  11,   0,   9,  31,   0,
  11,   6,   8,  12,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  34,   7,
  13,  27,  28,  14,  15,  16,  17,  18,  19,  20,
  21,  22,  23,  24,  25,  26,  32,   0,  35 };
#ifndef lint
static	char yaccpar_sccsid[] = "@(#)yaccpar 1.2 86/07/18 SMI"; /* from UCB 4.1 83/02/11 */
#endif

#
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

 yystack:    /* put a state and value onto the stack */

#ifdef YYDEBUG
	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
#endif
		if( ++yyps>= &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
		}

 yydefault:
	/* default state action */

	if( (yyn=yydef[yystate]) == -2 ) {
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
		yyerrlab:
			++yynerrs;

		case 1:
		case 2: /* incompletely recovered error ... try again */

			yyerrflag = 3;

			/* find a state where "error" is a legal shift action */

			while ( yyps >= yys ) {
			   yyn = yypact[*yyps] + YYERRCODE;
			   if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
			      yystate = yyact[yyn];  /* simulate a shift of "error" */
			      goto yystack;
			      }
			   yyn = yypact[*yyps];

			   /* the current yyps has no shift onn "error", pop stack */

#ifdef YYDEBUG
			   if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
			   --yyps;
			   --yypv;
			   }

			/* there is no state on the stack with an error shift ... abort */

	yyabort:
			return(1);


		case 3:  /* no shift yet; clobber input char */

#ifdef YYDEBUG
			if( yydebug ) printf( "error recovery discards char %d\n", yychar );
#endif

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

#ifdef YYDEBUG
		if( yydebug ) printf("reduce %d\n",yyn);
#endif
		yyps -= yyr2[yyn];
		yypvt = yypv;
		yypv -= yyr2[yyn];
		yyval = yypv[1];
		yym=yyn;
			/* consult goto table to find next state */
		yyn = yyr1[yyn];
		yyj = yypgo[yyn] + *yyps + 1;
		if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
		switch(yym){
			
case 2:
# line 91 "config.y"
{doing = "device definitions";} break;
case 8:
# line 102 "config.y"
{mktype(yypvt[-0]);} break;
case 9:
# line 105 "config.y"
{yyval = currtname = cktname(yypvt[-1]);} break;
case 10:
# line 108 "config.y"
{yyval = currname =
					 lookup(yytext,yyleng);
					} break;
case 13:
# line 116 "config.y"
{newattr(CSR,yypvt[-0]);} break;
case 14:
# line 118 "config.y"
{newattr(IVEC,yypvt[-0]);} break;
case 15:
# line 120 "config.y"
{newattr(OVEC,yypvt[-0]);} break;
case 16:
# line 122 "config.y"
{newattr(IINT,yypvt[-0]);} break;
case 17:
# line 124 "config.y"
{newattr(OINT,yypvt[-0]);} break;
case 18:
# line 126 "config.y"
{newattr(OPEN,yypvt[-0]);} break;
case 19:
# line 128 "config.y"
{newattr(CLOSE,yypvt[-0]);} break;
case 20:
# line 130 "config.y"
{newattr(INIT,yypvt[-0]);} break;
case 21:
# line 132 "config.y"
{newattr(GETC,yypvt[-0]);} break;
case 22:
# line 134 "config.y"
{newattr(PUTC,yypvt[-0]);} break;
case 23:
# line 136 "config.y"
{newattr(READ,yypvt[-0]);} break;
case 24:
# line 138 "config.y"
{newattr(WRITE,yypvt[-0]);} break;
case 25:
# line 140 "config.y"
{newattr(SEEK,yypvt[-0]);} break;
case 26:
# line 142 "config.y"
{newattr(CNTL,yypvt[-0]);} break;
case 27:
# line 150 "config.y"
{yyval = otoi(yytext,yyleng);} break;
case 28:
# line 152 "config.y"
{yyval = otoi(yytext,yyleng);} break;
case 32:
# line 160 "config.y"
{mkdev(yypvt[-3],yypvt[-1],yypvt[-0]);} break;
case 33:
# line 163 "config.y"
{yyval = currdname = ckdname(yypvt[-0]);} break;
case 34:
# line 166 "config.y"
{yyval = 0;} break;
case 35:
# line 168 "config.y"
{yyval = yypvt[-0];} break;
		}
		goto yystack;  /* stack new state and value */

	}
