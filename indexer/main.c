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

int writeDirectoriesToFile(const char *, const struct stat *, int);

struct fileHash {
    const char* filePath;
    int count;
    UT_hash_handle hh;
};

struct wordHash {
    const char* word;
    struct fileHash *fhash;
    UT_hash_handle hh;
};

int main(int argc, char * argv[])
{
    remove("allFiles");//MOVE THIS TO THE END
    char path[1000], *home;
    FILE *fp, *fp2;
    char * token;
    struct wordHash *s, *tmp, *words = NULL;
    struct fileHash *s2, *tmp2, *files = NULL;
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
                    if(tmp){//if we've seen the word before

                        /*printf("found: ");
                        printf("%s\n", token);
                        s2 = (struct fileHash*)malloc(sizeof(struct fileHash));
                        HASH_FIND_STR(s->fhash, line, tmp2);
                        if(tmp2){//it appeared before in the file
                            printf("%s\n", line);
                            tmp2->count = tmp2->count;
                            printf("%d\n", tmp2->count);
                        }
                        else{//first time we're seeing it in this file
                            s2 = (struct fileHash*)malloc(sizeof(struct fileHash));
                            s2->filePath = line;
                            s2->count = 1;
                            tmp->fhash = s2;
                            printf("%d\n", s2->count);
                        }*/
                    }
                    else{
                        s = (struct wordHash*)malloc(sizeof(struct wordHash));
                        //add to outer
                        s->word = token;

                        //make inner
                        s2 = (struct fileHash*)malloc(sizeof(struct fileHash));
                        s->fhash = (struct fileHash*)malloc(sizeof(struct fileHash));
                        s2->filePath = line;
                        s2->count = 1;
                        printf("\ni am here adding: %s\n", token);
                        s->fhash = s2;
                        HASH_ADD_KEYPTR(hh, files, s2->filePath, strlen(s2->filePath), s2);
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

