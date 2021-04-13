#include <stdio.h>
int workcount = 0;

main()
{
  int nwork, work();

  printf("Enter cycles of work to do: ");
  scanf("%d",&nwork);
  resume(create(work,1000,20,"worker",1,nwork));
  while (1)
    {
      getc(CONSOLE);		/* wait for <CR> from keyboard */
      printf("worker at # %d\n",workcount);
    }
}

work(ncycles)
{
  int i;
  printf("Worker starting\n");
  for (i=1;i<=ncycles;i++)
    {
      docycle(i);		/* do ith work cycle */
      workcount++;		/* count it where parent can see it */
    }
}

docycle(n)
int n;
{
  int i;
  for (i=0;i<n;i++);
}
