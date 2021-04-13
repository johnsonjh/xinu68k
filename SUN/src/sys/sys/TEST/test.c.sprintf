/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <network.h>

#define STRLEN 60

char remem[10000], *prem;
char rem1[10000], *prem1;
char rem2[10000], *prem2;
char rem3[100000], *prem3;
char rem4[100000], *prem4;
char rem5[100000], *prem5;

char str[STRLEN];

/*------------------------------------------------------------------------
 *  main  --  start UDP echo server and then run Xinu pseudo-shell
 *------------------------------------------------------------------------
 */
main()
{
	int i,j;

	prem = remem;
	prem1 = rem1;
	prem2 = rem2;
	prem3 = rem3;
	prem4 = rem4;
	prem5 = rem5;

	printf("\n\n");
	for (i=0; i<50; ++i) {
	    sprintf(str,"%03d ",i);
	    for (j=0; j<26; ++j) {
		/*sprintf(str + 4+j,"%c", 'a'+j);*/
		sprintf(str + strlen(str),"%c", 'a'+j);
	    }
	    /*sprintf(str + 30,"  ");*/
	    sprintf(str + strlen(str),"%c",' ');
	    for (j=0; j<26; ++j) {
		/*sprintf(str + 31 + j,"%c", 'A'+j);*/
		sprintf(str + strlen(str),"%c", 'A'+j);
	    }
	    /*sprintf(str + 57,"\n");*/
	    sprintf(str + strlen(str),"%c",'\n');
	    newwrite(str);
	}
	sleep(1);
	
	*prem = '\00';
	*prem1 = '\00';
	*prem2 = '\00';
	*prem3 = '\00';
	*prem4 = '\00';
	*prem5 = '\00';

	kprintf("\n=====remem=====\n%s=====remem=====\n",remem);
	kprintf("\n=====rem1=====\n%s=====rem1=====\n",rem1);
	kprintf("\n=====rem2=====\n%s=====rem2=====\n",rem2);
	kprintf("\n=====rem3=====\n%s=====rem3=====\n",rem3);
	kprintf("\n=====rem4=====\n%s=====rem4=====\n",rem4);
	kprintf("\n=====rem5=====\n%s=====rem5=====\n",rem5);

	ret_mon();
}

newwrite(str)
     char *str;
{
      while (*str != '\00') {
	  *prem++ = *str;
          putc(CONSOLE,*str);
	  ++str;
      }
}






