/* test.c - main */

#include <conf.h>

main ()
{
	char	ch;
	char	line[128];
	int	i,l;

	printf("Starting I/O test\n");
	printf("type line(s) to be echoed (you have 5 seconds): ");
	sleep10(50);
	printf("I got %d characters\n\n" , l=read(CONSOLE,line,0));
	for (i=0 ; i<l ; i++)
		putc(CONSOLE,line[i]);
	printf("\n\nI'll echo chars until you type S\n");
	while ( (ch=getc(CONSOLE)) != 'S')
		putc(CONSOLE,ch);
	printf("\n\ntest over\n");
}
