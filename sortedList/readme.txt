System's programming 214

The efficiency of our program is O(n*k) where n is the number of items in the list and k is the number of iterators. We go through each item in the list, and have a linear time insertion. We build a sorted linked list, each time we are asked to insert a node. Then iterators are changed as needed.

The problems that we ran into were related to the iterators while removing/adding nodes.



