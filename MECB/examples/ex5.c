/* ex5.c - main, prod2, cons2  */

int	n = 0;

/*-------------------------------------------------------------------
 *    main --  producer and consumer process synchronized with semaphores
 *---------------------------------------------------------------------
*/
main()
{
  int	prod2(), cons2();
  int	produced, consumed;

  consumed = screate(0);
  produced = screate(1);
  resume( create(cons2, 1000, 20, "cons", 2, consumed, produced)  );
  resume( create(prod2, 1000, 20, "prod", 2, consumed, produced)  );
}

/*----------------------------------------------------------------------------
 *    prod2   --   increment n 2000 times, waiting for it to be consumed
 *----------------------------------------------------------------------------
 */
prod2(consumed, produced)
{
  int i;

  for( i=1 ; i<=2000 ; i++ )  {
    wait(consumed); 
    n++;
    signal(produced);
  }
}

/*----------------------------------------------------------------------------
 *  cons2   --   print n 2000 times, waiting for it to be produced
 *----------------------------------------------------------------------------
 */
cons2(consumed, produced)
{
	int i;

	for(  i=1  ;  i<=2000  ; i++  )  {
		wait(produced);
		printf("n is %d \n",  n);
		signal(consumed);
	}
}

