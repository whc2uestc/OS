 /****************************************************
  * File: streamer.h
  * A compiler for the C-Minus language
  *
  * MUST CS106 2015 Fall
  * Programming designed by whc
  * Created on: Oct 27, 2015
  ****************************************************/

#ifndef _STREAMER_H_
#define _STREAMER_H_

#include "util.h"

struct streamer;
typedef struct streamer Streamer;

/* Interface of using a read-only stream */
struct streamer{
  Bool (*begin)(Streamer *); /* goto the beginning of a stream, be prepared for the start */
  int (*next_position)(Streamer *); /* increment the current position in the stream, if it is not the end */
  int (*get_current_poistion)(Streamer *); /* return the current position as an integer, without changing it */
  int (*prev_position)(Streamer *); /*go back to the previous position, decrement the current position if it is not the begining. */
  int (*goto_position)(Streamer *, int); /* set the current position to the parameter position */
  int (*get_current_char)(Streamer *); /* return the char at the current position, without advancing the position in the stream */
  Bool (*is_end)(Streamer *); /* Returns TRUE if the end of the stream is reached (by a certain logic), no remaining character in the stream*/
  Bool (*close)(Streamer *); /* close the stream (by some proper actions) , return a signal indicating success or failure,
                     return TRUE means success,  FALSE means failure */
  void * info;    /* A place to save some possible data. It could be NULL if not necessary*/
};



#endif
