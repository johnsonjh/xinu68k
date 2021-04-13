/* test.c - main */

#include <conf.h>
#include <kernel.h>

char	message[] = "Hello there world.";

/*----------------------------------------------------------------------
 *  main  --  test dlc basics assuming RINOUT connected to RINGIN
 *----------------------------------------------------------------------
 */
main()
{
	int	p();

	kprintf("Testing dlc\n");
	kprintf("Creating process to read RINGIN (%d)\n",RINGIN);
	resume(create(p,300,25,"dlctest",1, RINGIN));
	kprintf("Writing this message to RINGOUT(%d): %s\n\n",
		RINGOUT, message);
	write(RINGOUT, message, strlen(message) );
	kprintf("Writing 'hi' to RINGOUT(%d)\n", RINGOUT);
	write(RINGOUT, "hi", 2);
	dldump();
	kprintf("End writing\n");
}
p(dev)
{
	char	ch;
	int	len,i;
	char	buff[130];

	kprintf(" P:reading device %d\n",dev);
	while (TRUE) {
		kprintf(" P:read returns %d\n",len=read(dev,buff,128));
		for (i=0 ; i<len ; i++)
			if ( (ch=buff[i]) < ' ')
				printf("^%c", ch+0100);
			else if (ch==0177)
				printf("^?");
			else
				printf("%c", ch);
		printf("\n");
	}
}
