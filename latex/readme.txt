Readme:

Group:
Stew Smith
Kaushal Parikh


1) We take the delimiters, and the input string, and parse it for special characters. 
2) Then we go through the input string character by character, comparing them to the tokens. 
3) We store each of the tokens in a linked list
4) We iterate through the linked list and print out each individual token, making sure to replace special characters with their hex values.
5) We free all of the memory that our program allocates.


The struggles we ran into were dealing with pointers, special characters, and edge cases regarding delimiter locations. 
We also had trouble choosing a data structure, and ended up chosing a linked list.

Our program runs in time O(n*m) where n is the size of the input and m is the size of the delimiters.
