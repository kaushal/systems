#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>

typedef struct MemEntry MemEntry;

struct MemEntry{
    unsigned int size;
    MemEntry *prev;
    MemEntry *succ;
    char isfree;
};

void *my_malloc( unsigned int size , const char *file, const unsigned long line);
void my_free( void * p , const char *file, const unsigned long line);

#define malloc( x ) my_malloc( x, __FILE__ , __LINE__ );
#define free( x ) my_free( x, __FILE__ , __LINE__ );




