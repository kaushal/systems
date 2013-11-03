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

/*
 * Parameters: target word, hashMap
 * Returns: linked-list of target word, NULL if not found
 *
 * Searches for a word and returns its linked list
 */
struct listNode *makeList(char *searchWord, struct wordHash *wordHashMap)
{
    if(searchWord == NULL || wordHashMap == NULL){
        return NULL;
    }

    struct wordHash *j;
    for(j = wordHashMap; j != NULL; j=j->hh.next){
        if(strcmp(j->word, searchWord) == 0){
            return j->head;
        }
    }
    return NULL;
}

/* Parameters: Two Linked-Lists
 * Returns: Logical AND of lists
 * this method merges head, and list together,
 * and stores the differences at the end of head
 */
struct listNode *addToListAND(struct listNode *list1, struct listNode *list2)
{
     listNode *finalList = NULL,
              *list1Iter = list1,
              *list2Iter = list2;

    // V not sure if we want to do this V
    if(list1 == NULL || list2 == NULL){
        return NULL;
    }

    //do a logical anding of the two lists
    while(list1Iter != NULL){
        while(list2Iter != NULL){
            if(strcmp(list1Iter->fileName, list2Iter->fileName) == 0) {
                listNode *finalListHead = malloc(sizeof(listNode));
                finalListHead->fileName = list1Iter->fileName;
                finalListHead->count = list1Iter->count;
                finalListHead->next = finalList;
                finalList = finalListHead;
            }
            list2Iter = list2Iter->next;
        }
        list1Iter = list1Iter->next;
        list2Iter = list2;
    }
    return finalList;
}

/*this method merges head, and list together,
 * and stores the differences at the end of head
 */
struct listNode *addToListOR(struct listNode *list, struct listNode *head){
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

    if (pointer == NULL) {
        printf("Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    for (c = 0 ; c < position -1 ; c++) {
        string++;
    }

    for (c = 0 ; c < length ; c++){
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
        //QUIT
        if(strcmp(option, "q") == 0){
            break;
        }
        if(read == -1){
            printf("getline failed");
            return 1;
        }
        //SEARCH OR
        if(strcmp(option, "so") == 0){
            printf("Enter names of the words to be searched\n");
            //Consume the '\n' --must be a better way
            read = getline(&buffer, &len, stdin);
            read = getline(&buffer, &len, stdin);
            char *delim = " \n\t";
            currentWord = strtok(buffer, delim);
            while(currentWord != NULL){ //Loop through user input
                for(j = wordHashMap; j != NULL; j=j->hh.next){
                    if(strcmp(j->word, currentWord) == 0){
                        finalList = addToListOR(finalList, j->head);
                        break;
                    }
                }
                currentWord = strtok(NULL, delim);
            }
            tempList = finalList;
            while(tempList != NULL){
                printf("%s\n", tempList->fileName);
                tempList = tempList->next;
            }
        }
        //SEARCH AND
        else if(strcmp(option, "sa") == 0){
            printf("Enter names of the words to be searched\n");
            //Consume the '\n' --must be a better way
            read = getline(&buffer, &len, stdin);
            read = getline(&buffer, &len, stdin);
            char *delim = " \n\t";
            char *firstWord = strtok(buffer, delim);
            //This is an option that we may want to change
            if(firstWord == NULL){
                printf("SA requires at least one word\n");
                continue;
            }
            finalList = makeList(firstWord, wordHashMap);
            currentWord = strtok(NULL, delim);
            while(currentWord != NULL){ //Loop through user input
                listNode *currentList = makeList(currentWord, wordHashMap);
                finalList = addToListAND(finalList, currentList);
                currentWord = strtok(NULL, delim);
            }
            listNode *temp = finalList;
            printf("Answer is:\n");
            while(temp != NULL) {
                printf("%s ", temp->fileName);
                temp = temp->next;
            }
            printf("\n");
        }
        else{
            printf("Please enter 'so', 'sa' or 'q'\n");
        }
    }

    return 0;
}
