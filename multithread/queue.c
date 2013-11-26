#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct Queue *makeQueue()
{
    struct Queue *queue = malloc(sizeof(struct Queue));
    queue->isOpen = 1;
    queue->category = NULL;
    queue->length = 0;
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&queue->mutex, 0);
    return queue;
}

void enqueue(struct Queue *queue, struct QueueNode *data)
{
    struct QueueNode *node = malloc(sizeof(struct QueueNode));
    node->data = data;
    node->next = NULL;

    /*Check for empty queue*/
    if(queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
        queue->length = 1;
        return;
    }
    queue->tail->next = node;
    queue->tail = node;
    queue->length++;
    return;
}

void *dequeue(struct Queue *queue)
{
    char * data = queue->head->data;
    struct QueueNode *head = queue->head;
    if(queue->length != 1) {
        queue->head = queue->head->next;
    }
    free(head);
    queue->length--;
    return data;
}

void destroyQueue(struct Queue *queue)
{
    while(queue->length !=0) {
        dequeue(queue);
    }
}
