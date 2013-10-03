/*
 * sorted-list.c
 */

#include<string.h>
#include"sorted-list.h"

int compareInts(void *p1, void *p2)
{
    int i1 = *(int*)p1;
    int i2 = *(int*)p2;

    return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
    double d1 = *(double*)p1;
    double d2 = *(double*)p2;

    return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
    char *s1 = p1;
    char *s2 = p2;

    return strcmp(s1, s2);
}

int main()
{
    SortedListPtr a;
    a = SLCreate(compareInts);
    int * one;
    int * two = 2;
    int * three = 3;
    SLInsert(a, one);
    SLInsert(a, two);
    SLInsert(a, three);
    SortedListIteratorPtr iter = SLCreateIterator(a);
    void* ab = SLNextItem(iter);
    printf("%s", (char*)ab);
    ab = SLNextItem(iter);
    printf("%s", ab);
    ab = SLNextItem(iter);
    printf("%s", ab);
    SLDestroy(a);
}
