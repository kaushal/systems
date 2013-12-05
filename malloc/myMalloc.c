#include<stdlib.h>
#include<stdio.h>

int main() {
    int i = 0;
    char * mem = (char *)calloc(1, 5000);
    for(i = 0; i < 5000; i++)
        mem[i] = 0;

    fprintf(stderr, "%d\n", mem[500]);

    return 0;
}
