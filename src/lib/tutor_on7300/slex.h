/* header file slex.h */
struct cmd 
{
  char *cmdtoken;		/* TM or whatever--uppercase version of cmd */
  int (*cmdfn)();		/* implementation routine for cmd */
  char *help;			/* helpstring for cmd */
};

#define MAXTOKENLEN 80
