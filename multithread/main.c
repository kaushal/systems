#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "uthash.h"

struct customer {
    char * name;
    int customerID;         /* key */
    double balance;
    char * address;
    char * state;
    char * areaCode;
    UT_hash_handle hh;
};

struct sharedData {
    int         isopen;
    unsigned int    front;
    unsigned int    count;
    unsigned int    bufsize;
    pthread_mutex_t mutex;
    pthread_cond_t  dataAvailable;
    pthread_cond_t  spaceAvailable;
};

struct bookOrder {
    char * title;
    double price;
    int customerID;
    char * category;
};

/*Global Hash Table*/
struct customer *customersHashTable = NULL;

/*Method Declarations*/
struct customer * makeCustomer(char *line);
void addCustomer(struct customer *cInfo);
char ** processCategories(char * categories);

int main(int argc, char * argv[])
{
    /*Argument check */
    if(argc != 4) {
        printf("USAGE: The first argument is the name of the database input file\n");
        printf("The second is the name of the book order input file.\n");
        printf("The third is the list of category names separated by blanks in a single "
                "or double quoted string.\n");
        return 0;
    }

    /*Read and check files*/
    FILE *dbFile = fopen(argv[1], "r");
    FILE *bookFile = fopen(argv[2], "r");
    if( dbFile == NULL || bookFile == NULL) {
        printf("Error reading file\n");
        return 1;
    }

    char line[256];
    /*Create customers hash table*/
    while(fgets(line, sizeof(line), dbFile)) {
       struct customer *newCustomer =  makeCustomer(line);
       printf("newCustomer: %s\n", newCustomer->name);
       addCustomer(newCustomer);
    }

    /* Process Categories */
    char * allCategories = argv[3];
    char ** categoriesArray = processCategories(allCategories);

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
    struct customer *c;
    c = malloc(sizeof(struct customer));
    c->customerID = cInfo->customerID;
    HASH_ADD_INT(customersHashTable, customerID, c );  /* Arguments: Hash Table, key, value*/
}

char ** processCategories(char * categories)
{
    int categoryCount = 0;

    char * token = strtok(categories, " ");
    if(token) categoryCount++;
    while((token = strtok(NULL, " ")) != NULL) {
        categoryCount++;
    }
    char **categoriesArray[categoryCount];
    token = strtok(categories, " ");
    if(token) categoryCount++;
    while((token = strtok(NULL, " ")) != NULL) {
        printf("working\n");
    }
    return categoriesArray;
}
