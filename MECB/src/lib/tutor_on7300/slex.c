/*
 *			file: ~yaz/cs241/hw2/slex.c
 *
 *	Source code provided by prof. O'neil for tutor assignment.
 *
 *	Modified by Yasuhiro Endo so that slex would recognize comands typed
 *	in lower case in Oct., 1989.
 *
 */

#include <stdio.h>
#include <ctype.h>
#include "slex.h"

/* simple lexical analyzer, front part of a parser                   */
/* --compare to UNIX tool "lex", general lexical analyzer            */
/* gets a (space-delimited) token from linebuf and tries to match    */
/*   it to one of the cmdtokens in the provided cmdtable             */
/* accepts:                                                          */
/*         linebuf--string to get token from                         */
/*         cmdtable--cmdtable to use                                 */
/* returns:                                                          */
/*         *cnum_ptr--command # (offset in cmdtable) or -1 if no match */
/*         *pos_ptr--new place in linebuf after token match          */

slex ( char * linebuf, struct cmd cmdtable[], int * cnum_ptr, int * pos_ptr ) {

    /* linebuf   : string from user */
    /* cmdtable[]: cmd table to use */
    /* cnum_ptr  : returned command number */
    /* pos_ptr   : returned new place in linebuf */

  int i = 0;	/* i is used twice as misc. counter */
  char token[MAXTOKENLEN];
  int newpos;

  if (gettoken(linebuf,token,&newpos)<0) /* get token from linebuf */
    return -1;			/* couldn't find token */
  else
      while ( token[i] ) {
	  if ( islower ( token[i] ))
	      token [i] = toupper ( token[i] );	/* make upper case */
          i++;
      }
  i = 0;
  while ((cmdtable[i].cmdtoken != NULL)) {
    if (strcmp(cmdtable[i].cmdtoken,token)==0)
      {
	*cnum_ptr = i;		/* success--return command # */
	*pos_ptr = newpos;	/* and where we got to in linebuf */
	return 0;
      }
    else
      i++;			/* keep scanning table */
  }
  return -1;			/* no match */
}

/* get one space-delimited token from string in linebuf, also return */
/* new position in string */  
gettoken(linebuf,token,pos_ptr)
char *linebuf;
char *token;
int *pos_ptr;
{
  int i = 0;
  int j = 0;

  while (linebuf[i]==' ')
    i++;			/* skip blanks (other things???) */
  while (linebuf[i]!=' '&&linebuf[i]!='\0')
    token[j++] = linebuf[i++];	/* copy chars to token */
  if (j==0)
    return -1;			/* nothing there */
  else
    {
      token[j] = '\0';		/* null-terminate token */
      *pos_ptr = i;		/* return place in linebuf we got to */
      return 0;			/* success */
    }
}
#define BS 8
get_edited_line(char *line)
{
   char *p= line, *q = line;

   gets(line);
   while (*p != '\0') {
     if ((*p == BS) && (q>line))
       q--; /* wipe out prev char */
     else if (*p != BS)
       *q++ = *p;  /* copy char to final place */
     p++;
    }
   *q = '\0';
}

