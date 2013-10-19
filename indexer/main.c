#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <ftw.h>
#include <signal.h>
#include "uthash.h"
#include "tokenizer.h"

#define BUFLEN 20

int writeDirectoriesToFile(const char *, const struct stat *, int);

typedef struct listNode {
    struct listNode *next;
    char *fileName;
    int count;
} listNode;

struct wordHash {
    const char *word;
    struct el *sortedList;
    UT_hash_handle hh;
    struct listNode *head;
};

int sort_by_name(struct wordHash *a, struct wordHash *b) {
  return strcmp(a->word, b->word);
}

void toLowerCase(char *word)
{
    int i;
    for(i=0; i<strlen(word); i++) {
        word[i] = tolower(word[i]);
    }
}

int main(int argc, char * argv[])
{
    char path[1000], *home;
    FILE *fp, *fp2, *fp3;
    FILE *fileDest = fopen(argv[1], "w");
    char * token;
    struct wordHash *s, *tmp, *words = NULL;

    if(argc != 3) {
        printf("USAGE: The first argument is file to be written to. The second is a file or directory to be read.\n");
        return 0;
    }

    DIR *dir;
    dir = opendir(argv[2]);
    if(dir == NULL){
        fp3 = fopen(argv[2], "r");
        if(!fp3) {
            printf("Not a valid file or directory\n");
            return 1;
        }
        fp = fopen("allFiles", "a+");
        fprintf(fp, "%s\n", argv[2]);
        fclose(fp);
    }
    else{
        home = getenv("HOME");
        printf(path, "%s/sysprog", home);
        ftw(argv[2], writeDirectoriesToFile, 7);
    }

    fp = fopen("allFiles", "r");
    char line[256];
    char line2[256];
    while(fgets(line, sizeof(line), fp)){
        if(line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = '\0';
        }
        dir = opendir(line);
        if(dir != NULL){
            continue;
        }
        else{
            fp2 = fopen(line, "r");
            while(fgets(line2, sizeof(line2), fp2)){
                TokenizerT * tokenizer = TKCreate("", line2);
                s = (struct wordHash*)malloc(sizeof(struct wordHash));
                while((token = TKGetNextToken(tokenizer))) {
                    toLowerCase(token);
                    HASH_FIND_STR(words, token, tmp);
                    if(tmp){
                        listNode *current, *previous, *tempNode;
                        current = tmp->head;
                        previous = current;
                        int added = 0;
                        while(current != NULL){
                            if(strcmp(current->fileName, (char*)line) == 0){
                                current->count++;
                                added = 1;
                                if(current->next != NULL ){
                                    while(current != NULL && current->count > current->next->count){
                                        tempNode = current->next;
                                        current->next = tempNode->next;
                                        tempNode->next = current;
                                        if(previous == tmp->head) {
                                            tmp->head = tempNode;
                                        }
                                        previous = current;
                                        current = current->next;
                                    }
                                }
                                break;
                            }
                            previous = current;
                            current = current->next;
                        }
                        if(added == 0){
                            listNode *l1 = malloc(sizeof(listNode));
                            l1->fileName = malloc(sizeof(line));
                            strcpy(l1->fileName, line);
                            l1->count = 1;
                            l1->next = NULL;
                            previous->next = l1;
                        }
                    }
                    else{
                        s = (struct wordHash*)malloc(sizeof(struct wordHash));
                        //add to outer
                        s->word = token;

                        listNode *l1 = malloc(sizeof(listNode));
                        l1->fileName = malloc(sizeof(line));
                        strcpy(l1->fileName, line);
                        l1->count = 1;
                        l1->next = NULL;
                        s->head = l1;

                        //make inner
                        HASH_ADD_KEYPTR(hh, words, s->word, strlen(s->word), s);
                    }
                }
            }
        }
    }

    HASH_SRT(hh, words, sort_by_name);
    struct wordHash *j;

    //Prints in order
    char *tempFileName;
    for(j= words; j != NULL; j=j->hh.next) {
        fprintf(fileDest, "%s --> ", j->word);
        listNode *current = j->head, *tempFileNode, *tempFileNodePrev, *prev;
        prev = current;
        int fiveCount = 1;
        while(j->head != NULL) {
            int maxCount = 0;
            while(current != NULL){
                if(current->count > maxCount){
                    maxCount = current->count;
                    tempFileName = current->fileName;
                    tempFileNodePrev = prev;
                    tempFileNode = current;
                }
                prev = current;
                current = current->next;
            }

            if(tempFileNode == j->head){
                j->head = tempFileNode->next;
            }
            else{
                tempFileNodePrev->next = tempFileNode->next;
            }
            current = j->head;
            prev = current;
            fprintf(fileDest, "(%s , %d), ", tempFileName, maxCount);
            if(fiveCount % 6 == 0) {
                fprintf(fileDest, "\n");
            }
            fiveCount++;
            //delete the node
        }
        fprintf(fileDest, "\n");
    }

    //Free Hash Table
    struct wordHash *current, *next;

    HASH_ITER(hh, words, current, next) {
        HASH_DEL(words, current);
        free(current);
    }

    remove("allFiles");//MOVE THIS TO THE END

    return 0;
}

int writeDirectoriesToFile(const char *path, const struct stat *sptr, int type)
{
    /* Types: FTW_D = directory, FTW_F = normal file, FTW_DNR = non-traversable
       directory.  Do a man ftw for the full scoop!!!!*/
    FILE *fp = fopen("allFiles", "a+");

    fprintf(fp, "%s\n", path);
    if (strstr(path,"test"))
    {
        if (type == FTW_DNR) printf("Directory %s cannot be traversed.\n",
                path);
    }
    fclose(fp);
    return 0;
}

