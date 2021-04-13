extern int console;
putchar(ch)
char ch;
{
	putc(console,ch);
}
