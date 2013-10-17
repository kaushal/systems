#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <ftw.h>
#include <signal.h>

#include "tokenizer.h"

int writeDirectoriesToFile(const char *, const struct stat *, int);

int main(int argc, char * argv[])
{
    remove("allFiles");//MOVE THIS TO THE END
    char path[1000], *home;
    FILE *fp, *fp2;
    char * token;
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
    while(fgets(line, sizeof(line), fp)){
        if(line[strlen(line) - 1] == '\n'){
            line[strlen(line) - 1] = '\0';
        }
        dir = opendir(line);
        if(dir != NULL){
            continue;
        }
        else{
            printf("%s>>>>>>>>\n", line);
            fp2 = fopen(line, "r");
            while(fgets(line2, sizeof(line2), fp2)){
                printf(">>>>>>%s\n", line2);
                TokenizerT * tokenizer = TKCreate("", line2);
                while((token = TKGetNextToken(tokenizer))) {
                    printf("%s\n", token);
                }
            }
        }
    }

    /*if(index == NULL) {
      fprintf(stderr, "Could not open index file %s\n", argv[1]);
      return 0;
      }

      char * lineptr = NULL;
      size_t lineSize = 0;
      ssize_t read;
      char * token;

      while( (read = getline(&lineptr, &lineSize, index)) != -1) {
      TokenizerT * tokenizer = TKCreate("", lineptr);
      while((token = TKGetNextToken(tokenizer))) {
      printf("%s\n", token);
      }
      }

      fclose(index);*/
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
        printf("Name = %s\n", path);
        if (type == FTW_DNR) printf("Directory %s cannot be traversed.\n",
                path);
    }
    fclose(fp);
    return 0;
}


/*
 *
 DIR * dir;
 struct dirent * entry;
 extern int errno;

 if ( (dir = opendir(argv[1])) == 0) {
 printf("Could not open %s as directory: %s\n", argv[1], strerror(errno));
 }
 else {
 while( (entry = readdir(dir)) != 0)
 {
 printf("%s\n", entry->d_name);
 }
 closedir(dir);
 }
 */
