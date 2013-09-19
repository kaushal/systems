/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */


struct TokenizerT_ {
  char ** brokenTokens;
  char * inputString;
  char * delims;
  int current;
};

typedef struct TokenizerT_ TokenizerT;

char *TKGetNextToken(TokenizerT *tk);
int isInDelims(char * delims, char letter);

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
  char *next;
  TokenizerT *tokenStruct = (TokenizerT*)
    malloc(sizeof(TokenizerT));
  tokenStruct->current = 0;
  tokenStruct->delims = separators;
  tokenStruct->inputString = ts;

  //TODO Maybe don't print these -- populate array instead
  while((next = TKGetNextToken(tokenStruct)) != 0) {
    printf("%s\n", next);
  }

  //TODO function must return Tokenizert
  return tokenStruct;
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

//TODO: two delims in a row
//TODO: special characters
char *TKGetNextToken(TokenizerT *tk) {
  int start = tk->current;
  int current= start;
  if(tk->inputString[current] == '\0'){
      return 0;
  }
  //go past everything in delims
  if(isInDelims(tk->delims, tk->inputString[current])){
    while(isInDelims(tk->delims, tk->inputString[current])){
      current++;
    }
  }
  start = current;

  //capture everything not in delims
  while(!isInDelims(tk->delims, tk->inputString[current])) {
    current++;
  }
  if(current > start) {
    tk->current = current;
    const char* from = tk->inputString;
    char *to = (char*) malloc(current-start);
    char * temp2 = strncpy(to, from + start, current - start);
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

  TokenizerT *tk = TKCreate(delims, tokens);
  TKDestroy(tk);

  return 0;
};
