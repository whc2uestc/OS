 /****************************************************
  * File: scanner_dfa.h
  * A compiler for the C-Minus language
  *
  * MUST CS106 2015 Fall
  * Programming designed by whc
  * Created on: Nov 2, 2015
  ****************************************************/

#ifndef _SCANNER_DFA_H_
#define _SCANNER_DFA_H_

#include "scan.h"
#include "s_automata.h"

/*  Some information of running a DFA for scanner. These information are needed and cannot be saved by a simple DFA
 * The info field of an automata will */
struct scanInfo{
	char * tokenString;  /* a character array, which stores the lexeme of each token that is  NUM nor ID/Keywords,
	     it will be a char array of size [MAX_TOKEN_LEN+1] with its last character as '\0' making it a string */
	int tokenStringIndex;  /* where to save a character into the token string */
	int lineNum ; /* the line number of the current token, starting from line 1 */
	TokenType tokenType; /* it will be set with a meaningful value before a token node is created */
	TokenList * tokenList; /* The result of scanning. The token list will be growing during a run of the dfa for scanner*/
};

typedef struct scanInfo ScanInfo;

/*
 * Creating a DFA specially designed to work as a scanner. The streamer of the DFA is provided by the parameter.
 * So, the scanner can accept input from different sources managed by different streamers.
 */
SAutomata * make_scanner_dfa(Streamer* streamer);

/* Run the automaton, and the token list is generated and returned.*/
TokenList * run_scanner_dfa(SAutomata * atp);

#endif
