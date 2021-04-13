static int saved_ps;   /* only one "process"--so one global saved_ps */
disable()
{
#ifdef	DEBUG
	kprintf("disable()\n");
#endif
	_disable(&saved_ps);
}

restore()
{
#ifdef	DEBUG
	kprintf("restore()\n");
#endif
	_restore(&saved_ps);
}
