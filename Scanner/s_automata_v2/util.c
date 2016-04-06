 /****************************************************
  * File: util.c
  * A compiler for the C-Minus language
  * Partly inspired by the textbook
  * "Compiler Construction: Principles and Practice"
  * Book author:  Kenneth C. Louden
  * MUST CS106 2015 Fall
  * Programming designed by the teacher: Liang, Zhiyao
  * Created on: Sep 25, 2015
  ****************************************************/

#include "util.h"

void clear_input_queue(void){
	while(getchar() != '\n')
		continue;
}

/* When use type something and hit the Return/Enter, the last character appearin
 * in the inputque is \n.
 * For %d, scanf will leave the \n character in the input queue,
 * but for %c, scanf will not skip any white space character, including the \n.*/
char input_char(char const * msg){
	char result;
	printf("%s", msg);
	while(scanf("%c", &result)!=1){
		if(result != '\n') /* make sure the input queue is not empty */
			clear_input_queue();
		printf("%s", msg);
	}
	if(result != '\n')
		clear_input_queue();
	return result;
}

/* Return the character y or n. Does not quit until a user provide a line start with y or n, case ignored
 * The input queue is cleared.
 * */
char yes_or_no(void){
	char c;
	const char * msg="Please type a character y for yes or n for no, followed by Enter/Return. \n";
	while(TRUE){
		c = tolower(input_char(msg));
		if(c=='y' || c=='n')
			break;
		printf("%c is not y nor n, please do it again.\n", c);
	}
	return c;
}

/* <parameters>:
 * arr: an array of char, which will be used to record a string.
 * size: number of characters that can be saved in arr.
 *       At most size-1 input characters can be received, since there will be a place to put the ending \n character.
 * msg: A string to be printed for a user h
 * <computation>:
 * 1. Print msg.
 * 2. Receive a line of input (ending by return/enter), and record at most the first size-1 characters from the input
 *   and save them in arr.
 *   Print out some message if size is not enough for the input.
 * 3. Put '\0" after all of the recorded characters in arr, to make arr a string.
 * 4. Clear the input queue if the input queue is not empty.
 * 5. Return the number of characters from the input that are saved in arr, not including the added '\0'.
 * */
unsigned int input_line(char * arr, unsigned int size, const char *msg){
	unsigned int j;
	unsigned int count;
	char c;
	puts(msg);
	for(j=0; j<size-1; j++){
		c = getchar();
		if(c=='\n')
			break;
		arr[j]=c;
	}
	arr[j]='\0'; /* make it a string */
	if(j>size-1)
		printf("Some input characters are ignored. At most %u can be received by the program.\n", size-1);
	if(c!='\n')
		clear_input_queue();
	return j;
}

void pause_msg(const char * msg){
	printf("%s", msg);
	clear_input_queue();
}
