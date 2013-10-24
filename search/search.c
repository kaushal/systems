#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include "uthash.h"

typedef struct listNode {
    struct listNode *next;
    char *fileName;
    int count;
} listNode;

struct wordHash {
    struct listNode *head;
    const char *word;
    UT_hash_handle hh;
};

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
    int tempOpen = 0, tempClose = 0;
    char *word;
    struct listNode *head = NULL, *node = NULL, *current;
    struct wordHash *s, *tmp, *wordHashMap= NULL;
    fp = fopen(argv[1], "r");

    if(!fp){
        printf("File not found\n");
        return 0;
    }
    char line[256];
    while(fgets(line, sizeof(line), fp)){
        if(line[strlen(line) - 1] == '\n'){
            line[strlen(line) -1] = '\0';
        }
        char buff[256];
        head = NULL;
        first = 0;
        for(i = 0; i < strlen(line); i++){
            if(line[i] == ' ' && first == 0){
                word = substring (line, 0, i);
                printf("%s\n", word);
                tempOpen = 0;
                tempClose = 0;
                for(; i < strlen(line); i++){
                    if(line[i] == '('){
                        tempOpen = i+2;
                    }
                    if(line[i] == ' ' && tempOpen != 0){
                        tempClose = i + 1;
                    }
                    if(tempOpen != 0 && tempClose != 0 && head == NULL){//add to linked list
                        listNode *temp = malloc(sizeof(listNode));
                        temp->fileName = substring(line, tempOpen, tempClose - tempOpen);
                        printf("first: %s\n", temp->fileName);
                        head = temp;
                        current = head;
                        tempOpen = 0;
                        tempClose = 0;

                    }
                    else if(tempOpen != 0 && tempClose != 0 && head != NULL){
                        listNode *temp = malloc(sizeof(listNode));
                        temp->fileName = substring(line, tempOpen, tempClose - tempOpen);
                        printf("next: %s\n", temp->fileName);
                        current->next = temp;
                        current = temp;
                        tempOpen = 0;
                        tempClose = 0;
                    }
                }
                s = (struct wordHash*)malloc(sizeof(struct wordHash));
                s->word = word;
                s->head = head;
                HASH_ADD_KEYPTR(hh, wordHashMap, word, strlen(word), s);
                first = 1;
                break;
            }
        }

    }
    return 0;
}
