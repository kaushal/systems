#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include "uthash.h"

struct QueueNode {
    void * data;
    struct QueueNode *next;
};

struct Queue {
    char * category; /* key */
    int length;
    struct QueueNode *head;
    struct QueueNode *tail;
    pthread_mutex_t mutex;
    pthread_cond_t dataAvailable;
    UT_hash_handle hh;
};

struct Queue *makeQueue();
void enqueue(struct Queue *queue, struct QueueNode *node);
void *dequeue(struct Queue *queue);
void destroyQueue(struct Queue *queue);

#endif
