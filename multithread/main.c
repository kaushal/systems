#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include "uthash.h"
#include "queue.h"

struct filePointer {
    FILE *fp;
    struct Queue *table;
};

struct customer {
    char * name;
    int customerID;         /* key */
    double balance;
    char * address;
    char * state;
    char * areaCode;
    pthread_mutex_t mutex;
    UT_hash_handle hh;
};

struct bookOrder {
    char * title;
    double price;
    char * customerID;
    char * category;
};

/*Global Hash Tables*/
struct customer *customersHashTable = NULL;
struct Queue *queueHashTable = NULL;

/*Method Declarations*/
void * consumer(void * arg);
struct customer * lookupCustomer(char * customerID);
struct customer * makeCustomer(char *line);
void addCustomer(struct customer *cInfo);
char ** processCategories(char * categories);
void * producer(void *arg);
struct Queue * lookupQueue(char * category);
int producerEmpty = 0;

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
    while(fgets(line, sizeof(line), categoriesFile)) {
        struct Queue *queue = makeQueue();
        line[strlen(line) - 1] = '\0';
        char *newString = strdup(line);
        fprintf(stderr, "%d\n", strlen(newString));
        fprintf(stderr, "------%s-------\n", newString);
        queue->category = newString;

        HASH_ADD_KEYPTR(hh, queueHashTable, queue->category, strlen(queue->category), queue);  /* Arguments: Hash Table, key, value*/
    }
    struct filePointer *fp = malloc(sizeof(struct filePointer));
    fp->fp = bookFile;
    fp->table = queueHashTable;
    pthread_t ignore;
    struct Queue *s;

    pthread_create(&ignore, 0, producer, fp);

    while(producerEmpty == 0){
        fprintf(stderr, "");
    }

    for(s=queueHashTable; s != NULL; s=s->hh.next) {
        struct filePointer *data = malloc(sizeof(struct filePointer));
        data->table = s;
        if(s->category == NULL)
            continue;
        fprintf(stderr, "thread name: %s\n", data->table->category);
        pthread_create(&ignore, 0, consumer, data);
    }
    pthread_exit(0);

}

void * consumer(void * arg)
{
    pthread_detach( pthread_self() );
    struct filePointer *data = (struct filePointer *)arg;
    struct Queue *queue = data->table;
    fprintf(stderr, "about to go into the consumer in the thread %s\n", data->table->category);
    struct QueueNode *temp;
    struct bookOrder *bo;
    while(queue->length > 0){
        temp = (struct QueueNode*)dequeue(queue);
        bo = (struct bookOrder *)temp->data;
        
       struct customer *c = lookupCustomer(bo->customerID); 
       fprintf(stderr, "cat: %s, title: %s \n", bo->category, bo->title);
       if(c == NULL)
           fprintf(stderr, "------------------null\n");
       else    
           fprintf(stderr, "------------------not\n");
        //fprintf(stderr, "queue: %s ,title: %s\n", data->table->category, temp->title);
       // fprintf(stderr, "queue: %s ,length: %d\n", data->table->category, queue->length);
    }
    
    return 0;
}

void * producer(void * arg)
{
    pthread_detach( pthread_self() );
    fprintf(stderr, "about to go into the producer\n");
    struct filePointer *fp = (struct filePointer *)arg;
    FILE *bookFile = fp->fp;
    char line[256];
    while(fgets(line, sizeof(line), bookFile)) {
        struct bookOrder *order = malloc(sizeof(struct bookOrder));
        struct QueueNode *node = malloc(sizeof(struct QueueNode));

        //title
        char * token = strtok(line, "|");
        fprintf(stderr, "%p\n", &token);
        char *tempTitle = malloc(sizeof(1 + strlen(token)));
        strncpy(tempTitle, token, 100000);
        order->title = tempTitle;

        //price
        token = strtok(NULL, "|");
        fprintf(stderr, "%s\n", token);
        char *tempPrice= malloc(sizeof(1 + strlen(token)));
        strcpy(tempPrice, token);
        order->price = atof(tempPrice);

        //customerID
        token = strtok(NULL, "|");
        fprintf(stderr, "%s\n", token);
        char *tempCID= malloc(sizeof(1 + strlen(token)));
        strcpy(tempCID, token);
        order->customerID = tempCID;

        //category
        token = strtok(NULL, "|");
        token[strlen(token) - 1] = '\0';
        char *tempCat= malloc(sizeof(1 + strlen(token)));
        strcpy(tempCat, token);
        order->category = tempCat;

        node->data = order;

        fprintf(stderr, "%s\n", order->title);
        fprintf(stderr, "%f\n", order->price);
        fprintf(stderr, "%d\n", order->customerID);
        fprintf(stderr, "%s\n", order->category);
        struct Queue *queue = lookupQueue(order->category);
        enqueue(queue, node);
        printf("%s enqueued\n", queue->category);
    }
    producerEmpty = 1;
    return 0;
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
    HASH_FIND(hh, queueHashTable, category, strlen(category), q);
    return q;
}


