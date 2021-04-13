#include <stdio.h>
int workcount = 0;

main()
{
  int nwork, ch, work();

  printf("Enter cycles of work to do: ");
/*  scanf("%d",&nwork); */
  nwork = 1000;
  ch = getc(CONSOLE);
  printf("Got %c %d\n",ch,nwork);
  resume(create(work,500,20,"worker",1,nwork));
  printf("Parent back from resume of child\n");
  while (1)
    {
      getc(CONSOLE);		/* wait for <CR> from keyboard */
      printf("worker at # %d\n",workcount);
    }
}

work(ncycles)
{
  int i;
  printf("child starting, ncycles=%d...\n",ncycles);
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
