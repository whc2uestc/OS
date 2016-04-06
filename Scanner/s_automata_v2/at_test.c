 /****************************************************
  * File: at_test.c
  * A compiler for the C-Minus language
  * Partly inspired by the textbook
  * "Compiler Construction: Principles and Practice"
  * Book author:  Kenneth C. Louden
  * MUST CS106 2015 Fall
  * Programming designed by the teacher: Liang, Zhiyao
  * Created on: Oct 27, 2015
  ****************************************************/

#include "util.h"
#include "s_automata.h"

Streamer * make_input_streamer(void);

/* show introduction of an automata */
void intro(void){
	puts("..............");
	printf("This program implements a DFA that accepts the language L such that\n");
	printf("The alphabet of L is {a, b}, \n");
	printf("and each string in L has an even number of a's, and an odd number of b's.\n");
	printf("L = {w | w in {a,b}*, and in w |a| is even, and |b| is odd}\n");
	puts("..............");
}

/*
Bool yes(char c){

Bool yes(int c){

*/
Bool yes(SAutomata * atp){
	return TRUE;
}

/*
Bool no(char  c)
Bool no(char  c) */
Bool no(SAutomata * atp){
	return FALSE;
}

void nothing(SAutomata * atp){
	return;
}

/* Assuming the states and labels of atp are already settled.
 * Add rules into atp, and return the number of rules that are added.
 * */
unsigned int make_rules(SAutomata * atp){
	State EaEb, EaOb, OaEb, OaOb;
	atp->rules.size = 0;  /* initially, now rule is addded into the set of rules.*/
	atp->rules.sizeLimit = 8; /* It is known that the automata has 8 states */
	atp->rules.addr = (ARule *)malloc(sizeof(ARule) * atp->rules.sizeLimit);


	/* there are 8 transition rules */
    EaEb = l2s(atp, "EaEb");
    EaOb = l2s(atp, "EaOb");
    OaEb = l2s(atp, "OaEb");
    OaOb = l2s(atp, "OaOb");
	if(atp->failureState == EaEb || atp->failureState == EaOb ||
			atp->failureState == OaEb || atp->failureState == OaOb  ){
		puts("Wrong state name found in make_rules().");
		exit(1);
	}

	/* unsigned int add_rule(SAutomata *atp, State from, State to, char c, Bool (*condition)(char), void (*side_effect)(SAutomata*));
	 * When condition is no, then the input character must be the same as c to trigger the rule.
	 * */
	add_rule(atp, EaEb, OaEb, 'a', no, nothing);
	add_rule(atp, EaEb, EaOb, 'b', no, nothing);
	add_rule(atp, EaOb, OaOb, 'a', no, nothing);
	add_rule(atp, EaOb, EaEb, 'b', no, nothing);
	add_rule(atp, OaEb, EaEb, 'a', no, nothing);
	add_rule(atp, OaEb, OaOb, 'b', no, nothing);
	add_rule(atp, OaOb, EaOb, 'a', no, nothing);
	add_rule(atp, OaOb, OaEb, 'b', no, nothing);
	return atp->rules.size;
}

/* to make a dfa, 8 parts of it need to be made:
 * 1) streamer
 * 2) stateNum
 * 3) labels
 * 4) startState
 * 5) currentState,  no need to set
 * 6) failureState
 * 7) acceptState
 * 8) rules
 *
 *
 * */

SAutomata * make_automata(void){
	SAutomata * atp = (SAutomata *)malloc(sizeof(SAutomata));
	StateSet accepts;
	ARuleSet rules;
	ARule  r;
	unsigned int j;
	/* EaOb means even number of a's, odd number of b's*/
	const char* labels[] = {"EaEb", "EaOb", "OaEb", "OaOb"};

	/* 1) */
	atp->streamer = make_input_streamer();
	if(AT_DEBUG)
		puts("automata's streamer is made.");

	/* 2) */
	atp->stateNum = 4; /* not including the failure state */
	if(AT_DEBUG)
		puts("automata's stateNum is made.");

	/*  3) labels */
    atp->labels = (const char **) malloc(sizeof(const char *) * atp->stateNum);
    /*  (const char**)labels  is used to avoid a warning
     * passing 'const char *[4]' to parameter of type 'char **'
     *  */
    set_labels(atp, (const char**)labels, 4);
	if(AT_DEBUG)
		puts("automata's labels are made.");

	/* 4)  startState*/
	atp->startState = l2s(atp, "EaEb");
	if(AT_DEBUG)
		puts("automata's startState is made.");

	/* 5) currentState,  no need to set now, but doing it does not matter */
	atp->currentState = atp->startState;
	if(AT_DEBUG)
		puts("automata's currentState is made.");

	/* 6) failureState */
	atp->failureState = atp->stateNum;
	if(AT_DEBUG)
		puts("automata's failureState is made.");

	/* 7) acceptStates */
	accepts.size = 1;
	accepts.sizeLimit = 1;
	accepts.addr = (State *)malloc(sizeof(State)*accepts.sizeLimit);
	accepts.addr[0] = l2s(atp, "EaOb");
    atp->acceptStates = accepts;
	if(AT_DEBUG)
		puts("automata's accept states are made.");

    /* 8) rules */
    j = make_rules(atp);
	if(AT_DEBUG)
		printf("%d rules are added into the automata.\n", j);

	/* 9) info */
	atp->info = NULL;

	/* 10) before_running */
	atp->before_running = nothing;

	/* 11) after_running */
	atp->after_running = nothing;

    return atp;
}

int main(void){
	SAutomata * atp;
    char choice;
	intro();
	pause_msg("Press enter to continue.\n");
    atp = make_automata();
    while(TRUE){
    	puts("~~~~~~~~~~~~~~~~~~~~~");
    	puts("Run the automaton?");
    	choice = yes_or_no();
    	if(choice == 'n')
    		break;
    	/* now choice must by 'y'*/
    	run_sautomata(atp);
    	if(is_in_accept(atp)==TRUE)
    		puts("The input string is accepted.");
    	else
    		puts("The input string is rejected.");
    }
    puts("Byebye.");
    return 0;
}
