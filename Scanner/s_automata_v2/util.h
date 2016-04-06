 /****************************************************
  * File: util.h
  * A compiler for the C-Minus language
  * Partly inspired by the textbook
  * "Compiler Construction: Principles and Practice"
  * Book author:  Kenneth C. Louden
  * MUST CS106 2015 Fall
  * Programming designed by the teacher: Liang, Zhiyao
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
}Bool;

/* Clear the input queue of stdin stream.
 * Be careful: if this function is called when the
 * input queue is not empty, the function will work like pause, waiting for a character input
 */
void clear_input_queue(void);

char input_char(char const * msg);

char yes_or_no(void);

unsigned int input_line(char * arr, unsigned int size, const char * msg);

void pause_msg(const char * msg);

#endif
