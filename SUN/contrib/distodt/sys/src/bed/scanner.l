%{
/*
 * Scanner for machines database.
 *
 * Steve Chapin, 13 Apr 1988
 */
#include <stdio.h>
#include "tokens.h"

extern char *newsb(), *xmalloc();

YYSTYPE yylval;

%}
C		[^\n\t ]
N		[\n]
W		[\t ]|"\\"\n
%%
{W}			/* eat up white space */ ;
"#"[^\n]*		/* eat up comment lines */ ;
{N}			{
			    return(TKNEWLINE);
		        }
{C}+			{
    			    yylval.sb = newsb(yytext);
		    	    return(TKSTRING);
			}
%%

/*
 *---------------------------------------------------------------------------
 * char *newsb(sb)
 * 	char *sb;
 *
 * makes a new string and copies in the contents of the arg.
 *---------------------------------------------------------------------------
 */
char *newsb(sb)
     char *sb;
{
        char *sb2 = (char *) xmalloc(strlen(sb) + 1);

        strcpy(sb2, sb);
        return(sb2);
}

