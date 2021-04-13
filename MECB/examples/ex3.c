/* ex3.c - main, prntr */

#include <conf.h>

/*-----------------------------------------------------------------------------
 *  main  --  example of 2 processes executing the same code concurrently
 *-----------------------------------------------------------------------------
 */
main()
{
  int	prntr();

  resume( create(prntr,  1000,  20, "print A", 1, 'A')  );
  resume( create(prntr,  1000,  20, "print B", 1, 'B')  );
  resume( create(prntr,  1000,  20, "print C", 1, 'C')  );
}

/*-----------------------------------------------------------------------------
 *  prntr  --  print a character indefinitely
 *-----------------------------------------------------------------------------
 */
prntr(ch)
char	ch;
{
  while ( 1 )
    putc(CONSOLE, ch);
}
