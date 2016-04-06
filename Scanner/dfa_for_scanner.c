 /****************************************************
  * File: dfa_for_scanner.c
  * A compiler for the C-Minus language
  * Partly inspired by the textbook
  *
  * MUST CS106 2015 Fall
  * Programming designed by whc
  * Created on: Nov 2, 2015
  ****************************************************/

#include "util.h"
#include "dfa_for_scanner.h"
#include "streamer_for_scanner.h"
#include "scan.h"

/* number of states in the dfa, not counting the invisible failure state */
#define STATE_NUM 10

/* number of rules in the rule set */
#define RULE_SIZE_LIMIT  45

/* NONC  is the "non-character",  which is an integer that is different from any possible characters that can appear in the input.
 * When NOC appears in a transition rule r of an the automata of a scanner, then only way that rule can be fired is that the condition() function
 * of the rule returns TRUE, since it is impossible for the character returned by get_current_char() to be the same as NOC.
 * Important:  NOC must be different from EOS, otherwise some trouble can make. Example can be easily found,
 * like when the source code ends improperly, the scanner can still consider as normal situation.
 * */
#define NONC   (-5)


/*labels of the states in the DFA
 * The order of labels will decide its corresponding state number. For example, the third label corresponds to state 2.
 * The order of the labels are not important, though, since in the make_rules() function a state is identified by its lable, not by number.
 * But when the DFA graph is made, we can make the order of the labels corresponds to the state number in the graph, as what is done here.*/
const char* labels[] =
{"START", "READ_OVER", "IN_COMMENT", "WAIT_FOR_OVER", "READ_LT", "READ_GT", "READ_EQ", "READ_AMP", "IN_NUM", "IN_ID"};

/* initialize some running time book-keeping information */
static void before(SAutomata * self);

/* this function is not used, but can possibly used by a rule to accept any current character*/
/*
static Bool yes(SAutomata * atp){
	return TRUE;
}
*/

static Bool no(SAutomata * atp){
	return FALSE;
}

static void nothing(SAutomata * atp){
	return;
}

/* initialize some running time book-keeping information */
static void before(SAutomata * self){
	ScanInfo * info = (ScanInfo *)self->info;
	if(self->info == NULL){
		self->info = info = (ScanInfo *) malloc(sizeof(ScanInfo));
		info->tokenList = (TokenList *) malloc(sizeof(TokenList));
	}
	else if(info->tokenList == NULL)
		info->tokenList = (TokenList *) malloc(sizeof(TokenList));
	else /* (info != NULL && info->tokenList != NULL) */
		  free_token_list(info->tokenList);
	info->tokenList->head = info->tokenList->tail = NULL; /* don't forget it, don't let the head or tail pointing to objects that are freed already. */
	//info->curentChar = NONC;
	info->lineNum = 1;
	//info->tokenString[0] = '\0';
	info->tokenString = (char *)malloc(sizeof(char)*(MAX_TOKEN_LEN+1));
	info->tokenStringIndex = 0;
	info->tokenType = END;
	/* now tokenList field should be created */
	info->tokenList->head = info->tokenList->tail = NULL;
}

/* update line number */
static void L(SAutomata * atp){
	ScanInfo * info = (ScanInfo *)atp->info;
	info->lineNum++;
}

/* the side effect functions */
/*U: to decrement the current position of the streamer, similar to ungetting the current char.  */
static void U(SAutomata * atp){
	if(SCAN_DEBUG == TRUE)
		puts("U is called.");
	atp->streamer->prev_position(atp->streamer);
}


/* R: to record the current char (into the token string)
 * <Parameter>  atp: a pointer to the automata that is running as a scanner.
 * <Computation>
 * 1) if tokenStringIndex (a field in the info field of atp) is larger then limit (the number of already recorded characters are equal or more than MAX_TOKEN_LEN),
 *        print out some error message;
 *        change the current state of atp to its failure state;
 *        end this function call of R(). .
 * 2) Record the current character of atp in its token string (a field in its info) at the index of tokenStringIndex.
 * 3) increment tokenStringIndex.
 * Optional:   if SCAN_DEBUG is TRUE, print out some message as you like.
 * */
