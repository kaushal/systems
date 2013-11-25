#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include "uthash.h"
#include "queue.h"

struct filePointer {
    FILE *fp;
};

struct customer {
    char * name;
    int customerID;         /* key */
    double balance;
    char * address;
    char * state;
    char * areaCode;
    UT_hash_handle hh;
};

struct bookOrder {
    char * title;
    double price;
    int customerID;
    char * category;
};

/*Global Hash Tables*/
struct customer *customersHashTable = NULL;
struct Queue *queueHashTable = NULL;

/*Method Declarations*/
struct customer * makeCustomer(char *line);
void addCustomer(struct customer *cInfo);
char ** processCategories(char * categories);
<<<<<<< HEAD
void * producer(void *arg);
=======
struct Queue * lookupQueue(char * category);
>>>>>>> 54590a3c9c7c7250ad843d7669e83a1fbf1ba1eb

int main(int argc, char * argv[])
{
    /*Argument check */
    if(argc != 4) {
        printf("USAGE: The first argument is the name of the database input file\n");
        printf("The second is the name of the book order input file.\n");
        printf("The third is the name of the category input file\n");
        return 0;
    }

    /*Read and check files*/
    FILE *dbFile = fopen(argv[1], "r");
    FILE *bookFile = fopen(argv[2], "r");
    FILE *categoriesFile = fopen(argv[3], "r");
    if( dbFile == NULL || bookFile == NULL || categoriesFile == NULL) {
        printf("Error reading file\n");
        return 1;
    }

    char line[256];
    /*Create customers hash table*/
    while(fgets(line, sizeof(line), dbFile)) {
       struct customer *newCustomer =  makeCustomer(line);
       addCustomer(newCustomer);
    }

    /* Process Categories */
    UT_hash_handle hh;
    while(fgets(line, sizeof(line), categoriesFile)) {
        struct Queue *queue = makeQueue();
        queue->category = line;
        HASH_ADD_KEYPTR(hh, queueHashTable, line, strlen(line), queue);  /* Arguments: Hash Table, key, value*/
    }
    struct filePointer *fp = malloc(sizeof(struct filePointer));
    fp->fp = bookFile;
    pthread_t ignore;

    fprintf(stderr, "about to go into the thread\n");

<<<<<<< HEAD
    pthread_create(&ignore, 0, producer, fp);
=======
    struct Queue *queue = lookupQueue("SPORTS01\n");
    printf("queue test: %s\n", queue->category);

>>>>>>> 54590a3c9c7c7250ad843d7669e83a1fbf1ba1eb
    pthread_exit(0);
}

struct customer * makeCustomer(char *line)
{
    struct customer *c = malloc(sizeof(struct customer));

    char * token = strtok(line, "|");
    c->name = token;
    token = strtok(NULL, "|");
    c->customerID = atoi(token);
    token = strtok(NULL, "|");
    c->address = token;
    token = strtok(NULL, "|");
    c->state = token;
    token = strtok(NULL, "|");
    c->areaCode = token;
    return c;
}

void addCustomer(struct customer *cInfo)
{
    HASH_ADD_INT(customersHashTable, customerID, cInfo );  /* Arguments: Hash Table, key, value*/
}

struct customer * lookupCustomer(char * customerID)
{
    struct customer *c;
    HASH_FIND_STR(customersHashTable, customerID, c);
    return c;
}

struct Queue * lookupQueue(char * category)
{
    struct Queue *q;
    HASH_FIND_STR(queueHashTable, category, q);
    return q;
}

void * producer(void * arg)
{
    fprintf(stderr, "in thread\n");
    struct filePointer *fp = (struct filePointer *)arg;
    FILE *bookFile = fp->fp; 
    char line[256];
    while(fgets(line, sizeof(line), bookFile)) {
        struct bookOrder *order = malloc(sizeof(struct bookOrder));
        char * token = strtok(line, "|");
        order->title = token;
        token = strtok(NULL, "|");
        order->price = atof(token);
        token = strtok(NULL, "|");
        order->customerID = atoi(token);
        token = strtok(NULL, "|");
        order->category= token;
        
        fprintf(stderr, "%s\n", order->category);
        struct Queue *queue = lookupQueue(order->category);
        fprintf(stderr, "----%s-----\n", queue->category);
        enqueue(queue,order);
        fprintf(stderr, "here\n");
        printf("----%s-----\n", queue->category);
    }
    return 0;
}


