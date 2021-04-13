/* dotrace.c - dotrace */

#include <kernel.h>

int	dotrace(procname, argv, argc)
	char	*procname;
	int	*argv;
	int	argc;
{
	int	i;

	kprintf("%s(", procname);
	for (i=0; i<argc-1; i++)
		kprintf("%x,",*(argv + i));
	if (argc != 0)
		kprintf("%x)\n",*(argv + argc - 1));
	else
		kprintf(")\n");
}
