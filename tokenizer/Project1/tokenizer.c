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
    struct Node_ * head;
    char * inputString;
    char * delims;
    int current;
};
/*
 * Node type for the linked list
 * */

struct Node_ {
    char * data;
    struct Node_ * next;
};

typedef struct Node_ Node;
typedef struct TokenizerT_ TokenizerT;

char *TKGetNextToken(TokenizerT *tk);
int isInDelims(char * delims, char letter);
void replaceSpecial(char * destination, char * sourceChar);

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

    }
    return tokenStruct;
    //TODO function must return NULL otherwise
}

/*
 * printList iterates through the linked list
 * printing normal charcters when it encounters them
 * and printing hex values otherwise
  */

void printList(TokenizerT *tk){
    Node * newNode;
    newNode = tk->head;

    while(newNode != NULL){
        char * currentTokenString = newNode->data;
        while(*currentTokenString != '\0') {
            if(((int)*currentTokenString >= 7 &&  (int)*currentTokenString <=13) ||
                    (int)*currentTokenString == 92) {
                printf("[0x%.2x]", *currentTokenString);
            }
            else {
                printf("%c", *currentTokenString);
            }
            currentTokenString++;
        }
        printf("\n");
        newNode = newNode->next;
    }
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
    Node * pointerNode = tk->head;
    while(pointerNode != NULL) {
        Node * temp = pointerNode;
        pointerNode = pointerNode->next;
        free(temp);
    }
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
        char *to = (char*) malloc(current-tokenStart);
        char * temp2 = strncpy(to, from + tokenStart, current - tokenStart);
        return temp2;
    }
    else {
        return 0;
    }
}

/*
 * Return 0 or 1 depending on if the letter
 * is in the delimiters
 */

int isInDelims(char * delims, char letter)
{
    int i = 0;
    if(letter == '\0')
        return 1;
    for(i = 0; i < strlen(delims); i++) {
        if(delims[i] == letter) {
            return 1;
        }
    }
    return 0;
}

/*
 * A preporcessinfg function that searches for special
 * characters
 */

char * processTokens(char * unprocessed)
{
    char * processed = (char*) malloc(sizeof(unprocessed));
    char * head = processed;
    char * currentChar = unprocessed;

    while(*currentChar != '\0') {
        if(*currentChar == '\\' && *(currentChar + 1) != '\0') {
            replaceSpecial(processed, ++currentChar);
        }
        else{
            *processed = *currentChar;
        }
        currentChar++;
        processed++;
    }
    *processed = '\0';
    return head;
}

/* A big switch statement that builds a new string
 * with special charaters
 */

void replaceSpecial(char * destination, char * sourceChar)
{
    switch(*sourceChar) {
        case 'n':
            *destination = '\n';
            break;
        case 't':
            *destination = '\t';
            break;
        case 'v':
            *destination = '\v';
            break;
        case 'b':
            *destination = '\b';
            break;
        case 'r':
            *destination = '\r';
            break;
        case 'f':
            *destination = '\f';
            break;
        case '\\':
            *destination = '\\';
            break;
        case 'a':
            *destination = '\a';
            break;
        case '"':
            *destination = '"';
            break;
        default:
            *destination = *sourceChar;
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
    if(argc > 3) {
        printf("Too many => error.\n");
        return 1;
    }

    if(argc < 3) {
        printf("Too few => error.\n");
        return 1;
    }

    char *delims =  argv[1];
    char *tokens =  argv[2];

    char * processedTokens = malloc(sizeof(tokens));
    processedTokens = processTokens(tokens);
    char* processedDelims = malloc(sizeof(delims));

    processedDelims = processTokens(delims);
    TokenizerT *tk = TKCreate(processedDelims, processedTokens);
    printList(tk);
    TKDestroy(tk);

    printf("\n");
    return 0;
};
