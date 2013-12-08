PA6 Readme:
For this assignment, we didn't use the given malloc algorithm. Instead we decided to write one ourselves that handles all of the different cases.

First we allocate a 5000 member array on the stack.

There are two methods that we wrote, char *myMalloc(size_t bits), and void free(char* ptr)
Both of these methods can be called in any order and we will do the given operations on them. 

Algorithm:
There are two data structures that we use to perform the malloc and free operations. There is an array of size 5000, and a linked list of memNodes. MemNodes contain their starting index in the array, their size, and a pointer to the next one. 
If you iterate through all existing memNodes, you will get to the last index that is current Malloc'd


myMalloc()
We only ever have free space at the end of the array, so we go through the linked list to determine the position of the free space, and then we see if there is enough space at the end to perform the new Malloc operation. If so, we make a new memNode, and attach it on to the end of the list, and return a pointer to the beginning of the newly allocated memory. This is similar to the 'best fit' algorithm.

myFree()
First we look in the linked list to see if we can find the pointer to what needs to be freed. If we find it we go ahead and delete it, then shift the entire array over the amount that is required, and make sure to have free space at the end of the array only. This also runs in the worst case of O(n).

Fragmentation:
we have no fragmentation problem because we have the sum total of the amount of free space we allocate at the end of the array, so this will never be an issue.

Saturation:
In the case of saturation, NULL is returned back from the myMalloc function, and an error is printed to the console.

