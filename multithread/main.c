#include <stdio.h>

int main(int argc, char * argv[])
{
    if(argc != 3) {
        printf("USAGE: The first argument is the name of the database input file\n");
        printf("The second is the name of the book order input file.\n");
        printf("The third is the list of category names separated by blanks in a single "
                "or double quoted string.\n");
        return 0;
    }

    FILE *dbFile = fopen(argv[1], "r");
    FILE *bookFile = fopen(argv[2], "r");
}
