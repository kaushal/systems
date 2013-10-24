#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "uthash.h"

char *substring(char *string, int position, int length)
{
    char *pointer;
    int c;

    pointer = malloc(length+1);

    if (pointer == NULL)
    {
        printf("Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    for (c = 0 ; c < position -1 ; c++)
        string++;

    for (c = 0 ; c < length ; c++)
    {
        *(pointer+c) = *string;
        string++;
    }

    *(pointer+c) = '\0';

    return pointer;
}

int main(int argc, char *argv[]){
    FILE *fp;
    int i = 0, first = 0;
    fp = fopen(argv[1], "r");

    if(!fp){
        printf("File not found");
        return 0;
    }
    char line[256];
    while(fgets(line, sizeof(line), fp)){
        if(line[strlen(line) - 1] == '\n'){
            line[strlen(line) -1] = '\0';
        }
        char buff[256];
        first = 0;
        for(i = 0; i < strlen(line); i++){
            if(line[i] == ' ' && first == 0){
                char *pointer = substring (line, 0, i);
                printf("%s\n", pointer);
                first = 1;
}
        }
    }
    return 0;
}