static void R(SAutomata * atp){
 /* !!!!!!!!!!!! Please provide the missing code here  !!!!!!!!!!!!!!!!!!! */
	ScanInfo * info = (ScanInfo *)atp->info;
	if(SCAN_DEBUG == TRUE)
		puts("R is called.");
	if(info->tokenStringIndex > MAX_TOKEN_LEN){
		puts("tokenStringIndex is larger than limit.");
		atp->currentState = atp->failureState;
		return ;
	}
	info->tokenString[info->tokenStringIndex++]= atp->streamer->get_current_char(atp->streamer);
	//puts(info->tokenString);
}

/* Have to make listp a pointer to TokenList, instead of TokenList, in order to change some tokenList */
static void append_token_node(TokenList * listp, TokenNode * nodep){
	if(listp == NULL) /* undefined behavior if listp is NULL*/
		return;
	if(listp->head == NULL){
		listp->head = nodep;
		listp->tail = nodep;
	}
	else{  /* listp->tail is not NULL */
		listp->tail->next = nodep;
		nodep->prev = listp->tail;
		listp->tail = nodep;
	}
}


/*C: create a token node and append  it to the token list.
 * <Parameter>  atp: a pointer to the automata that is running as a scanner
 * <Assmption>: Before the function call, the correct token type and token string are already recorded in the "info" field of atp.
 * <Computation>
 *  1) Create a token (using malloc)
 *  2) Let the "type" field of this token is the "tokenType" (in the "info" of atp) of the atp.
 *  3) Set the "string" field of this token:
 *     - If the type is ID or NUM, the "string" of the token is a clone (call the string_clone() function) of the "tokenString"
 *         (in the "info" of atp) of the atp. Make sure that the clone ends with a NULL character '\0' to form a real string.
 *     - Otherwise, (token type is not ID nor NUM), the "string" field of the token is NULL.
 *  4) Clear the token string of atp by changing its "tokenStringIndex" to 0.
 *  5) Create a token node.
 *  6) Let the "token" field of this new token node to be the new token created above.
 *  7) Let the "prev" and "next" fields of the token node to be NULL.
 *     Explanation: When a pointer is not assigned with a value, they can be trouble-makers.
 *     Also, we need to let its "next" field to be NULL since the token node will be the tail of a list.
 *  8) Let "lineNum" field of the token node to be the "lineNum" of atp (in its "info").
 *  9) Append the token node to the "tokenList" of atp. So the new node becomes the tail of the token list.
 *  <Optional>:
 *  if SCAN_DEBUG is TRUE, print some information to track the running status as desired.
 *   */
static void C(SAutomata * atp){
	/* !!!!!!!!!!!! Please provide the missing code here  !!!!!!!!!!!!!!!!!!! */
	if(SCAN_DEBUG == TRUE)
		puts("C is called.");
	TokenNode *pNode = (TokenNode*)malloc(sizeof(TokenNode));
	ScanInfo * info = (ScanInfo *)atp->info;
	pNode->token = (Token*)malloc(sizeof(Token));
	pNode->token->type = info->tokenType;
	if(pNode->token->type==ID || pNode->token->type==NUM){
		info->tokenString[info->tokenStringIndex] = 0;
		pNode->token->string = string_clone(info->tokenString);
	}
	else{
		pNode->token->string = NULL;
	}
	pNode->prev = NULL;
	pNode->next = NULL;
	pNode->lineNum = info->lineNum;
	memset(info->tokenString,0,sizeof(info->tokenString));
	info->tokenStringIndex = 0;
	
	append_token_node(info->tokenList, pNode);
	/*pNode->prev = info->tokenList->tail;
	if(info->tokenList->tail==NULL){
		info->tokenList->tail = info->tokenList->head = pNode;
	}else{
		info->tokenList->tail->next = pNode;
		info->tokenList->tail = pNode;
	}*/
}


static void UC(SAutomata * atp){
	U(atp), C(atp);
}

static TokenType name_to_type(const char * name){
	if(strcmp(name, "if") == 0)
		return IF;
	if(strcmp(name, "else") == 0)
		return ELSE;
	if(strcmp(name, "int") == 0)
		return INT;
	if(strcmp(name, "return") == 0)
		return RETURN;
	if(strcmp(name, "void") == 0)
		return VOID;
	if(strcmp(name, "while") == 0)
		return WHILE;
	if(strcmp(name, "do") == 0)
		return DO;
	if(strcmp(name, "for") == 0)
		return FOR;
	return ID;
}

/*Now define all the condition and effect functions
 * c0_1 means the condition function of the transition from state 0 to state 1 (in the DFA graph)
 * e0_1 means the side_effect function of the transition from state 0 to state 1.
 * When there are more then 1 transition edges with the same from and to states, these function names are
 * appended with additional string for obvious distinguishing purpose. For example, 0_0_EOS vs. 0_0_space.
 * */

