System's programming 214

The efficiency of our program is O(n*k) where n is the number of items in the list and k is the number of iterators. We go through each item in the list, and have a linear time insertion. We build a sorted linked list, each time we are asked to insert a node. Then iterators are changed as needed.

The problems that we ran into were related to the iterators while removing/adding nodes.

We added a few structs to help us with this solution. We have an IterNode struct that has a linked list of iterators. This is to help us destroy all of the memory that we allocate. 

In slinsert, we deal with three cases. 
1) When the insertion has to happen at the head
2) when the insertion is in the middle
3) When the insertion is at the end

Remove works in a similar way to insert.


