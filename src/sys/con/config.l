A		[a-z_A-Z]
M		[a-z_A-Z0-9]
	int skipping;
%%
"/*"			{skipping = 1;}
"*/"			{skipping = 0;}
"%"			{extern	int	brkcount;
			 if (! skipping)
				if (++brkcount == 2)
					return(0); /* end-of-file */
				else
					return(DEFBRK);
			}
":"			{if (! skipping) return(COLON);}
0[0-9]*			{if (! skipping) return(OCTAL);}
[1-9][0-9]+		{if (! skipping) return(INTEGER);}
is			{if (! skipping) return(IS);}
on			{if (! skipping) return(ON);}
"="			;
"iint"			{if (! skipping) return(IINT);}
"-iint"			{if (! skipping) return(IINT);}
"oint"			{if (! skipping) return(OINT);}
"-oint"			{if (! skipping) return(OINT);}
"-csr"			{if (! skipping) return(CSR);}
"csr"			{if (! skipping) return(CSR);}
"-ivec"			{if (! skipping) return(IVEC);}
"ivec"			{if (! skipping) return(IVEC);}
"-ovec"			{if (! skipping) return(OVEC);}
"ovec"			{if (! skipping) return(OVEC);}
"-i"			{if (! skipping) return(INIT);}
"i"			{if (! skipping) return(INIT);}
"-o"			{if (! skipping) return(OPEN);}
"o"			{if (! skipping) return(OPEN);}
"-c"			{if (! skipping) return(CLOSE);}
"c"			{if (! skipping) return(CLOSE);}
"-r"			{if (! skipping) return(READ);}
"r"			{if (! skipping) return(READ);}
"-g"			{if (! skipping) return(GETC);}
"g"			{if (! skipping) return(GETC);}
"-p"			{if (! skipping) return(PUTC);}
"p"			{if (! skipping) return(PUTC);}
"-w"			{if (! skipping) return(WRITE);}
"w"			{if (! skipping) return(WRITE);}
"-s"			{if (! skipping) return(SEEK);}
"s"			{if (! skipping) return(SEEK);}
"-n"			{if (! skipping) return(CNTL);}
"n"			{if (! skipping) return(CNTL);}
[ \t]+			;
"\n"			{extern int linectr;
			 linectr++;
			}
{A}{M}*			{if (! skipping) return(IDENT);}
.			{if (! skipping) return(yytext[0]);}
