#include <stdlib.h>
#include "sorted-list.h"


/*
 * SLCreate creates a new, empty sorted list.  The caller must provide
 * a comparator function that can be used to order objects that will be
 * kept in the list.
 *
 * If the function succeeds, it returns a (non-NULL) SortedListT object.
 * Else, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListPtr SLCreate(CompareFuncT cf)
{
  SortedListPtr newList = malloc(sizeof(SortedListPtr));
  newList->head = NULL;
  newList->comp = cf;
  newList->iterHead = NULL;
  return newList;
}

/*
 * SLDestroy destroys a list, freeing all dynamically allocated memory.
 *
 * You need to fill in this function as part of your implementation.
 */
void SLDestroy(SortedListPtr list)
{
  if(list == NULL || list->head == NULL)
    return;

  IterNode *temp = list->iterHead;
  while(temp->next != NULL) {
    free(temp);
  }

  Node * current = list->head;
  Node * prev = NULL;

  while(current->next != NULL) {
    prev = current;
    current = current->next;
    free(prev);
  }
  free(list);
}

/*
 * SLInsert inserts a given object into a sorted list, maintaining sorted
 * order of all objects in the list.  If the new object is equal to a subset
 * of existing objects in the list, then the subset can be kept in any
 * order.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLInsert(SortedListPtr list, void *newObj)
{
  if(list->head == NULL) {
    Node *newNode = malloc(sizeof(Node));
    newNode->data =  newObj;
    list->head = newNode;
    return 1;
  }
  else {
    Node *currentNode = list->head;
    Node *nextNode = NULL;

    //newobj is less than head of list
    int currentResult = list->comp(newObj, currentNode->data);
    int nextResult;
    if(currentResult >= 0) {
        Node *newNode = malloc(sizeof(Node));
        newNode->data = newObj;

        newNode->next = currentNode;
        list->head = newNode;
        return 1;
    }
    //newobj is in the middle of the list
    while(currentNode->next != NULL) {
      nextNode = currentNode->next;
      currentResult = list->comp(newObj,currentNode->data);
      nextResult = list->comp(newObj ,nextNode->data);
      if(currentResult <= 0 && nextResult > 0) {
        Node *newNode = malloc(sizeof(Node));
        newNode->data =  newObj;
        newNode->next = nextNode;
        currentNode->next = newNode;
        return 1;
      }
      currentNode = currentNode->next;
    }
    Node *newNode = malloc(sizeof(Node));
    newNode->data = newObj;
    currentNode->next =  newNode;
    return 1;
  }
  return 0;
}


/*
 * SLRemove removes a given object from a sorted list.  Sorted ordering
 * should be maintained.
 *
 * If the function succeeds, it returns 1.  Else, it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

int SLRemove(SortedListPtr list, void *target)
{
  if(list == NULL || list->head == NULL) {
    return 0;
  }

  Node *pointer = list->head;
  Node *prev = NULL;
  if(list->head->next != NULL && list->head->data == target){
      free(list->head);
  }
  //target is the head of the list
  if(list->head->data == target) {
    prev = list->head->next;
    list->head->data = list->head->next->data;
    list->head->next = list->head->next->next;

    free(prev);
    return 1;
  }
  //target is in the middle of the list
  while(pointer->next != NULL) {
    if(pointer->data == target) {
      prev = list->head->next;
      list->head->data = list->head->next->data;
      list->head->next = list->head->next->next;
      free(prev);
      return 1;
    }
    prev = pointer;
    pointer = pointer->next;
  }
  //target is the last in the list
  if(pointer->data == target){
    prev->next = NULL;
    free(pointer);
  }
  return 0;
}

/*
 * SLCreateIterator creates an iterator object that will allow the caller
 * to "walk" through the list from beginning to the end using SLNextItem.
 *
 * If the function succeeds, it returns a non-NULL SortedListIterT object.
 * Else, it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
    SortedListIteratorPtr iter = malloc(sizeof(struct SortedListIterator));
    if(list->iterHead == NULL) {
      list->iterHead = malloc(sizeof(IterNode));
      list->iterHead->iterator = iter;
    }
    else {
      IterNode *tempNode = malloc(sizeof(IterNode));
      tempNode->iterator = iter;
      tempNode->next = list->iterHead;
      list->iterHead = tempNode;
    }
    iter->node = list->head;
    return iter;
}


/*
 * SLDestroyIterator destroys an iterator object that was created using
 * SLCreateIterator().  Note that this function should destroy the
 * iterator but should NOT affectt the original list used to create
 * the iterator in any way.
 *
 * You need to fill in this function as part of your implementation.
 */

void SLDestroyIterator(SortedListIteratorPtr iter)
{
  free(iter);
}


/*
 * SLNextItem returns the next object in the list encapsulated by the
 * given iterator.  It should return a NULL when the end of the list
 * has been reached.
 *
 * One complication you MUST consider/address is what happens if a
 * sorted list encapsulated within an iterator is modified while that
 * iterator is active.  For example, what if an iterator is "pointing"
 * to some object in the list as the next one to be returned but that
 * object is removed from the list using SLRemove() before SLNextItem()
 * is called.
 *
 * You need to fill in this function as part of your implementation.
 */

void *SLNextItem(SortedListIteratorPtr iter){
    if(iter->node == NULL){
        return NULL;
    }
    void * returnData = iter->node->data;
    iter->node = iter->node->next;

    return returnData;
}
