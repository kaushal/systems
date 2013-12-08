#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define MEM_SIZE 5000

struct memNode {
    int startPos;
    int size;
    struct memNode *next;
};

char *mem[MEM_SIZE]; //5000 long char*, that represents all of the memory
struct memNode *masterList; //list that maps out the char*

char *myMalloc(size_t bits){
    struct memNode *temp = masterList;
    //if the memory is empty, nothing malloc'd yet, or just freed
    if(masterList == NULL){
        struct memNode *newNode = malloc(sizeof(struct memNode));
        newNode->startPos = 0;
        if(bits > MEM_SIZE) {
            printf("Trying to allocate more than memory size.\n");
            return NULL;
        }
        newNode->size = bits;
        masterList = newNode;
        newNode->next = NULL;
        return mem[0];
    }
    //if the memory isn't empty, go to the end and try to allocate the right amount of space
    while(temp->next != NULL){
        temp = temp->next;
    }
    //Requested block fits in memory
    if(MEM_SIZE - (temp->startPos + temp->size) >= bits){
        struct memNode *newNode = malloc(sizeof(struct memNode));
        newNode->startPos = temp->startPos + temp->size;
        newNode->size = bits;
        temp->next = newNode;
        newNode->next = NULL;
        return mem[newNode->startPos];
    }
    else {
        printf("Trying to allocate more memory than possible.\n");
        return NULL;
    }
}

void myFree(void *data){
    int startPos = -1, i, tempPos;
    struct memNode *tempNode, *currNode, *previousNode;

    //finds the starting position in mem
    for(i = 0; i < MEM_SIZE; i++){
        if(mem[i] == data){
            startPos = i;
            break;
        }
    }

    //detects whether or not the starting position was found
    if(startPos == -1){
        //trying to free something that doesn't exist
        printf("Trying to free something not malloc'd\n");
        return;
    }
    tempPos = startPos;
    tempNode = masterList;

    //isolates the block of memory in the masterList
    if(tempNode != NULL){
        if(tempNode->startPos == startPos)
            currNode = tempNode;
        while(tempNode->next != NULL){
            if(tempNode->startPos == startPos){
                currNode = tempNode;
                break;
            }
            tempNode = tempNode->next;
        }
    }
    else {
        printf("Trying to free something not malloc'd\n");
        return;
    }

    //shifts all of the memory over by the block size that we just found
    for(i = currNode->startPos + currNode->size; i < MEM_SIZE; i++){
        mem[tempPos] = mem[i];
        tempPos++;
    }

    tempNode = masterList;

    //moves all the startPos values over by the currNode->size and gets rid of the one we want to
    if(tempNode->startPos == currNode->startPos)
        masterList = masterList->next;
    if(tempNode != NULL){
        while(tempNode->next != NULL){
            if(tempNode->startPos == currNode->startPos)
                previousNode->next = tempNode->next;
            if(tempNode->startPos > currNode->startPos + currNode->size){
                tempNode->startPos -= currNode->size;
            }
            previousNode = tempNode;
            tempNode = tempNode->next;
        }
    }
}

int main(int argc, char* argv[]) {
    masterList = NULL;

    if(argc == 2) {

        /*tests on malloc*/
        if(strcmp("1", argv[1]) == 0){
            char *p = myMalloc(4000);
            char *p2 = myMalloc(1000);
            myFree(p);
            myFree(p2);
        }

        else if(strcmp("2", argv[1]) == 0){
            char *p = myMalloc(4000);
            char *p2 = myMalloc(1001);
            myFree(p);
            myFree(p2);
        }

        /*tests on free*/
        else if(strcmp("3", argv[1]) == 0){
            int *x;
            myFree(x);
        }

        else if(strcmp("4", argv[1]) == 0){
            char *p = myMalloc(4000);
            myFree(p + 100);
        }

        else if(strcmp("5", argv[1]) == 0){
            char *p = myMalloc(4000);
            myFree(p);
            myFree(p);
        }

        else if(strcmp("6", argv[1]) == 0){
            char *p;
            p = myMalloc(4000);
            myFree(p);
            p = myMalloc(4000);
            myFree(p);
        }

        else {
            printf("Please enter a number [1-4], to test this program.\n");
        }
    }
    else {
            printf("Please enter 2 arguments './myMalloc <[1-4]>'");
        }



    return 0;
}