/*Condition functions are needed when the conditions that cannot be expressed by matching a specific character  */



Bool c0_0_space(SAutomata * atp){
	int c = atp->streamer->get_current_char(atp->streamer);
		return isspace(c) ? TRUE: FALSE;
}

void e0_0_space(SAutomata * atp){
	if(atp->streamer->get_current_char(atp->streamer) == '\n')
		L(atp);
}

/*
Bool c0_0_EOS( )
is not needed, since it can apply with the exact current_char value EOS.
*/

void e0_0_EOS(SAutomata * atp){
	ScanInfo * info = (ScanInfo *) atp->info;
	info->tokenType = END;   /* make sure that before calling C(atp), the tokenType field is set with the correct value. */
	C(atp);
}

/*conditions from state q0 to q0 */
Bool c0_0(SAutomata * atp){
	int c = atp->streamer->get_current_char(atp->streamer);
	ScanInfo * info = (ScanInfo *) atp->info;
	if(c=='+' || c=='-' || c=='*' || c=='(' || c==')' || c=='[' || c==']' || c=='{' || c=='}' || c==',' || c==';'){
		if(c=='+')
			info->tokenType = PLUS;
		else if(c=='-')
			info->tokenType = MINUS;
		else if(c=='*')
			info->tokenType = TIMES;
		else if(c=='(')
			info->tokenType = LPAR;
		else if(c==')')
			info->tokenType = RPAR;
		else if(c=='{')
			info->tokenType = LCUR;
		else if(c=='}')
			info->tokenType = RCUR;
		else if(c=='[')
			info->tokenType = LBR;
		else if(c==']')
			info->tokenType = RBR;
		else if(c==',')
			info->tokenType = COMMA;
		else if(c==';')
			info->tokenType = SEMI;
		
		return TRUE;
	}
	else
		return FALSE;
	//return ispunct(c) ? TRUE: FALSE;
}
void e0_0(SAutomata * atp){
	C(atp);
}
/* c0_1 e0_1 unnecessary */

/*conditions from state q1 to q0 */
static Bool c1_0(SAutomata * atp){
	return (atp->streamer->get_current_char(atp->streamer) != '*') ? TRUE: FALSE;
}

static void e1_0(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = OVER;
	UC(atp);
}

/* !!!!!!!!!!!!!!
 * Please provide the missing condition functions and effect functions here
 * !!!!!!!!!!!!!!*/
/*conditions from state q2 to q2 */
static Bool c2_2(SAutomata * atp){
	return (atp->streamer->get_current_char(atp->streamer) != '*') ? TRUE: FALSE;
}
/*conditions from state q3 to q2 */
static Bool c3_2(SAutomata * atp){
	return ((atp->streamer->get_current_char(atp->streamer) != '*') && (atp->streamer->get_current_char(atp->streamer) != '/')) ? TRUE: FALSE;
}

/*conditions from state q4 to q0 */
static Bool c4_0_UC(SAutomata * atp){
	return (atp->streamer->get_current_char(atp->streamer) != '=') ? TRUE: FALSE;
}

static void e4_0_UC(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = LT;
	UC(atp);
}
static void e4_0_C(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = LTE;   /* make sure that before calling C(atp), the tokenType field is set with the correct value. */
	C(atp);
}

/*conditions from state q6 to q0 */
static Bool c6_0_UC(SAutomata * atp){
	return (atp->streamer->get_current_char(atp->streamer) != '=') ? TRUE: FALSE;
}

static void e6_0_UC(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = ASN;
	UC(atp);
}
static void e6_0_C(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = EQ;   /* make sure that before calling C(atp), the tokenType field is set with the correct value. */
	C(atp);
}
/*conditions from state q7 to q0 */
static void e7_0(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = NEQ;   /* make sure that before calling C(atp), the tokenType field is set with the correct value. */
	C(atp);
}
/*conditions from state q5 to q0 */
static Bool c5_0_UC(SAutomata * atp){
	return (atp->streamer->get_current_char(atp->streamer) != '=') ? TRUE: FALSE;
}

