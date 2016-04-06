 /****************************************************
  * File: util.h
  * A compiler for the C-Minus language
  *
  * MUST CS106 2015 Fall
  * Programming designed by whc
  * Created on: Sep 25, 2015
  ****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

/*  Putting  the library head files here.
 *  Let this file be included in each file where they are needed */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


typedef enum{
  FALSE,
  TRUE
} Bool;

/*
enum bool{
  FALSE,
  TRUE
};

typedef enum bool Bool;
*/

/* Clear the input queue of stdin stream.
 * Be careful: if this function is called when the
 * input queue is empty, the function will work like pause, waiting for a character input
 */
void clear_input_queue(void);

/* Print the message msg, then let return the first character that the user types.
 * The input queue is cleared if it is not empty before this function returns
 */
char input_char(char const * msg);

/* Return the character y or n. Does not quit until a user provide a line starting with y or n, case ignored
 * The input queue is cleared before this function returns.
 * */
char yes_or_no(void);

/* Print the message msg, then wait for the user to hit the enter/return key.
 * The input queue is cleared before this function returns.
 */
void pause_msg(const char * msg);

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
unsigned int input_line(char * arr, unsigned int size, const char * msg);

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
unsigned int input_lines(char * arr, unsigned int size, int end, const char *msg);

/* <Parameter:>
 * str:  a character string.
 * <Return:>
 * A copy (a clone) of the input string str, including the ending '\0'. The space of the clone does not overlap with the space of str.
 * */
char * string_clone(const char* str);

#endif
