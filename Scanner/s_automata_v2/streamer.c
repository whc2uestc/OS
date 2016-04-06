
 /****************************************************
  * File: s_automata.h
  * A compiler for the C-Minus language
  * Partly inspired by the textbook
  * "Compiler Construction: Principles and Practice"
  * Book author:  Kenneth C. Louden
  * MUST CS106 2015 Fall
  * Programming designed by the teacher: Liang, Zhiyao
  * Created on: Oct 27, 2015
  ****************************************************/

#include "streamer.h"

#define INPUT_LINE_LIMIT 100


struct inputInfo{
	char inputString[INPUT_LINE_LIMIT];
	int inputPosition;
	int beginPosition;
	int endPosition;
};

typedef struct inputInfo  InputInfo;

static Bool begin_1(Streamer * s){
	InputInfo * info = (InputInfo *) s->info;
	if(info == NULL){
		s->info = malloc(sizeof(InputInfo));
		info = (InputInfo *) s->info;
	}
	puts("Please type a string as the input of the automata.");
	/* cannot input the empty string by the following statement.
	 * scanf("%s", inputString);
	 */
	input_line(info->inputString, INPUT_LINE_LIMIT, "Type a line ending by the Enter/Return key. ");
	info->inputPosition = 0;
	info->beginPosition = 0;
	info->endPosition = strlen(info->inputString); /* endposition is the index of '\0' */
	return TRUE;
}

static int next_position_1(Streamer * s){
	InputInfo * info = (InputInfo *) s->info;
	if(info->inputPosition < info->endPosition)
		++info->inputPosition;
	return info->inputPosition;
}

/* return the current position as an integer, without changing it */
int get_current_position_1(Streamer * s){
	InputInfo * info = (InputInfo *) s->info;
	return info->inputPosition;
}

/*go back to the previous position */
int prev_position_1(Streamer * s){
	InputInfo * info = (InputInfo *) s->info;
	if(info->inputPosition > info->beginPosition)
		info->inputPosition--;
	return info->inputPosition;
}

/* goto the position p*/
int goto_position_1(Streamer * s, int p){
	InputInfo * info = (InputInfo *) s->info;
  if(p< info->beginPosition || p > info->endPosition) /* failure */
	  return -1;
  info->inputPosition = p;
  return p;
}

/* No need to return int, consider '\0' as the end if needed.
 * static char get_current_char_1(void){
*/
static int get_current_char_1(Streamer * s){
	InputInfo * info = (InputInfo *) s->info;
	return info->inputString[info->inputPosition];
}

/* static char unget_char_1(char c){ */
/*
static char unget_char_1(char c){
	inputString[--inputPosition] = c;
	return c;
}
*/
static Bool is_end_1(Streamer * s){
	InputInfo * info = (InputInfo *) s->info;
	/* For different situation, like running a scanner, the ending \0 need to be returned
	 * then the condition will be
	 * inputPosition > strlen(inputString)
	 *  */
	if(info->inputPosition >= strlen(info->inputString))
		return TRUE;
	else
		return FALSE;
}


Bool close_1(Streamer * s){
	InputInfo * info = (InputInfo *) s->info;
	info->inputPosition = info->endPosition = 0;
	info->inputString[0] = '\0'; /* empty input string */
	return TRUE; /* always success */
}

/* returns a streamer representing a line received from keyboard
 * removed from streamer.h to here.
 * */
Streamer *  make_input_streamer(void){
	Streamer * streamer = (Streamer *)malloc(sizeof(Streamer));
	streamer->begin = begin_1;
	streamer->next_position =  next_position_1;
	streamer->get_current_char = get_current_char_1;
	streamer->is_end = is_end_1;
	streamer->get_current_poistion = get_current_position_1;
	streamer->goto_position = goto_position_1;
	streamer->close = close_1;
	streamer->info = NULL;
	return streamer;
}

