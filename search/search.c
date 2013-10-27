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

struct listNode *andList(struct listNode *list1, struct listNode *list2){//this method merges head, and list together, and stores the differences at the end of head
    listNode *tempNode, *found, *newFinalList = NULL, *list1Iter = list1, *list2Iter = list2;
    int same = 0;

    if(list1 == NULL || list2 == NULL){
        return NULL;
    }

    //do a logical anding of the two lists


    return NULL;
}

struct listNode *addToListOR(struct listNode *list, struct listNode *head){//this method merges head, and list together, and stores the differences at the end of head
    listNode *tempNode, *current = list, *mergedCurrent = head;
    int same = 0;
    if(current == NULL){
        current = head;
        return current;
    }
    while(mergedCurrent != NULL){
        same = 0;
        while(current != NULL){
            if(strcmp(current->fileName, mergedCurrent->fileName) == 0){
                same = 1;
                break;
            }
            current = current->next;
        }
        tempNode = list;
        if(same == 0){
            while(tempNode->next != NULL){
                tempNode = tempNode->next;
            }
            tempNode->next = mergedCurrent;
        }
        mergedCurrent = mergedCurrent->next;

    }
    return list;
}

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
    struct listNode *head = NULL, *current;
    struct wordHash *s, *wordHashMap= NULL;

    if(argc != 2) {
        printf("USAGE: input index file\n");
        return 1;
    }

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
        head = NULL;
        first = 0;
        for(i = 0; i < strlen(line); i++){
            if(line[i] == ' ' && first == 0){
                word = substring (line, 0, i);
                //printf("%s\n", word);
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
                        //printf("first: %s\n", temp->fileName);
                        head = temp;
                        current = head;
                        head->next = NULL;
                        tempOpen = 0;
                        tempClose = 0;

                    }
                    else if(tempOpen != 0 && tempClose != 0 && head != NULL){
                        listNode *temp = malloc(sizeof(listNode));
                        temp->fileName = substring(line, tempOpen, tempClose - tempOpen);
                        //printf("next: %s\n", temp->fileName);
                        current->next = temp;
                        current = temp;
                        temp->next = NULL;
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
    listNode *tempList, *finalList = NULL;
    char *currentWord;
    struct wordHash *j;
    while(1) { //keep asking user for input
        char option[3];
        char *buffer = NULL;
        unsigned int len;
        int read;
        printf("sa, so, or q?\n");
        scanf("%s", option);
        if(strcmp(option, "q") == 0){
            break;
        }
        printf("Enter names of the words to be searched\n");
        //Consume the '\n' --must be a better way
        read = getline(&buffer, &len, stdin);
        read = getline(&buffer, &len, stdin);
        if(read == -1){
            printf("getline failed");
            return 1;
        }
        if(strcmp(option, "so") == 0){
            char *delim = " \n";
            currentWord = strtok(buffer, delim);
            printf("currentWord is %s\n", currentWord);
            while(currentWord != NULL){
                for(j = wordHashMap; j != NULL; j=j->hh.next){
                    if(strcmp(j->word, currentWord) == 0){
                        finalList = addToListOR(finalList, j->head);
                        break;
                    }
                }
                currentWord = strtok(NULL, delim);
                printf("currentWord is %s\n", currentWord);
            }
            tempList = finalList;
            while(tempList != NULL){
                printf("%s\n", tempList->fileName);
                tempList = tempList->next;
            }
        }
        else if(strcmp(option, "sa") == 0){
            printf("TODO\n");
        }
        else{
            printf("Please enter 'so', 'sa' or 'q'\n");
        }
    }

    return 0;
}
