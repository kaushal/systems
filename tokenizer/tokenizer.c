/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct Node_ {
  char * data;
  struct Node_ * next;
};

struct TokenizerT_ {
  struct Node_ * head;
  char * inputString;
  char * delims;
  int current;
};

typedef struct Node_ Node;
typedef struct TokenizerT_ TokenizerT;

char *TKGetNextToken(TokenizerT *tk);
int isInDelims(char * delims, char letter);
void replaceSpecial(char * character);

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts) {
  char *currToken;
  Node * newNode;
  Node * tempNode;
  TokenizerT *tokenStruct = (TokenizerT*)
    malloc(sizeof(TokenizerT));
  tokenStruct->current = 0;
  tokenStruct->delims = separators;
  tokenStruct->inputString = ts;
  tokenStruct->head = NULL;
  while((currToken = TKGetNextToken(tokenStruct)) != 0) {
    newNode = malloc(sizeof(Node));
    newNode->data = currToken;
    newNode->next = NULL;
    tempNode = tokenStruct->head;
    if(tempNode == NULL){
        tokenStruct->head = newNode;
    }
    else{
        while(tempNode->next != NULL){
            tempNode = tempNode->next;
        }
        tempNode->next=newNode;
    }
    printf("%s\n", currToken);
  }

  return tokenStruct;
  //TODO function must return NULL otherwise
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
  free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

//TODO: string ends with delim
//TODO: special characters
char *TKGetNextToken(TokenizerT *tk) {
  int tokenStart = tk->current;
  int current= tokenStart;
  if(tk->inputString[current] == '\0'){
      return 0;
  }
  //go past everything in delims
  if(isInDelims(tk->delims, tk->inputString[current])){
    while(isInDelims(tk->delims, tk->inputString[current])){
        if (tk->inputString[current] == '\0'){
            return 0;
        }
      current++;
    }
  }
  tokenStart = current;

  //capture everything not in delims
  while(!isInDelims(tk->delims, tk->inputString[current])) {
    current++;
  }
  if(current > tokenStart) {
    tk->current = current;
    const char* from = tk->inputString;
    char *to = (char*) malloc(current-tokenStart + 1);
    char * temp2 = strncpy(to, from + tokenStart, current - tokenStart + 1);
    return temp2;
  }
  else {
    return 0;
  }
}

int isInDelims(char * delims, char letter)
{
  int i = 0;
  for(i = 0; i < strlen(delims); i++) {
    if(delims[i] == letter || letter == '\0') {
      return 1;
    }
  }
  return 0;
}

char * processTokens(char * unprocessed)
{
  char * processed = (char*) malloc(sizeof(unprocessed));
  strcpy(processed, unprocessed);
  char * currentChar = processed;
  int slashCount = 0;

  //  h\nw

  while(*currentChar != '\0'){
    if(*currentChar == '\\'){
      slashCount++;
    }
    currentChar++;
  }

  currentChar = processed;

  while(*currentChar != '\0') {
    if(*currentChar == '\\') {
      replaceSpecial(++currentChar);
    }
    currentChar++;
  }
  return processed;
}

void replaceSpecial(char * character)
{
  switch(*character) {
    case 'n':
      *character = '\n';
      break;
    case 't':
      *character = '\t';
      break;
    case 'v':
      *character = '\v';
      break;
    case 'b':
      *character = '\b';
      break;
    case 'r':
      *character = '\r';
      break;
    case 'f':
      *character = '\f';
      break;
    case '\\':
      *character = '\\';
      break;
    case 'a':
      *character = '\a';
      break;
    case '"':
      *character = '"';
      break;
  }
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv)
{
  //Program needs three arguments to run
  if(argc != 3) {
    printf("Need two arguments.\n");
    return 1;
  }

  //Arguments must not be empty
  if(!strlen(argv[1]) || !strlen(argv[2])) {
    printf("Empty argument detected.\n");
    return 1;
  }

  char *delims =  argv[1];
  char *tokens =  argv[2];

  char * processedTokens = malloc(sizeof(tokens));
  processedTokens = processTokens(tokens);

  TokenizerT *tk = TKCreate(delims, tokens);
  TKDestroy(tk);

  return 0;
};
