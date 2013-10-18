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

int main(int argc, char * argv[])
{
    remove("allFiles");//MOVE THIS TO THE END
    char path[1000], *home;
    FILE *fp, *fp2;
    char * token;
    struct wordHash *s, *tmp, *words = NULL;

    if(argc < 2) {
        printf("Need a file to open\n");
        return 0;
    }
    //TODO: lowercase eveything

    DIR *dir;
    dir = opendir(argv[1]);
    if(dir == NULL){
        printf("------------NULL-----------");
        fp = fopen("allFiles", "a+");
        fprintf(fp, "%s\n", argv[1]);
        fclose(fp);
    }
    else{
        home = getenv("HOME");
        printf("%s------------------%s-------------\n", path, home);
        printf(path, "%s/sysprog", home);
        ftw(argv[1], writeDirectoriesToFile, 7);
    }

    fp = fopen("allFiles", "r");
    char line[256];
    char line2[256];
    UT_hash_handle hh;
    UT_hash_handle hhh;
    while(fgets(line, sizeof(line), fp)){
        if(line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = '\0';
        }
        dir = opendir(line);
        if(dir != NULL){
            continue;
        }
        else{
            //printf("%s>>>>>>>>\n", line);
            fp2 = fopen(line, "r");
            while(fgets(line2, sizeof(line2), fp2)){
                //printf(">>>>>>%s\n", line2);
                TokenizerT * tokenizer = TKCreate("", line2);
                s = (struct wordHash*)malloc(sizeof(struct wordHash));
                while((token = TKGetNextToken(tokenizer))) {

                    HASH_FIND_STR(words, token, tmp);
                    if(tmp){
                        printf("\ni am here adding: %s\n", token);
                        listNode *current, *previous, *tempNode;
                        current = tmp->head;
                        previous = current;
                        int added = 0;
                        while(current != NULL){
                            if(strcmp(current->fileName, (char*)line) == 0){
                                printf("\nfile name: %s\n", current->fileName);
                                printf("\nfile name: %s\n", line);
                                current->count++;
                                added = 1;
                                printf("_______%d-----------\n", current ->count);
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
                            l1->next = NULL;
                            printf("_______%d---===========--------\n", l1->count);
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
                        printf("\ni am here adding: ----------------------%s\n", token);
                        HASH_ADD_KEYPTR(hh, words, s->word, strlen(s->word), s);
                    }
                    //printf("%s\n", token);
                }
            }
        }
    }
    HASH_SRT(hh, words, sort_by_name);
    struct wordHash *j;

    //Prints in order
    for(j= words; j != NULL; j=j->hh.next) {
        printf("word is %s\n", j->word);
    }

    //Free Hash Table
    struct wordHash *current, *next;

    HASH_ITER(hh, words, current, next) {
        HASH_DEL(words, current);
        free(current);
    }

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

