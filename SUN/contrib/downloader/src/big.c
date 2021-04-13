/* this file has a BIG image (just for testing) */

/* We need to make this structure large to increase the size of the image */
/* If we don't init it, it will go into bss instead of data portion */
char big[1000000] = {'a','b','c','d','e'};  /* about 1 meg */

main()
{
    kprintf("\n\nThis is a REALLY big image\n\n");
}
