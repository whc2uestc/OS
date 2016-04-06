 /****************************************************
  * File: scan.c
  * A compiler for the C-Minus language
  *
  * MUST CS106 2015 Fall
  * Programming designed by the teacher: Liang, Zhiyao
  * Created on: Oct 14, 2015
  ****************************************************/

#include "util.h"
#include "scan.h"



/* By default, it is true. Print the token list after scanner */
Bool SCANNER_TRACE = TRUE;

Bool SCAN_DEBUG = FALSE; /* when TRUE, print out debugging information of the DFA designed as the scanner */

const char * token_type_to_string(TokenType type){
	switch(type){
	case IF: return "if";  break;
	case ELSE: return "else";  break;
	case WHILE: return "while";  break;
	case INT:   return "int";  break;
	case VOID:  return "void";  break;
	case RETURN: return "return";  break;
	case DO: return "do";  break;
	case FOR: return "for";  break;
	case ASN: return("="); break;
	case EQ: return("=="); break;
	case NEQ: return("!="); break;
	case LT: return("<"); break;
	case LTE: return("<="); break;
	case GT: return(">"); break;
	case GTE: return(">="); break;
	case LPAR: return("("); break;
	case RPAR: return(")"); break;
	case LBR: return("["); break;
	case RBR: return("]"); break;
	case LCUR: return("{"); break;
	case RCUR: return("}"); break;
	case SEMI: return(";"); break;
	case COMMA: return(","); break;
	case PLUS: return("+"); break;
	case MINUS: return("-"); break;
	case TIMES: return("*"); break;
	case OVER: return("/"); break;
	case NUM: return("NUM"); break;
	case ID:  return("ID"); break;
	case END: return("END"); break;
	default: /* should never happen */
		printf("Unknown token type: %d", type);
		return("ERROR");
		break;
	}
 }


static void free_token_node( TokenNode * nd){
	if(nd != NULL){
		if(nd->token != NULL)
			free((void *)(nd->token->string)); /* the token's string should be created by malloc() */
		free((void *)(nd->token));
		free(nd);
	}
	if(SCAN_DEBUG == TRUE)
		puts("One token is freed");
}

/* Free the token List (all of the token nodes) described by tklist.
 * The TokenList struct pointed by tklist is not freed, and it is changed to describe an empty list */
void free_token_list(TokenList * tklist){
	TokenNode  * this, *next;
	if(tklist==NULL)
		return  ;
	this = tklist->head;
	while(this !=NULL){
		next = this->next;
		free_token_node(this);
		this = next;
	}
	tklist->head = NULL;
	tklist->tail = NULL;
	return ;
}

void print_token(const Token * tkp){
	if(tkp->type == ID || tkp->type == NUM)
		printf("%s: %s", token_type_to_string(tkp->type), tkp->string);
	else
		printf("%s", token_type_to_string(tkp->type));
}

/*Print the tokens of the nodes one by one, from head to tail.*/
 void print_token_list(TokenList * lis){
	 /* !!!!!!!!!!! provide the missing code here !!!!!!!!!!!!!!!!!*/
	 TokenNode *cur = lis->head;
	 if(NULL == cur)
		 return ;
	 while(cur!=lis->tail){
		 printf("%d: ",cur->lineNum);
		 print_token(cur->token);
		 if(cur->token->string)
			printf("%s\n",cur->token->string);
		 else
			printf("\n");
		 cur = cur->next;
	 }
	 printf("%d: ",cur->lineNum);
	 print_token(cur->token);
	 if(cur->token->string)
		printf("%s\n",cur->token->string);
	 else
		printf("\n");
 }

