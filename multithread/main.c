#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "uthash.h"

struct customerInfo {
    char * name;
    int customerID;
    double credit;
    char * address;
    char * state;
    char * areaCode;
};

struct customerHash {
    int customerID;                   /* key */
    struct customerInfo *customer;    /* value */
    UT_hash_handle hh;
};

/*Global Hash Table*/
struct customerHash *customersHashTable = NULL;

/*Method Declarations*/
struct customerInfo * makeCustomer(char *line);
void addCustomer(struct customerInfo *cInfo);

int main(int argc, char * argv[])
{
    if(argc != 4) {
        printf("USAGE: The first argument is the name of the database input file\n");
        printf("The second is the name of the book order input file.\n");
        printf("The third is the list of category names separated by blanks in a single "
                "or double quoted string.\n");
        return 0;
    }

    //Read and check files
    FILE *dbFile = fopen(argv[1], "r");
    FILE *bookFile = fopen(argv[2], "r");
    if( dbFile == NULL || bookFile == NULL) {
        printf("Error reading file\n");
        return 1;
    }

    char line[256];

    while(fgets(line, sizeof(line), dbFile)) {
       struct customerInfo *newCustomer =  makeCustomer(line);
       printf("newCustomer: %s\n", newCustomer->name);
       addCustomer(newCustomer);
    }

    return 0;
}

struct customerInfo * makeCustomer(char *line)
{
    struct customerInfo *c = malloc(sizeof(struct customerInfo));

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

void addCustomer(struct customerInfo *cInfo) {
    struct customerHash *c;
    c = malloc(sizeof(struct customerHash));
    c->customerID = cInfo->customerID;
    HASH_ADD_INT(customersHashTable, customerID, c );  /* Arguments: Hash Table, key, value*/
}
