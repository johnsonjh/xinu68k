%{
/*
 * Scanner for machines database.
 *
 * Steve Chapin, 13 Apr 1988
 */
#include <stdio.h>

#include "../h/tokens.h"

char *newsb();
%}
C		[^\n\t ]
N		[\n]
W		[\t ]
%%
{W}			/* eat up white space */ ;
"#"[^\n]*{N}		/* eat up comment lines */ ;
{N}			{
			return(NEWLINE);
		        }
{C}+			{
    			yylval.sb = newsb(yytext);
		    	return(STRING);
			}
%%



