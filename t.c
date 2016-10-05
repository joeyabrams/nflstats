#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    char *p;
    p = realloc(p, 1024);
    strcpy(p, "hello world");
    puts(p);
}
