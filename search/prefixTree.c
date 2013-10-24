/* From http://kposkaitis.net/blog/2013/03/09/prefix-tree-implementation-in-c/ */

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "sortedList.h"

typedef enum {false, true} bool;


typedef struct TrieNode
{
  char character;
  struct TrieNode *parent;
  struct TrieNode **children;
  bool is_word;
  struct FileNode *fileHead;
} TrieNode;

struct TrieNode *create_trienode(char c, struct TrieNode *parent)
{
  struct TrieNode *node = malloc(sizeof(struct TrieNode));
  node->character = c;
  node->parent = parent;
  node->children = malloc(26*sizeof(struct TrieNode));
  node->is_word=false;
  return node;
}



void destroy_trienode(struct TrieNode *node)
{
  int i;

  if(node==NULL)
  {
      return;
  }

  for(i=0; i<26; i++)
  {
      destroy_trienode(node->children[i]);
  }

  free(node->children);
  free(node);
  return;
}

struct TrieNode *create_tree(FILE *file)
{
  struct TrieNode *root = create_trienode(' ', NULL);
  struct TrieNode *ptr = root;
  int character;
  int converted;
  int buffer;

  /*This handles if file does not end with a newline*/
  character = fgetc(file);
  buffer = fgetc(file);

  while(character != EOF)
  {
      character = tolower(character);

      if(isalpha(character))
      {
          converted = character - 97;
          if(ptr->children[converted] == NULL)
          {
              ptr->children[converted] = create_trienode(character, ptr);
          }
          ptr = ptr->children[converted];
      }
      if(ptr != root && (!isalpha(character) || buffer == EOF))
      {
          ptr->is_word = true;
          ptr = root;
      }

      character = buffer;
      buffer = fgetc(file);
  }

  return root;
}

struct FileNode * createFileNode(char *filename)
{
    struct FileNode *node = malloc(sizeof(struct FileNode));
    node->filename = filename;
    node->nextFile = NULL;
    node->count = 1;

    return node;
}

void insertFileNode(TrieNode *ptr, char *filename)
{
    if(ptr->fileHead == NULL) {
        ptr->fileHead = createFileNode(filename);
        return;
    }

    struct FileNode *filePtr = ptr->fileHead;
    struct FileNode *prev = ptr->fileHead;
    while(filePtr != NULL) {
        if(strcmp(filePtr->filename, filename) == 0) {
            filePtr->count++;
            //Delete and reinsert
        }
    }
    //if you haven't inserted yet, then make a new node, and call insert

    return;
}

struct TrieNode *insertWord(TrieNode *root, char *word, char *filename)
{
    int wordLen = strlen(word);
    struct TrieNode *ptr = root;
    int i = 0;
    char character;
    for(i = 0; i < wordLen; i++) {
        character = word[i]-97;
        if(ptr->children[character] == NULL) {
            ptr->children[character] = create_trienode(word[i], ptr);
        }
        ptr = ptr->children[character];
    }
    ptr->is_word = true;
    //insertFileNode(ptr, filename);
}



int main(int argc, char * argv[])
{
    TrieNode
    FILE * index = fopen(argv[1], "w");
    TrieNode * root = create_tree(index);
}

