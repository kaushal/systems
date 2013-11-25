#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

struct QueueNode {
    void * data;
    struct QueueNode *next;
};

struct Queue {
    char * category; /* key */
    int isOpen;
    int length;
    struct QueueNode *head;
    struct QueueNode *tail;
    pthread_mutex_t lock;
    UT_hash_handle hh;
};

struct Queue *makeQueue();
void enqueue(struct Queue *queue, void *category);
void *dequeue(struct Queue *queue);
void destroyQueue(struct Queue *queue);

#endif
