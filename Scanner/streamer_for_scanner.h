/****************************************************
  * File: streamer_for_scanner.h
  * A compiler for the C-Minus language

  * MUST CS106 2015 Fall
  * Programming designed by whc
  * Created on: 22 Oct 2015
  ****************************************************/



#ifndef _STREAMER_FOR_SCANNER_H_
#define _STREAMER_FOR_SCANNER_H_

#include "streamer.h"

/* EOS means End Of Stream */
/* For a streamer that will be used by the scanner, the get_current_char() function should return EOS at the end of the stream.
 * The concept is similar to EOF, but we use a different symbol EOS for two reasons:
 * 1. EOS is an independent abstraction of the end of a stream.
 * 2. Making EOS representing '\0' can be helpful for interpreter-like situations, when a piece of code is saved as a C string.*/
#define EOS   '\0'

/* At most how many characters can be typed at keyboard as input code */
#define STDIN_INPUT_LIMIT 1000

Streamer * make_file_streamer(const char* inputFileName);

Streamer * make_stdin_streamer(void);

void change_file_name_in_streamer(Streamer * streamer, const char * fileName);

#endif
