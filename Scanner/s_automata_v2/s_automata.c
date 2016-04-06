 /****************************************************
  * File: s_automata.c
  * A compiler for the C-Minus language
  * Partly inspired by the textbook
  * "Compiler Construction: Principles and Practice"
  * Book author:  Kenneth C. Louden
  * MUST CS106 2015 Fall
  * Programming designed by the teacher: Liang, Zhiyao
  * Created on: Sep 25, 2015
  ****************************************************/

#include "s_automata.h"

Bool AT_DEBUG = TRUE;

/* <Parameter>: atp is a pointer to an automaton
 * <Computation> outlined as follows pseudo code:
 *    1.   Let the current state of atp to be the start state of atp.
 *    2.   Do the before-running tasks of the automaton (call the before_running() function of atp).
 *    3.   Let the streamer of atp begin (call the function). If the streamer cannot begin, atp's current state turns to the Failure state, then return the Failure state.
 *    4.   (step 4 starts a loop iteration )
 *         If the current state of atp is the failure state, quit the loop -- goto step 10.
 *    5.   If end of the streamer of atp is reached (call is_end() function), quit the loop -- goto step 10).
 *    6.   Get the current character X for the streamer atp (call the get_current_char() function).
 *    7.   Try each transition rule of atp, to check if a rule is matched. A rule r is matched if
 *              atp's current state equals to  r's from state, AND,
 *              r's character c equals X, OR,  r's condition function is satisfied (returns TRUE).
 *         When the first matched rule r is found:  change atp's current state to r's "to" state, and call r's side_effect function.
 *         If no rule r can be matched, then atp's current state goes to the Failure state.
 *    8.   Advance the current position of atp's streamer to its next one (call the next_position() function ).
 *    9.   Continue the loop, go back to step 4.
 *    10.  Do the after-running tasks of the automaton (call the after_running() function of atp).
 *    11.  close the streamer (call the close() function of the streamer).
 *    12   Return the current state (last state) of atp.
 * <Optional behavior>:
 *    If AT_DEBUG is TRUE, print out some internal details as desired.
 * <Return>:
 *     The current state of atp when the computation finishes.
 *
 */
State run_sautomata(SAutomata * atp){
	int currentChar;
	Streamer *  streamer = atp->streamer;
	/* int currentChar; */
	int j; /* j is the index of a rule */
	if(AT_DEBUG)
		puts("~~~~~~~Now the DFA starts running~~~~~~~");
	atp->currentState = atp->startState;
	atp->before_running(atp);
	if(AT_DEBUG)
	  printf("The before-running tasks are done. The current state is %s.\n", s2l(atp, atp->currentState));

	if(streamer->begin(streamer) == FALSE){
		puts("the input stream cannot start, quit.");
		atp->currentState = atp->failureState;
		return(atp->failureState);
	}
	while(TRUE){
		if(atp->currentState == atp->failureState){
			if(AT_DEBUG)
				  puts("failure state is reached. Quit the DFA");
			break;
		}
		if(streamer->is_end(streamer) == TRUE){
			if(AT_DEBUG)
				puts("The end of the streamer is reached. Quit the DFA");
			break;
		}
		j = 0;
		currentChar = streamer->get_current_char(streamer);
		if(AT_DEBUG){
			printf("Current state: %u, with label: %s,  current input char:  %c with value %d\n", atp->currentState, s2l(atp, atp->currentState), currentChar, currentChar );
		}
		/* try to find the first applicable transition rule*/
		while(j< atp->rules.size ){
			ARule r = atp->rules.addr[j];
		    if(r.from == atp->currentState &&
		    		(r.c == currentChar || r.condition(atp) == TRUE)  ){
		    	/* fire the rule */
		    	atp->currentState = r.to;
		    	r.side_effect(atp);
		    	break; /* stop after firing the first applicable rule */
		    }
		    j++; /*update j, fixed 1a bug */
		}
		if (j >= atp->rules.size){ /* no applicable rule*/
			atp->currentState = atp->failureState;
		}
		streamer->next_position(streamer); /*after consuming the current char, advance the position in the stream */
	}
	atp->after_running(atp);
	if(AT_DEBUG)
		puts("The after-running tasks are done.");
	if(AT_DEBUG)
		printf("End of input. Last state: %u, with label: %s \n", atp->currentState, s2l(atp, atp->currentState) );
	if(streamer != NULL){
		if(streamer->close(streamer) != TRUE)
			printf("The streamer of the automata cannot be closed successfully.\n");
		else if(AT_DEBUG)
			puts("The streamer of the automata is closed successfully");
	}
	return atp->currentState;
}

/*label to state*/
State l2s(SAutomata* at, const char * label){
   unsigned int j;
   int limit = at->stateNum;
   for(j=0; j<limit; j++){
	   const char * l = at->labels[j];
	   if(strcmp(label, l)==0)
		   return j;
   }
   /* cannot find the label */
   printf("The label %s is not used the the automata\n", label);
   return at->failureState; /* failureState indicates a wrong label */
}

/*state to label */
const char * s2l(SAutomata* at, State s){
	if(s < at->stateNum)
		return at->labels[s];
	else
		if(s == at->failureState)
			return FAILURE_STATE_LABEL;
		else
			return WRONG_STATE_MSG;
}

/* len is the length of the array labels*/
void set_labels(const  SAutomata * at, const char * labels[], unsigned int len){
	unsigned int stateNum = at->stateNum;
	unsigned int limit = ( stateNum > len) ? len :  stateNum; /* smaller one */
	int j;
	for(j=0; j<limit; j++)
		at->labels[j] = labels[j];
	if (limit < stateNum)
		for(j=limit; j<stateNum; j++)
			at->labels[j] = ""; /* for a state whose label is not provided in labels, its label is an empty string. */
	return;
}

Bool is_in_accept(SAutomata * atp){
	State s = atp->currentState;
	int j;
	int limit = atp->acceptStates.size;
	for(j=0; j<limit; j++)
		if(s == atp->acceptStates.addr[j])
			return TRUE;
	return FALSE;
}

unsigned int add_rule(SAutomata *atp, State from, State to, /*char c*/  int c, Bool (*condition)(/*char*/ /*int*/ SAutomata *), void (*side_effect)(SAutomata*)){
	unsigned int index = atp->rules.size;
	ARule * r = atp->rules.addr + index;
	r->from = from;
	r->to = to;
	r->c = c;
	r->condition = condition;
	r->side_effect = side_effect;
	atp->rules.size++; /* one rule is added, increase the size of rules */
	if(atp->rules.size > atp->rules.sizeLimit){
		printf("Error: Number or rules of the automaton exceeds the limit.\n");
		exit(1); /* terminate the whole program */
	}
	return index;
}

