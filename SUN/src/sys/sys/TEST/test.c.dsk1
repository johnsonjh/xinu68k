/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <xebec.h>
#include <dtc.h>
#include <disk.h>

#define	NBLOCKS	200

/*------------------------------------------------------------------------
 *  main  --  test disk driver
 *------------------------------------------------------------------------
 */
main()
{
	int	w;			/* nonzero for write test	*/
	int	i, j;
	char	ch;
	char	buff[DBUFSIZ], buf2[DBUFSIZ];

	kprintf("Type R for read-only test, W for read/write test: ");
	while ( (ch=getc(CONSOLE)) != 'R' && ch!='W')
		;
	w = ch=='W' ? 1 : 0;
	if (w)
		kprintf("\nPerforming read/write test\n\n");
	else
		kprintf("\nPerforming only read test\n\n");
	kprintf("Test 1: read first %d blocks from disk.\n",NBLOCKS);
	for (i=0 ; i<NBLOCKS ; i++)
		if (NBLOCKS < 20)
		kprintf("Read of block %d is %d\n",i,read(DISK0,buff,i));
		else
			read(DISK0,buff,i);
	if ( ! w ) {
		kprintf("Finshed testing\n");
		return;
	}
	for (j=0 ; j< DBUFSIZ ; j++)
		buf2[j] = 'X';
	kprintf("\nTest 2: write the first %d blocks\n",NBLOCKS);
	for (i=0 ; i < NBLOCKS ; i++)
		if (NBLOCKS < 20)
		kprintf("Write of block %d is %d\n",i,
			write(DISK0,dskcbuf(buf2),i));
		else
			write(DISK0,dskcbuf(buf2),i);
	kprintf("\nTest 3: read back and check first %d blocks\n",NBLOCKS);
	for (i=0 ; i < NBLOCKS ; i++) {
		for (j=0 ; j<DBUFSIZ ; j++)
			buff[j] = '\01';
		if (NBLOCKS < 20)
		kprintf("Read of block %d got %d\n",i,read(DISK0,buff,i));
		else
			read(DISK0,buff,i);
		for (j=0 ; j<DBUFSIZ ; j++)
			if (buf2[j] != buff[j])
				kprintf("Error on byte %d 0%o->0%o\n",
					j,buf2[j],buff[j]);
		if (NBLOCKS < 20)
			kprintf("Comparison done\n");
	}
	kprintf("\n\nFinished all tests\n");
}
