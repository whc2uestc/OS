 /****************************************************
  * File: s_automata.h
  * A compiler for the C-Minus language
  * Partly inspired by the textbook
  * "Compiler Construction: Principles and Practice"
  * Book author:  Kenneth C. Louden
  * MUST CS106 2015 Fall
  * Programming designed by the teacher: Liang, Zhiyao
  * Created on: Sep 25, 2015
  ****************************************************/

#ifndef _S_AUTOMATA_H_
#define _S_AUTOMATA_H_

#include "util.h"
#include "streamer.h"

/* Message printed by s2l() when a state does not belong to the automaton */
#define WRONG_STATE_MSG "!wrong state number!"

/*Label for the special failure state */
#define FAILURE_STATE_LABEL "!Failure!"

/*When it is TRUE, print computation trace details of running an automata */
extern Bool AT_DEBUG;

typedef unsigned int State;

/* In the word SAutomata, 's' means Special, Scanner, or Stream.
 * Since the structures sAutomata and aRule are mutually referencing, we have to mention
 * the structure name of one of them here.*/
struct sAutomata;
typedef struct sAutomata SAutomata;

/* assuming the alphabet is the ASCII characters, or what can be represented by the type char, plus
 * some special-purpose symbols */

 /*  A transition rule (of type ARule) is defined below as a tuple (c, from, to, condition, side_effect).
  *  When an automaton is running, and its character is X, and a current state is S,  a  rule r is matched (ready to be triggered/fired)  when:
  *  r's from is same as S, and,  ( r's c equals to X, OR, r's condition is True.).
  *  So, when r's c is different from any possible X, like when r's c is negative, r's condition must be True if r is matched.
  *  Also, when the r's condition is always False, X must be equal to r's c if r is matched.
 */
struct aRule{
	int c;
	State from;
	State to;
	Bool (*condition)(SAutomata *);
	/* side_effect is designed to describe the action that is performed by a transducer automaton when firing this rule,
	 * like recording some book-keeping data, something that normally a simple/classical DFA will not do.
	 * So, the side_effect should normally not include the action of changing the current state of the automaton,
	 * which be automatically done by the automaton (handled by the run_sautomata() function).
	 */
	void (*side_effect)(SAutomata*);
};

/* aRule means atomata's rule */
typedef struct aRule  ARule;

typedef struct aRuleSet{
	unsigned int sizeLimit; /* maximum number of elements in the set */
	unsigned int size; /* actually how many elements are saved in this set. size<=sizeLimit*/
	ARule * addr; /*address of the first element in an array */
}ARuleSet;

typedef struct stateSet{
	unsigned int sizeLimit; /* maximum number of elements in the set */
	unsigned int size; /* actually how many elements are saved in this set. size<=sizeLimit*/
	State * addr; /*address of the first element in an array */
}StateSet;

/* Assume the states range from 0 to stateNum-1
 * startState is between 0 and stateNum-1
 * By convention, failureState is "invisible", it should not be between 0 and stateNum-1
 * It is suggested that failureState >=stateNum,
 * */
struct sAutomata{
	Streamer * streamer;  /* making it a pointer, for easiness of memory management */
	unsigned int stateNum;
	const char * * labels; /* an array of stateNum strings, corresponding to state 0 to stateNum-1*/
	State startState;
	State currentState;
	State failureState;
	StateSet acceptStates;
	ARuleSet rules;
	/* the following three fields are specially designed for the automaton as a transducer. */
	void * info; /* a place to save additional information, like some bookkeeping information at running time.*/
	void (*before_running)(struct sAutomata * self); /* Do something to prepare the bookkeeping information */
	void (*after_running)(struct sAutomata * self);  /* Do something to finally process the bookkeeping information*/
};


/*label_to_state*/
State l2s(SAutomata* at, const char * label);

/*state_to_label*/
const char * s2l(SAutomata* at, State s);

/* <Parameters>
 * at: an automaton
 * labels: an array of strings
 * len: the number of elements in the array labels.
 * <computation>:
 * Suppose when len<=at.stateNum, at.labels[j] is the same as labels[j], for 0<=j<=len-1.
 * For all other states their labels are set with an empty string.
 * When len > at.stateNum, only the first at.stateNum strings in labels are used, and the extra labels are not used.
 */
void set_labels(const  SAutomata * at, const char * labels[], unsigned int len);

/* returns TRUE when the current state of the automaton is an accept state, otherwise return FALSE.*/
Bool is_in_accept(SAutomata *atp);

/* Add a rule into the automata pointed by atp. Details of the rule are provided by the parameters.
 * Return the the rule number, which is an index in the array.
 *  */
unsigned int add_rule(SAutomata *atp, State from, State to, int c, Bool (*condition)(/*char int*/ SAutomata *), void (*side_effect)(SAutomata*));


/* Run the automaton, and return the last state when the end of its streamer is reached. */
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
 */
State run_sautomata(SAutomata * at);


#endif
