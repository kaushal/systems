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

typedef struct el {
    char bname[BUFLEN];
    struct el *next, *prev;
} el;

struct wordHash {
    const char *word;
    struct el *sortedList;
    UT_hash_handle hh;
};

int namecmp(el *a, el *b) {
    return strcmp(a->bname,b->bname);
}

el *head = NULL; /* important- initialize to NULL! */

int main(int argc, char * argv[])
{
    remove("allFiles");//MOVE THIS TO THE END
    char path[1000], *home;
    FILE *fp, *fp2;
    char * token;
    struct wordHash *s, *tmp, *words = NULL;
    el *name, *elt, *tmpList, etmp;

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

                    }
                    else{
                        s = (struct wordHash*)malloc(sizeof(struct wordHash));
                        //add to outer
                        s->word = token;

                        //make inner
                        printf("\ni am here adding: %s\n", token);
                        HASH_ADD_KEYPTR(hh, words, s->word, strlen(s->word), s);
                    }
                    //printf("%s\n", token);
                }
            }
        }
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

