 /****************************************************
  * File: util.c
  * A compiler for the C-Minus language
  *
  * MUST CS106 2015 Fall
  * Programming designed by whc
  * Created on: Sep 25, 2015
  ****************************************************/

#include "util.h"

void clear_input_queue(void){
	while(getchar() != '\n')
		continue;
}

/* Print the message msg, then let return the first character that the user types.
 * The input queue is cleared if it is not empty before this function returns.
 *
 * <Something on scanf:>
 * When use type something and hit the Return/Enter, the last character appearing
 * in the inputqueue is \n.
 * When %d is used in the format string, scanf will leave the \n character in the input queue,
 * but when %c is used in the format string, scanf will consume any white space character, including the \n.*/
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

/* Return the character y or n. Does not quit until a user provide a line starting with y or n, case ignored
 * The input queue is cleared before this function returns.
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

/* Print the message msg, then wait for the user to hit the enter/return key.
 * The input queue is cleared before this function returns.
 */
void pause_msg(const char * msg){
	printf("%s", msg);
	clear_input_queue();
}

/* <parameters>:
 * arr: an array of char, which will be used to record a line input by the user. Then ending '\n' is not recorded.
 * size: the number of characters that can be saved in arr.
 *       At most size-1 input characters can be recorded, since the '\0' need to be recorded at the end to make arr a string.
 * msg: A string to be printed, which designed as some helping information to let the user know.
 * <computation>:
 * 1. Print msg.
 * 2. Receive a line of input (ending by return/enter), and record at most the first size-1 characters from the input
 *   and save them in arr. The ending '\n' is not saved in arr.
 *   Print out some message if size is not enough for the input.
 * 3. Put '\0" after all of the recorded characters in arr, to make arr a string.
 * 4. Clear the input queue if the input queue is not empty (when last consumed character is not '\n').
 * 5. Return the number of characters from the input that are saved in arr, not counting the added '\0'.
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

/* <parameters>:
 * arr: an array of char, which will be used to record multiple lines of users input as a string.
 * size: number of characters that can be saved in arr.
 *       At most size-1 input characters can be received, since the '\0' need to be recorded at the end to make arr a string.
 * end: A value that can be returned by getchar(), and when end is received, the input_line function call should stop receiving character.
 * msg: A string to be printed, which designed as some helping information to let the user know.
 * <computation>:
 * 1. Print msg.
 * 2. Receive line(s) of characters user typed on a keyboard, record at most the first size-1 characters
 *   and save them in arr, until end is received, which is returned by getchar(), or size-1 characters are received.
 *   Print out some message if size is not enough for the input.
 * 3. Put '\0" after all of the recorded characters in arr, to make arr a string.
 * 4. Clear the input queue if the input queue is not empty.
 * 5. Return the number of characters from the input that are saved in arr, not counting the added '\0'.
 * */
unsigned int input_lines(char * arr, unsigned int size, int end, const char *msg){
	int ch;
	int count = 0;
	int last;
	puts(msg);
	while((ch = getchar()) != end && count < size-1){
		arr[count++] = ch;
	}
	if(count>=size-1)
		printf("The input length limit %d is reached, some characters could be omitted.\n", size);
	if(ch != '\n') /* the last char received is ch */
		clear_input_queue();
	arr[count] = '\0'; /* make it a string */
	return count;
}

/* <Parameter:>
 * str:  a character string.
 * <Return:>
 * a copy (a clone) of the input string str, including the ending '\0'. The space of the clone does not overlap with the space of str.
 * */
char * string_clone(const char* str){
 /* !!!!!!!!!!!!! Please provide the missing code here !!!!!!!!!!!*/
	int i;
	int len = strlen(str)+1;
	char *copy_str = (char*)malloc(len);
	for(i=0; i<len; i++)
		copy_str[i] = str[i];
	return copy_str;
}