static void e5_0_UC(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = GT;
	UC(atp);
}
static void e5_0_C(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = GTE;   /* make sure that before calling C(atp), the tokenType field is set with the correct value. */
	C(atp);
}
/*conditions from state q0 to q8 */
Bool c0_8(SAutomata * atp){
	int c = atp->streamer->get_current_char(atp->streamer);
		return isdigit(c) ? TRUE: FALSE;
}

void e0_8(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = NUM; 
	R(atp);
}

/*conditions from state q8 to q0 */
Bool c8_0(SAutomata * atp){
	int c = atp->streamer->get_current_char(atp->streamer);
		return isdigit(c) ? FALSE: TRUE;
}

void e8_0(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = NUM;
	UC(atp);
}
/*conditions from state q8 to q8 */
Bool c8_8(SAutomata * atp){
	int c = atp->streamer->get_current_char(atp->streamer);
		return isdigit(c) ? TRUE: FALSE;
}

void e8_8(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = NUM; 
	R(atp);
}


/*conditions from state q0 to q9 */
Bool c0_9(SAutomata * atp){
	int c = atp->streamer->get_current_char(atp->streamer);
		return isalpha(c) ? TRUE: FALSE;
}

void e0_9(SAutomata * atp){
	R(atp);
}

/*conditions from state q9 to q0 */
Bool c9_0(SAutomata * atp){
	int c = atp->streamer->get_current_char(atp->streamer);
		return isalpha(c) ? FALSE: TRUE;
}

void e9_0(SAutomata * atp){
	ScanInfo * info = (ScanInfo*) atp->info;
	info->tokenType = name_to_type(info->tokenString);
	UC(atp);
}
/*conditions from state q9 to q9 */
Bool c9_9(SAutomata * atp){
	int c = atp->streamer->get_current_char(atp->streamer);
		return isalpha(c) ? TRUE: FALSE;
}

void e9_9(SAutomata * atp){
	R(atp);
}
/* the failure statue is invisible*/
static int make_rules(SAutomata * atp){
	State start, read_over, in_comment, wait_for_over, read_gt, read_eq, read_amp, read_lt, in_num, in_id;

    /* atp has already labeled its states */
	start = l2s(atp, "START");
	read_over = l2s(atp,"READ_OVER");
	in_comment = l2s(atp, "IN_COMMENT");
	wait_for_over = l2s(atp, "WAIT_FOR_OVER");
	read_gt = l2s(atp, "READ_GT");
	read_eq = l2s(atp, "READ_EQ");
	read_amp = l2s(atp, "READ_AMP");
	read_lt = l2s(atp, "READ_LT");
	in_num = l2s(atp, "IN_NUM");
	in_id = l2s(atp, "IN_ID");

	atp->rules.size = 0;  /* initially, no rule is added into the set of rules.*/
	atp->rules.sizeLimit = RULE_SIZE_LIMIT;
	atp->rules.addr = (ARule *)malloc(sizeof(ARule) * atp->rules.sizeLimit);

   /* add the 28 rules one by one */
	add_rule(atp, start, start, NONC, c0_0_space, e0_0_space); /* 0_0_space */
	add_rule(atp, start, start, EOS, no, e0_0_EOS); /*0_0_EOS*/
	add_rule(atp, start, read_over, '/', no, nothing); /*0_1*/
	add_rule(atp, read_over, start, NONC, c1_0, e1_0); /*1_0*/

	/* !!!!!!!!!!!!!!
	 * Please add missing rules to the automaton here.  About 24 rules need to be added according to the DFA graph.
	 * !!!!!!!!!!!!!!
	 * */
	add_rule(atp, read_over, in_comment, '*', no, nothing); /*1_2*/
	add_rule(atp, in_comment, in_comment, NONC, c2_2, nothing); /*2_2*/
	add_rule(atp, in_comment, wait_for_over, '*', no, nothing); /*2_3*/
	add_rule(atp, wait_for_over, in_comment, NONC, c3_2, nothing); /*3_2*/
	add_rule(atp, wait_for_over, wait_for_over, '*', no, nothing); /*3_3*/
	add_rule(atp, wait_for_over, start, '/', no, nothing); /*3_0*/
	add_rule(atp, start, read_lt, '<', no, nothing); /*0_4*/
	add_rule(atp, read_lt, start, NONC, c4_0_UC, e4_0_UC); /*4_0_UC*/
	add_rule(atp, read_lt, start, '=', no, e4_0_C); /*4_0_C*/
	add_rule(atp, start, read_eq, '=', no, nothing); /*0_6*/
	add_rule(atp, read_eq, start, NONC, c6_0_UC, e6_0_UC); /*6_0_UC*/
	add_rule(atp, read_eq, start, '=', no, e6_0_C); /*6_0_C*/
	add_rule(atp, start, read_amp, '!', no, nothing); /*0_7*/
	add_rule(atp, read_amp, start, '=', no, e7_0); /*7_0*/
	add_rule(atp, start, read_gt, '>', no, nothing); /*0_5*/
	add_rule(atp, read_gt, start, NONC, c5_0_UC, e5_0_UC); /*5_0_UC*/
	add_rule(atp, read_gt, start, '=', no, e5_0_C); /*5_0_C*/
	add_rule(atp, start, in_num, NONC, c0_8, e0_8); /*0_8*/
	add_rule(atp, in_num, start, NONC, c8_0, e8_0); /*8_0*/
	add_rule(atp, in_num, in_num, NONC, c8_8, e8_8); /*8_8*/
	add_rule(atp, start, in_id, NONC, c0_9, e0_9); /*0_9*/
	add_rule(atp, in_id, start, NONC, c9_0, e9_0); /*9_0*/
	add_rule(atp, in_id, in_id, NONC, c9_9, e9_9); /*9_9*/
	add_rule(atp, start, start, NONC, c0_0, e0_0); /*0_0*/


	return atp->rules.size;
}


