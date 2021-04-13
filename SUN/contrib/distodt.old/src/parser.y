%{
    /*
     * Parser for machines database reader.
     *
     * Steve Chapin, 14 Apr 88
     * 
     */

#include <stdio.h>
	
#include "../h/bedata.h"

%}
%union {
    char *sb;
}
%token STRING
%token NEWLINE
%start file
%%
file: file line
    | line
    ;
line : STRING STRING STRING STRING STRING STRING STRING STRING NEWLINE
     {
	 table[cnt++] = makeodt($1, $2, $3, $4, $5, $6, $7, $8);
	 if (cnt >= size) {
	    size += 10;
	    if (!(table = (struct odtdata **) 
			realloc(table, size * sizeof(struct odtdata *)))) {
		fprintf(stderr, "Not enough memory to hold data table.\n");
		exit(1);
	    }
	}
     }
     | NEWLINE
     {
	 return(yyparse());
     }
%%
yyerror()

{
    fprintf(stderr, "Error in database format.\n");
}
