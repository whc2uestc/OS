 /****************************************************
  * File: scan.h
  * A compiler for the C-Minus language
  *
  * MUST CS106 2015 Fall
  * Programming designed by whc
  * Created on: Oct 14, 2015
  ****************************************************/

#include "util.h"

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXRESERVED = the number of reserved words/ keywords */
/* if else return int void while for do */
#define MAX_RESERVED 8

/* MAXTOKENLEN is the maximum size of a token */
#define MAX_TOKEN_LEN 40

/* Controlling toggle for printing detailed information of running the scanner */
extern Bool SCANNER_TRACE ;

extern Bool SCAN_DEBUG;

typedef enum
   {
    /* reserved words
     if else int return void while do for
    */
    IF,ELSE,INT,RETURN,VOID,WHILE,DO,FOR,
    /* multicharacter tokens */
    ID,NUM,
    /* special symbols for
       + - * / < <= > >= == != = ; , ( ) [ ] { }
    */
    PLUS, MINUS, TIMES, OVER, LT, LTE, GT, GTE, EQ, NEQ, ASN, SEMI, COMMA, LPAR, RPAR, LBR, RBR, LCUR, RCUR,
	/* book-keeping token, representing the EOS (end of stream) signal received at the end of the streamer */
	END
   } TokenType;


typedef struct{
  const char * string; /* A string, which is only needed for id  or number tokens. For other kind of tokens, it should be NULL*/
  TokenType type;
} Token;

// A node in a double linked list
typedef struct TkNd{
  Token* token;
  int lineNum;
  struct TkNd * prev;
  struct TkNd * next;
}TokenNode;

typedef struct {
	TokenNode * head;
	TokenNode * tail;
}TokenList;


/* Print the token list after scanning when it is TRUE */
extern Bool SCANNER_TRACE;

/* The token list is defined somewhere, and it will be used by parser and analyzer.  */
extern TokenList * theTokenList;

const char * token_type_to_string(TokenType);

void print_token(const Token * );

/*Print the tokens of the nodes one by one, from head to tail.*/
void print_token_list(TokenList *);

/* Free the token List (all of the token nodes) described by tklist.
 * The TokenList struct pointed by tklist is not freed, and it is changed to describe an empty list */
void free_token_list(TokenList *);

#endif

