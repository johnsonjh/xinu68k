/* program to show that normal C i/o works on the MECB */

extern  breakpoint();
char msg[] = "testing";
int a;
char buf[80];
main()
{
  char ch;

  a = 3;
  printf("\nWelcome to C on the ATT7300\n\n");
  printf("%s, %s, %d, %d, %d...\n",msg,msg,1,2,a);
  printf("Now enter a char: ");
  ch = getchar();
  printf("\nNote how it was picked up and used before you typed <CR>.\n");
  printf("Here it is, via putchar: ");
  putchar(ch);
  printf("\nNow enter a word: ");
  scanf("%s",buf);
  printf("\nscanf got %s\n",buf);
}
