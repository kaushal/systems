#include	"malloc2_prevs.h"
#include	<unistd.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>

struct MemEntry *	root = 0;

// Full-scale malloc() implementation using sbrk().
void *my_malloc( unsigned int size , const char *file, const unsigned long line)
{
    static struct MemEntry *	last = 0;
    struct MemEntry *		p;
    struct MemEntry *		succ;

    p = root;

    if(size < 1){
        fprintf(stderr, "Error: trying to malloc invalid size < 1 byts in file %s, and line: %ld \n", file, line);
    }

    while ( p != 0 )
    {
        if ( p->size < size )
        {
            p = p->succ;					// too small
        }
        else if ( !p->isfree )
        {
            p = p->succ;					// in use
        }
        else if ( p->size < (size + sizeof(struct MemEntry)) )
        {
            p->isfree = 0;					// too small to chop up
            return (char *)p + sizeof(struct MemEntry);
        }
        else
        {
            succ = (struct MemEntry *)((char *)p + sizeof(struct MemEntry) + size);
            succ->prev = p;
            succ->succ = p->succ;
            //p->succ->prev = succ;
            //begin add
            if(p->succ != 0)
                p->succ->prev = succ;
            //end add
            p->succ = succ;
            succ->size = p->size - sizeof(struct MemEntry) - size;
            succ->isfree = 1;
            p->size = size;
            p->isfree = 0;
            last = (p == last) ? succ : last;
            return (char *)p + sizeof(struct MemEntry);
        }
    }
    if ( (p = (struct MemEntry *)sbrk( sizeof(struct MemEntry) + size )) == (void *)-1 )
    {
        fprintf(stderr, "Error: trying to malloc more than possible in file: %s, and line: %ld\n", file, line);
        return 0;
    }

    else if ( last == 0 )				// first block created
    {
        printf( "BKR making first chunk size %ld\n", size );
        p->prev = 0;
        p->succ = 0;
        p->size = size;
        p->isfree = 0;
        root = last = p;
        return (char *)p + sizeof(struct MemEntry);
    }
    else						// other blocks appended
    {
        printf( "BKR making another chunk size %ld\n", size );
        p->prev = last;
        p->succ = last->succ;
        p->size = size;
        p->isfree = 0;
        last->succ = p;
        last = p;
        return (char *)p + sizeof(struct MemEntry);
    }
    return 0;
}

void my_free( void * p , const char *file, const unsigned long line)
{
    struct MemEntry *		ptr;
    struct MemEntry *		pred;
    struct MemEntry *		succ;
    int checker = 0;
    ptr = root;
    while(ptr != NULL){
        struct MemEntry *temp = (struct MemEntry *)(p - sizeof(struct MemEntry));
        void * ptrChunk = ptr;
        //+ ptr->size + sizeof(struct MemEntry);
        void *test = ptr + sizeof(MemEntry);
        if ((struct MemEntry *)temp > (struct MemEntry *)ptr && (struct MemEntry *)temp < (struct MemEntry *)ptrChunk && !ptr->isfree) {
            fprintf(stderr, "Error: freeing pointer to dynamic memory that were not return from malloc in file %s, and line: %ld \n", file, line);
            return;
        }
        if(ptr == temp ){
            if (ptr->isfree) {
                fprintf(stderr, "Error: freeing pointer freeing memory already freed in file %s, and line: %ld\n", file, line);
                return;
            }
            else {
                checker = 1;
            }
        }
        ptr = ptr->succ;
    }

    if (checker == 0) {
        fprintf(stderr, "Error: memory location never malloc'd in file %s, and line: %ld\n", file, line);
        return;
    }

    ptr = (struct MemEntry *)((char *)p - sizeof(struct MemEntry));
    if ( (pred = ptr->prev) != 0 && pred->isfree )
    {
        pred->size += sizeof(struct MemEntry) + ptr->size;	// merge with predecessor

        pred->succ = ptr->succ;
        //begin added
        ptr->isfree=1;
        pred->succ = ptr->succ;
        if(ptr->succ != 0)
            ptr->succ->prev = pred;
        //end added
        printf( "BKR freeing block %#x merging with predecessor new size is %ld.\n", p, pred->size );
    }
    else
    {
        printf( "BKR freeing block %#x.\n", p );
        ptr->isfree = 1;
        pred = ptr;
    }
    if ( (succ = ptr->succ) != 0 && succ->isfree )
    {
        pred->size += sizeof(struct MemEntry) + succ->size;	// merge with successor
        pred->succ = succ->succ;
        //begin added
        pred->isfree = 1;

        if(succ->succ != 0)
            succ->succ->prev=pred;
        //end added
        printf( "BKR freeing block %#x merging with successor new size is %ld.\n", p, pred->size );
    }
}
