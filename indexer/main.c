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
        printf("Must specify directory name\n");
   }

   else if ( (dir = opendir(argv[1])) == 0) {
       printf("Could not open %s as directory: %s\n", argv[1], strerror(errno));
   }
   else {
       while( (entry = readdir(dir)) != 0)
       {
           printf("%s\n", entry->d_name);
       }
       closedir(dir);
   }
}
