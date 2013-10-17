#ifndef SORTEDLIST_H
#define SORTEDLIST_H
/*
 *sortedList.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * SortedList type.
 */

struct FileNode{
    char* fileName;
    struct * fileNode nextFile;
    int count;
} FileNode;

#endif

