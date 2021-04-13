	/**********************************************************************
	 * mem.c - memory and register display/modify routines.
	 *
	 *	   Feb. 1991 - Joe Prosser 
	 */
	#include <stdio.h>
	#include <ctype.h>
	#include "slex.h"
	#include "cpu.h"
	/* mem.h begin */
	#define BUFSIZE 16 
	#define TRUE 0
	#define FALSE 1
	#define USAGE() printf("%s\n",cp->help);return 0
	extern int registers[];
	extern int console;
	/*****************************************************************************
	 *  FUNCTION: mem_display - display 16 byte (clicks) of code.  do ascii
	 *		representation, but not disassemble
	 *  
	 */

	int mem_display ( char *line, struct cmd *cp ) 
	{
	    unsigned char buf[BUFSIZE];
	    int count,		/* how many 16 byte sections do we display */
		i,strloc;	/* run through arg string in gettoken */

	    static unsigned char *addr;	/* keep address between calls*/

	    if(gettoken(line,buf,&strloc)) /* pressed md , no args*/
	      {
		count = 1;		/* 1 more line */
	      }
	    else
	    {
	      sscanf(buf,"%x",&addr);	/* get addr */
		line += strloc;
		if(gettoken(line,buf,&strloc)) /* pressed md , no count*/
		    count = 1;
		else
		{	/* If count isn't a multiple round up unless remainder = 1*/
		  sscanf(buf,"%d",&count);
		    if(count % 16 >= 1)
			count /= 16;
		    else
			count = count / 16 + 1;
		}
	    }	
	    while(count--)
	    {
		printf("\n%08x    ",(int)addr);
		for (i = 0; i<16;i++)
		printf("%02x ",addr[i]);

		for(i = 0; i < 16;i++)
		{
		   if(isprint(addr[i]))
			printf("%c",(char)addr[i]);
		   else
			printf(".");
		}
		addr += 16;
	    }/* end while */
	    return 0;
	}


	/*****************************************************************************
	 *  FUNCTION: mem_mod - modify memory - try casting a void pointer to 
	 *		different types to handle looking at different size
	 *  		chunks.
	 */

	int mem_mod ( char * line, struct cmd * cp ) 
	{
	    static int addr;
	    char buf[BUFSIZE],
		  cntrl;		/* control character */
	    char format;
	    int no_disp = FALSE,        /* for the 'N' flag */
		strloc,
		val,			/* new value */
		size,			/* how big a chunk do we look at? */
		incr = 0;		/* used to increment address */
	    void *area;

	    if(gettoken(line,buf,&strloc)) /* pressed mm , no args*/
	    {
		USAGE();
		return -1;
	    }
	    if (sscanf(buf,"%x;%c",&addr,&format)!=2) {
	    if (sscanf(buf,"%x",&addr)==1)
	    {
		area = (unsigned char *)addr; /* default - byte */
		size = 1;
	    }
	    else 
	    {
		USAGE();
		return -1;
	    } 
	    } else {   /* got addr;format */
	    switch(format|('a'-'A')) /* make lowercase */
	    {
		case 'w':  /* words */
		    area = (unsigned short *)addr;
		    size = 2;
		    break;
		case 'l': /* longwords */
		    area = (unsigned long *)addr;
		    size = 4;
		    break;
		case 'o':
		    area = (unsigned char *)addr; /* byte odd address */
		    incr = 1;
		    size = 1;
		    if(!(addr & 1))  /* this is an even byte */
			area++;
		    break;
		case 'v':
		    area = (unsigned char *)addr; /* byte even address */
		    incr = 1;
		    size = 1;   
		    if(addr & 1)  /* this is an odd byte */
			area++;
		    break;			
		case 'n':
		    no_disp = TRUE;
		    break;
		default:
		    printf("Bad format %c\n",format);
		    return -1;
		    break;
	    }/* end switch */
	  } /* else else */
	/* now begin io loop */
	    while(1)
	    {
	      int contents;

	      switch (size) {
	      case 1: contents = *(unsigned char *)area;
		printf("%06x: %02x ?",area,contents);
		break;
		printf("%06x: %x ?",area,contents);
	      case 2: contents = *(unsigned short *)area;
		printf("%06x: %04x ?",area,contents);
		break;
	      case 4: contents = *(unsigned int *)area;
		printf("%06x: %08x ?",area,contents);
		break;
	      default: printf("Bad size\n");
		return -1;
		break;
	      }
		gets(buf);
		if (buf[0]=='.')
		  break;
		if (buf[0]=='\0') {
		  area += size;
		  continue;  /* skip this update */
		}
		if(sscanf(buf,"%x",&val))     /* get the new value */
		{
		    switch(size) /* update */
		    {
			case 1:
			    *(unsigned char *)area = val;
			    break;
			case 2:
			    *(unsigned short *)area = val;
			    break;
			case 4:
			    *(unsigned long *)area = val;
			    break;
		    }
		    (int)area += size;
	#ifdef NEEDS_WORK
		    switch(cntrl)
		    {
			case '\0':  /*  move  */
				area++;
				area += incr;
				break;
			case '^':  /* update and move back */
				area--;
				area -= incr;
				break;
			case '=':
				break;
			case '.':
				return 0;
				break;
			default:
				printf("oops!\n");
				break;
		    }/* end witch */
	#endif
		}
	     }
	    return 0;
	}

	/* mem_set - stores max 8 byte data into specified memory location */

	/*****************************************************************************
	 *  FUNCTION: mem_set - alters memory by setting data into address specified
	 *			Data can be an ascii string or hex data. several
	 *  			strings can be entered, size is limited to eight chars
	 *		I was going to load everything into a float, and then do a
	 *		direct assignment (Why an 8 byte limit??), but couldn't think
	 *		of a quicker way of getting the data into it than by direct
	 *		assignment.  Any ideas?
	 */

	int mem_set ( char * line, struct cmd * cp ) 
	{

	    unsigned int addr,strloc,len;
	    char buf[BUFSIZE], *tmp;
	    unsigned long set = 0;

	    if(gettoken(line,buf,&strloc) == 0 && /* got address */
		sscanf(buf,"%x", &addr) == 1) /* was legit */
	    {	
		line +=strloc;
		while(gettoken(line,buf,&strloc) == 0 &&  /* got value */
		     (len =strlen(buf)) <= 8) 		  /* not too big */	
		{
		    tmp = buf;
		    if(*tmp == '\'')  /* is the data a string? */
		    {
			tmp++;
			len -= 2; /* neglect quotes */
			while(len--)
			    *(unsigned char *)addr++ = *tmp++;
		    }
		    else /* buf is a hex int */
		      {
			int val;

			tmp = buf;
			/* first use 16 bit writes as far as possible--
			   -- some dev reg's and mmu page map on 7300 need them */
			while (len>0 && (len%4==0 || len%4==3) && (addr&1) == 0)
			 {
			   char bytestr[5];
			   int i,thislen;
			   
			   thislen = (len%4)? 3:4;
			   for (i=0;i<thislen;i++)
			    {
				bytestr[i] = *tmp;
				len--;
				tmp++;
			     }
			    bytestr[i] = '\0';
			    sscanf(bytestr,"%x",&val);  /* pick up word's worth */
			    *(short *)addr = (short)val;  /* use word write */
			    addr += 2;
			  }
			while (len>0)
			  {
			    char bytestr[3];
			    
			    if (len%2)	/* odd len */
			      {
				bytestr[0] = ' ';
				bytestr[1] = tmp[0];
				bytestr[2] = '\0';
				len -= 1;
				tmp += 1;
			      }
			    else	/* even len */
			      {
				bytestr[0] = tmp[0];
				bytestr[1] = tmp[1];
				bytestr[2] = '\0';
				len -= 2;
				tmp += 2;
			      }
			    sscanf(bytestr,"%x",&val);
			    *(unsigned char *)addr++ = val;
			  }
		      }
		    line +=strloc;
		}
	    }
	    else 
	      printf("Error in command\n");
	    return 0;
	}



	int disp_regs(char *line, struct cmd *cp)
	{
		printf("PC=%08x SR=%04x\n",registers[PC], 
		registers[PS]);

		printf("D0=%08x D1=%08x D2=%08x D3=%08x\n",registers[D0], 
		registers[D1], registers[D2], registers[D3]);

		printf("D4=%08x D5=%08x D6=%08x D7=%08x\n",registers[D4], 
		registers[D5], registers[D6], registers[D7]);
		
		printf("A0=%08x A1=%08x A2=%08x A3=%08x\n",registers[A0], 
		registers[A1], registers[A2], registers[A3]);
		
		printf("A4=%08x A5=%08x A6=%08x A7=%08x\n",registers[A4], 
		registers[A5], registers[A6], registers[A7]);
		return 0;
	}


	int dota0(char *line, struct cmd *cp)
	{
	    printf("A0=%08x\n",registers[A0]);
	}

	int dota1(char *line, struct cmd *cp)
	{
	    printf("A1=%08x\n",registers[A1]);
	}

	int dota2(char *line, struct cmd *cp)
	{
	    printf("A2=%08x\n",registers[A2]);
	}

	int dota3(char *line, struct cmd *cp)
	{
	    printf("A3=%08x\n",registers[A3]);
	}

	int dota4(char *line, struct cmd *cp)
	{
	    printf("A4=%08x\n",registers[A4]);
	}

	int dota5(char *line, struct cmd *cp)
	{
	    printf("A5=%08x\n",registers[A5]);
	}

	int dota6(char *line, struct cmd *cp)
	{
	    printf("A6=%08x\n",registers[A6]);
	}

	int dota7(char *line, struct cmd *cp)
	{
	    printf("A7=%08x\n",registers[A7]);
	}


	int dotd0(char *line, struct cmd *cp)
	{
	    printf("D0=%08x\n",registers[D0]);
	}

	int dotd1(char *line, struct cmd *cp)
	{
	    printf("D1=%08x\n",registers[D1]);
	}

	int dotd2(char *line, struct cmd *cp)
	{
	    printf("D2=%08x\n",registers[D2]);
	}

	int dotd3(char *line, struct cmd *cp)
	{
	    printf("D3=%08x\n",registers[D3]);
	}

	int dotd4(char *line, struct cmd *cp)
	{
	    printf("D4=%08x\n",registers[D4]);
	}

	int dotd5(char *line, struct cmd *cp)
	{
	    printf("D5=%08x\n",registers[D5]);
	}

	int dotd6(char *line, struct cmd *cp)
	{
	    printf("D6=%08x\n",registers[D6]);
	}

	int dotd7(char *line, struct cmd *cp)
	{
	    printf("D7=%08x\n",registers[D7]);
	}
	#ifdef NYI
	int dotSS(char *line, struct cmd *cp)
	{
	    printf("SS=%08x\n",registers[SS]);
	}

	int dotus(char *line, struct cmd *cp)
	{
	    printf("US=%08x\n",registers[US]);
	}
	#endif
	int dotSR(char *line, struct cmd *cp)
{
    int val;

    if (sscanf(line,"%x",&val)==1)
	registers[PS] = val;
    else
        printf("SR=%04x\n",registers[PS]);
    return 0;
}

int dotPC(char *line, struct cmd *cp)
{
    printf("PC=%08x\n",registers[PC]);
}

