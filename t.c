#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    char blah[] = "hello world\r\ntest 13 4\r\njkekejkeje ejk je\r\nkrlkklk\r\nerjekjkejekjejekjej88888";

    char *p = strtok(blah, "\r\n");
    while (p != NULL)
    {
        puts(p);
        p = strtok(NULL, "\r\n");
    }
}
