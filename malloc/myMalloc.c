#include<stdlib.h>
#include<stdio.h>

struct memNode {
    int startPos;
    int size;
    struct memNode *next;
};

char *mem; //5000 long char*, that represents all of the memory
struct memNode *masterList; //list that maps out the char*

char *myMalloc(int bits){

}

void myFree(char *data){
    int startPos = -1, i, tempPos;
    struct memNode *tempNode, *currNode, *previousNode;

    //finds the starting position in mem
    for(i = 0; i < 5000; i++){
        if(mem[i] == *data){
            startPos = i;
        }
    }

    //detects whether or not the starting position was found
    if(startPos == -1){
        //trying to free something that doesn't exist
        return;
    }
    tempPos = startPos;
    tempNode = masterList;

    //isolates the block of memory in the masterList
    while(tempNode->next != NULL){
        if(tempNode->startPos == startPos){
            currNode = tempNode;
            break;
        }
        tempNode = tempNode->next;
    }

    //shifts all of the memory over by the block size that we just found
    for(i = currNode->startPos + currNode->size; i < 5000; i++){
        mem[tempPos] = mem[i];
        tempPos++;
    }

    tempNode = masterList;

    //moves all the startPos values over by the currNode->size and gets rid of the one we want to
    if(tempNode->startPos == currNode->startPos)
        tempNode = tempNode->next;
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

int main() {
    mem = (char *)malloc(5000);
    int i = 0;
    for(i = 0; i < 5000; i++)
        mem[i] = 0;

    fprintf(stderr, "%d\n", mem[500]);

    return 0;
}
