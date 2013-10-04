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
    a = SLCreate(compareStrings);
    SLInsert(a, "aaa");
    SLInsert(a, "ccc");
    SLInsert(a, "ddd");

    SortedListIteratorPtr iter = SLCreateIterator(a);
    SortedListIteratorPtr iter1 = SLCreateIterator(a);
    SortedListIteratorPtr iter2 = SLCreateIterator(a);

    void* ab = SLNextItem(iter);
    printf("--------%s--------", (char*)ab);

    ab = SLNextItem(iter1);
    printf("--------%s--------", (char*)ab);

    ab = SLNextItem(iter2);
    printf("--------%s--------", (char*)ab);
    printf("\n");

    SLInsert(a, "bbb");


    ab = SLNextItem(iter);
    printf("--------%s--------", (char*)ab);


    ab = SLNextItem(iter1);
    printf("--------%s--------", (char*)ab);

    ab = SLNextItem(iter2);
    printf("--------%s--------", (char*)ab);

    printf("\n");

    ab = SLNextItem(iter);
    printf("--------%s--------", (char*)ab);


    ab = SLNextItem(iter1);
    printf("--------%s--------", (char*)ab);

    ab = SLNextItem(iter2);
    printf("--------%s--------", (char*)ab);

    printf("\n");

    ab = SLNextItem(iter);
    printf("--------%s--------", (char*)ab);


    ab = SLNextItem(iter1);
    printf("--------%s--------", (char*)ab);

    ab = SLNextItem(iter2);
    printf("--------%s--------", (char*)ab);


}
