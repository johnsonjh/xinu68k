/* ex4.c - main, produce, consume  */

int	n = 0;		/* external variables are shared by all processes  */

/*-------------------------------------------------------------------
 *    main --  example of unsynchronized producer and consumer processes
 *---------------------------------------------------------------------
 */
main()
{
  int	produce(), consume();

  resume( create(consume, 1000, 20, "cons", 0)  );
  resume( create(produce, 1000, 20, "prod", 0)  );
}

/*----------------------------------------------------------------------------
 *    produce   --   increment n 2000 times and exit
 *----------------------------------------------------------------------------
 */
produce()
{
  int i;

  for( i=1 ; i<=2000 ; i++ )
    n++;
}

/*----------------------------------------------------------------------------
 *  consume   --   print n 2000 times and exit
 *----------------------------------------------------------------------------
 */
consume()
{
  int i;

  for(  i=1  ;  i<=2000  ; i++  )
    printf("n is %d \n",  n);
}
