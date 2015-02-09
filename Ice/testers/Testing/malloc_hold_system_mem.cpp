#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
    int alloc_time = 40000;
    char *a[alloc_time];
    char *b[alloc_time];
    int i, j;
    for(i=0; i<alloc_time; i++)
    {
        a[i] = (char *)malloc(52722);
        memset(a[i], 1, 52722);
        //b[i] = (char *)malloc(16);
        //memset(b[i], 1, 16);
    }
    for(i=0; i<alloc_time; i++)
    {
        //a[i] = (char *)malloc(52722);
        //memset(a[i], 1, 52722);
        b[i] = (char *)malloc(16);
        memset(b[i], 1, 16);
    }
    printf("malloc finished\n");
    for(i=alloc_time-1; i>=0; i--)
    {
        free(a[i]);
        //free(b[i]);
    }
    for(i=alloc_time-1; i>=0; i--)
    {
        //free(a[i]);
        free(b[i]);
    }
    printf("free finished\n");
    char *p = (char *)malloc(110);
    free(p);
    while(1){
        sleep(3);
    }
}
