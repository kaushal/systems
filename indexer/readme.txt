Readme:

Design
The main datastructure that we decided to use for this project is a hash table of words with a linked of files where the words occur, and their respective counts attached to them. 

To walk along the files, we use a c utility called ftw. It walks through the files, and writes the results to a file that we delete after the program is terminated. 

The run time of the program is O(n) where n is the number of words in all of the files.
Our hash table has a write time of O(n), and a read time of O(1)
