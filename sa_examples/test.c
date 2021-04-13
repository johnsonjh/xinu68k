/* program to show that normal C i/o works on the MECB */
char msg[] = "testing";
int a;
char buf[80];
main()
{
  char ch;

  a = 3;
  printf("\nWelcome to C on this machine, whatever it is\n\n");
  printf("%s, %s, %d, %d, %d...\n",msg,msg,1,2,a);
  printf("Single-char input test using getchar:\n");
  printf("Now enter a char: ");
  ch = getchar();
  printf("\nIf running without an OS, note how it was picked up and used\n before you typed <CR>.\n");
  printf("\nIf running with an OS, note how it waited until you typed <CR>.\n");
  printf("Here it is, via putchar: ");
  putchar(ch);
  printf("\nMultiple-char input test using scanf:\n");
  printf("Now enter a word: ");
  scanf("%s",buf);
  printf("\nscanf got %s\n",buf);
  printf("\nNow enter a number: ");
  scanf("%d",&a);
  printf("\nscanf got %d\n",a);
}
