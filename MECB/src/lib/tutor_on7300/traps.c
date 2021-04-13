static char *trapnames[] = {"reset sp", \
 "reset pc","Bus Error","Address Error (odd address)", \
"Illegal Instruction","Divide by zero", \
"CHK","TRAPV","Privileged Instruction", \
"Trace","Line 1010","Line 1111", \
"Reserved","Reserved","RTE bad stack format", \
"Uninitialized interrupt vector", \
"Reserved", \
"Reserved", \
"Reserved", \
"Reserved", \
"Reserved", \
"Reserved", \
"Reserved", \
"Reserved", \
"Spurious Interrupt", \
"Level 1 Autovectored Interrupt", \
"Level 2 Autovectored Interrupt", \
"Level 3 Autovectored Interrupt", \
"Level 4 Autovectored Interrupt", \
"Level 5 Autovectored Interrupt", \
"Level 6 Autovectored Interrupt", \
"Level 7 Autovectored Interrupt", \
"Trap #0" \
"Trap #1" \
"Trap #2" \
"Trap #3" \
"Trap #4" \
"Trap #5" \
"Trap #6" \
"Trap #7" \
"Trap #8" \
"Trap #9" \
"Trap #10" \
"Trap #11" \
"Trap #12" \
"Trap #13" \
"Trap #14" \
"Trap #15"};

print_trap(int vector)
{
  printf("%s",trapnames[vector]);
}

