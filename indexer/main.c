#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char * argv[])
{
   DIR * dir;
   struct dirent * entry;
   extern int errno;

   if(argc < 2) {
        printf("Need a file to open\n");
        return 0;
   }

   FILE * index = fopen("test", "r");

   if(index == NULL) {
       fprintf(stderr, "Could not open index file %s\n", argv[1]);
       return 0;
   }

   char * lineptr = NULL;
   size_t lineSize = 0;
   ssize_t read;

   while( (read = getline(&lineptr, &lineSize, index)) != -1) {
       printf("%s\n", lineptr);
   }

   fclose(index);
   return 0;
}



/*
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