/*
 * Creating a DFA specially designed to work as a scanner. The streamer of the DFA is provided by the parameter.
 * So, the scanner can accept input from different sources managed by different streamers.
 */
SAutomata * make_scanner_dfa(Streamer* streamer){
	SAutomata * atp = (SAutomata *) malloc(sizeof(SAutomata));
	StateSet accepts;
	ARuleSet rules;
	ARule  r;
	unsigned int j;

	/* 1)   */
	atp->streamer = streamer;
	if(AT_DEBUG)
		puts("automata's streamer is made.");

	/* 2) */
	/* not including the failure state, the failure state is the same as the wrong state in the
	 * DFA graph */
	(*atp).stateNum = STATE_NUM;
	if(AT_DEBUG)
		puts("automata's stateNum is made.");

	/*  3) labels */
    atp->labels = (const char **) malloc(sizeof(const char *) * atp->stateNum);
    /*  (const char**)labels  is used to avoid a warning like
     * passing 'const char *[4]' to parameter of type 'char **'
     *  */
    set_labels(atp, (const char**)labels, atp->stateNum);
	if(AT_DEBUG)
		puts("automata's labels are made.");

	/* 4)  startState*/
	atp->startState = l2s(atp, "START");
	if(AT_DEBUG)
		puts("automata's startState is made.");

	/* 5) currentState,  no need to set now, but doing it does not matter */
	atp->currentState = atp->startState;
	if(AT_DEBUG)
		puts("automata's currentState is made.");

	/* 6) the special failureState. 0...stateNum-1 are the visible states on the DFA graph */
	atp->failureState = atp->stateNum;
	if(AT_DEBUG)
		puts("automata's failureState is made.");

	/* 7) acceptStates
	 * Only one accept state, which is the start state according to the updated DFA graph*/
	accepts.size = 1;
	accepts.sizeLimit = 1;
	accepts.addr = (State *)malloc(sizeof(State)*accepts.sizeLimit);
	accepts.addr[0] = l2s(atp, "START");
    atp->acceptStates = accepts;
	if(AT_DEBUG)
		puts("automata's accept states are made.");

    /* 8) rules */
    j = make_rules(atp);
	if(AT_DEBUG)
		printf("%d rules are added into the automata.\n", j);

    /* 9) info */
	atp->info = NULL; /* important, the info field should be handled or created by the before_running function */
	if(AT_DEBUG)
		printf("The info field is in the automata is processed.\n");

	/* 10) before_running */
	atp->before_running = before;
	if(AT_DEBUG)
		printf("The before_running method is added into the automata.\n");

	/* 11) after_running */
	atp->after_running = nothing;

	if(AT_DEBUG)
		printf("The after_running methods are added into the automata.\n");

	return atp;
}

/* Run the automaton, and the token list is generated and returned.*/
TokenList * run_scanner_dfa(SAutomata * atp){
	State result =  run_sautomata(atp);
	ScanInfo * info = (ScanInfo *) atp->info;
	if(is_in_accept(atp)!=TRUE)
		printf("Error:  The scanner found error at line %d\n", info->lineNum);
	return info->tokenList;
}
