#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include "uthash.h"
#include "tokenizer.h"
#include "queue.h"

struct filePointer {
    FILE *fp;
    struct Queue *table;
};

struct llNode {
    struct llNode *next;
    struct bookOrder *order;
};

struct customer {
    char * name;
    char * customerID;         /* key */
    double balance;
    char * address;
    char * state;
    char * areaCode;
    struct llNode *head;
    struct llNode *successHead;
    struct llNode *failedHead;
    pthread_mutex_t mutex;
    UT_hash_handle hh;
};

struct bookOrder {
    char * title;
    double price;
    char * customerID;
    char * category;
    double currBal;
    int success;
};

struct llNode *globalLL = NULL;
/*Global Hash Tables*/
int consumerThreadCount = 0;
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
        struct customer *tempCustomer;
        HASH_FIND_STR(customersHashTable, newCustomer->customerID, tempCustomer);
    }
    /* Process Categories */
    while(fgets(line, sizeof(line), categoriesFile)) {
        struct Queue *queue = makeQueue();
        line[strlen(line) - 1] = '\0';
        char *newString = strdup(line);
        queue->category = newString;

        HASH_ADD_KEYPTR(hh, queueHashTable, queue->category, strlen(queue->category), queue);  /* Arguments: Hash Table, key, value*/
    }
    struct filePointer *fp = malloc(sizeof(struct filePointer));
    fp->fp = bookFile;
    fp->table = queueHashTable;
    pthread_t ignore;
    struct Queue *s;

    pthread_create(&ignore, 0, producer, fp);
    int threadCount = 0;
    for(s=queueHashTable; s != NULL; s=s->hh.next) {
        while(producerEmpty == 0){;}
        struct filePointer *data = malloc(sizeof(struct filePointer));
        data->table = s;
        if(s->category == NULL)
            continue;
        pthread_create(&ignore, 0, consumer, data);
        threadCount++;
    }
    struct customer *c;
    while(consumerThreadCount != threadCount){;}
    for(c=customersHashTable; c != NULL; c=c->hh.next) {
        printf("\n=== BEGIN CUSTOMER INFO ===\n");
        printf("### BALANCE ###\n");
        printf("Customer Name: %s\n", c->name);
        printf("Customer ID: %s\n", c->customerID);
        printf("Remaining credit balance after all purchases (a dollar amount): $%f\n", c->balance);
        printf("### SUCCESSFUL ORDERS ###\n");
        struct llNode *tempHead = globalLL;
        while(tempHead != NULL){
            if(strcmp(c->customerID, tempHead->order->customerID) == 0 && tempHead->order->success == 1)
                printf(" %s | %f | %f\n", tempHead->order->title, tempHead->order->price, tempHead->order->currBal);
            tempHead = tempHead->next;
        }
        printf("### FAILED ORDERS ###\n");
        tempHead = globalLL;
        while(tempHead != NULL){
            if(strcmp(c->customerID, tempHead->order->customerID) == 0 && tempHead->order->success == 0)
                printf(" %s | %f | %f\n", tempHead->order->title, tempHead->order->price, tempHead->order->currBal);
            tempHead = tempHead->next;
        }
        printf("=== END CUSTOMER INFO ===\n");

    }
    pthread_exit(0);
}

void * consumer(void * arg)
{
    pthread_detach( pthread_self() );
    struct filePointer *data = (struct filePointer *)arg;
    struct Queue *queue = data->table;
    //fprintf(stderr, "-------------------------------about to go into the consumer in the thread %s\n", data->table->category);
    struct QueueNode *temp;
    struct bookOrder *bo;
    while(queue->length > 0){
        temp = (struct QueueNode*)dequeue(queue);
        bo = (struct bookOrder *)temp->data;

        struct customer *c = lookupCustomer(bo->customerID); 
        pthread_mutex_lock(&c->mutex);
        //fprintf(stderr, "cat: %s, title: %s, price: %f \n", bo->category, bo->title, bo->price);
        if(c == NULL)
            ;//fprintf(stderr, "------------------null\n");
        else {    
            //fprintf(stderr, "------------------not\n");
            //fprintf(stderr, "name: %s, balance: %f, cid: %s \n", c->name, c->balance, c->customerID);
            int success = 0;
            if(c->balance >= bo->price){
                c->balance -= bo->price;
                success = 1;
            }
            struct llNode *listHead;
            if(success){
                listHead = c->successHead;
            }
            else {
                listHead = c->failedHead;
            }
            listHead = globalLL;
            if(globalLL == NULL){
                struct llNode *tempNode = malloc(sizeof(struct llNode));
                tempNode->order = bo;
                tempNode->order->currBal = c->balance;
                tempNode->order->success = success;
                globalLL = tempNode;
                globalLL->next = NULL;
            }
            else { 
                while(listHead->next != NULL){
                    listHead = listHead->next;
                } 
                struct llNode *tempNode = malloc(sizeof(struct llNode));
                tempNode->order = bo;
                tempNode->order->currBal = c->balance;
                tempNode->order->success = success;
                listHead->next = tempNode; 
                tempNode->next = NULL;
            }
        }        
        pthread_mutex_unlock(&c->mutex);
    }
    consumerThreadCount++;
    return 0;
}

void * producer(void * arg)
{
    pthread_detach( pthread_self() );
    struct filePointer *fp = (struct filePointer *)arg;
    FILE *bookFile = fp->fp;
    char line[256];
    while(fgets(line, sizeof(line), bookFile)) {
        struct bookOrder *order = malloc(sizeof(struct bookOrder));
        struct QueueNode *node = malloc(sizeof(struct QueueNode));
        TokenizerT *tokenizer = TKCreate("|", line);

        //title
        char * tok = TKGetNextToken(tokenizer);
        order->title = tok;

        //price
        tok = TKGetNextToken(tokenizer);
        order->price = atof(tok);

        //customerID
        tok = TKGetNextToken(tokenizer);
        order->customerID = tok;

        //category
        tok = TKGetNextToken(tokenizer);
        order->category = tok;
        tok[strlen(tok)-1] = '\0';

        node->data = order;

        struct Queue *queue = lookupQueue(order->category);
        enqueue(queue, node);
    }
    producerEmpty = 1;
    return 0;
}


struct customer * makeCustomer(char *line)
{
    struct customer *c = malloc(sizeof(struct customer));
    TokenizerT *tokenizer = TKCreate("|", line);

    /*name*/
    char * token = TKGetNextToken(tokenizer);
    c->name = token;
    /*customerID*/
    token = TKGetNextToken(tokenizer);
    c->customerID = token;
    /*balance*/
    token = TKGetNextToken(tokenizer);
    c->balance= atof(token);
    /*address*/
    token = TKGetNextToken(tokenizer);
    c->address = token;
    /*state*/
    token = TKGetNextToken(tokenizer);
    c->state = token;
    /*areaCode*/
    token = TKGetNextToken(tokenizer);
    c->areaCode = token;

    c->successHead = NULL;
    c->failedHead = NULL;

    return c;
}

void addCustomer(struct customer *cInfo)
{
    HASH_ADD_KEYPTR(hh, customersHashTable, cInfo->customerID, strlen(cInfo->customerID), cInfo);  /* Arguments: Hash Table, key, value*/
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


