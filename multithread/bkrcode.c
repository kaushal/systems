#include    <errno.h>
#include    <malloc.h>
#include    <pthread.h>
#include    <stdio.h>
#include    <string.h>
#include    <unistd.h>

// Producer-Consumer example program.  Producer prompts for input.

#define FALSE   0
#define TRUE    1
#define SBUFSIZE        16

struct SharedData {
    int         isopen;
    unsigned int    front;
    unsigned int    count;
    unsigned int    bufsize;
    char            buf[SBUFSIZE];
    pthread_mutex_t mutex;
    pthread_cond_t  dataAvailable;
    pthread_cond_t  spaceAvailable;
};

void
initialize( struct SharedData * sptr )      // Looks like a ctor
{
    sptr->isopen = TRUE;
    sptr->front = 0;
    sptr->count = 0;
    sptr->bufsize = SBUFSIZE;
    pthread_mutex_init( &sptr->mutex, 0 );
    pthread_cond_init( &sptr->spaceAvailable, 0 );
    pthread_cond_init( &sptr->dataAvailable, 0 );
}

void *
consumer( void * arg )
{
    struct SharedData * d = (struct SharedData *)arg;
    char            buffer[200];
    int         i;

    pthread_detach( pthread_self() );
    while ( d->isopen )
    {
        pthread_mutex_lock( &d->mutex );
        while ( d->count == 0 )
        {
            pthread_cond_signal( &d->spaceAvailable );      // shout at producer
            printf( "BKR consumer() waits when shared buffer is empty.\n" );
            pthread_cond_wait( &d->dataAvailable, &d->mutex );
        }
        sleep( 1 );     // pretend to do something
        printf( "BKR consumer() now takes %d bytes from buffer.\n", d->count );
        for ( i = 0 ; (d->count > 0) && (i < sizeof(buffer) ) ; i++ )
        {
            buffer[i] = d->buf[d->front];
            d->front = (d->front + 1) % d->bufsize;
            --d->count;
        }
        pthread_cond_signal( &d->spaceAvailable );          // shout at producer
        pthread_mutex_unlock( &d->mutex );
    }
    return 0;
}

void *
producer( void * arg )
{
    struct SharedData * d = (struct SharedData *)arg;
    int         i;
    int         limit;
    int         back;
    char            p2[100];

    pthread_detach( pthread_self() );
    while ( printf( "Enter something->" ), scanf( " %[^\n]\n", p2 ) > 0 )
    {
        pthread_mutex_lock( &d->mutex );
        limit = strlen( p2 ) + 1;       // plus 1 for the \0 byte as well
        for ( i = 0 ; i < limit ; i++ )
        {
            while ( d->count == d->bufsize )
            {
                pthread_cond_signal( &d->dataAvailable );       // shout at consumer
                printf( "BKR producer() >%c< waits when shared buffer is full.\n", p2[i] );
                pthread_cond_wait( &d->spaceAvailable, &d->mutex );
            }
            back = (d->front + d->count) % d->bufsize;
            d->buf[back] = p2[i];
            ++d->count;
        }
        pthread_cond_signal( &d->dataAvailable );               // shout at consumer
        pthread_mutex_unlock( &d->mutex );
    }
    d->isopen = FALSE;      // is this bad?
    return 0;
}

int main( int argc, char ** argv )
{
    pthread_t       ignore;
    char *          func = "main";
    struct SharedData * sharedData;

    if ( (sharedData = (struct SharedData *)malloc( sizeof(struct SharedData) )) == 0 )
    {
        printf( "malloc() failed in %s.\n", func );
        return 1;
    }
    else
    {
        initialize( sharedData );
        pthread_create( &ignore, 0, consumer, sharedData );
        pthread_create( &ignore, 0, producer, sharedData );
        pthread_exit( 0 );      // Leaks the sharedData block, I know.
    }
}
