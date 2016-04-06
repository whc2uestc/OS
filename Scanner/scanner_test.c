 /****************************************************
  * File: scanner_test.c
  * A compiler for the C-Minus language
  *
  * MUST CS106 2015 Fall
  * Programming designed by whc
  * Created on: Nov 2, 2015
  ****************************************************/

#include "util.h"
#include "scan.h"
#include "dfa_for_scanner.h"
#include "streamer_for_scanner.h"

#define FILE_NAME_LEN 80

char pgm[FILE_NAME_LEN]; /* C- program source code file name */

void set_controls(void){
	/*Set all of the controlling toggles here. Cannot put the following statements at global scope,
	 * causing the error redefining variables with type int.  */
	SCANNER_TRACE = TRUE;
	AT_DEBUG = FALSE;  /* True to see debugging information */
	SCAN_DEBUG = FALSE;
}

TokenList * theTokenList = NULL; /* The result of running the scanner  will be saved here */
SAutomata * scanner = NULL;
Streamer * fileStreamer = NULL;
Streamer * stdinStreamer = NULL;

int main(void){
	char choice;

    set_controls();

	puts("Welcome, now we test the scanner of the language C-minus");
	while(TRUE){
		puts("~~~~~~~~~~~~~~~~~~~~~~~~~~");
		choice = input_char("Please choose:\n"
				"0 to quit \n"
				"1 to run scanner with source code input from a file\n"
				"2 to run scanner with source code input from keyboard\n"
		);
		if(choice == '0')
			break;
		else if(choice == '1'){
			/* -5 is for reserving space to add .cm */
			input_line(pgm, FILE_NAME_LEN - 5 , "What is the file name of the program?");
			if (strchr (pgm, '.') == NULL)
			    strcat(pgm,".cm");  /* Assume the C-Minus program has a .cm extension name */
			if(scanner == NULL){ /* first time to run the scanner */
				fileStreamer = make_file_streamer(pgm);
				scanner = make_scanner_dfa(fileStreamer);
				}
			else{ /* not the first time to run the scanner */
				if(fileStreamer !=NULL) /* not the first time to run the scanner with file streamer, only need to change the file name */
					change_file_name_in_streamer(fileStreamer, pgm);
				else /* first time to run the scanner automata with a file streamer, change its streamer to the file version. */
					fileStreamer = make_file_streamer(pgm);
				scanner->streamer = fileStreamer;
			}
		}
		else if(choice == '2'){
			if(scanner == NULL){ /* first time to run the scanner */
				stdinStreamer = make_stdin_streamer();
				scanner = make_scanner_dfa(stdinStreamer);
			}
			else { /* not the first time to run the scanner */
				if(stdinStreamer == NULL) /* the first time to run the scanner with file streamer, only need to change the file name */
					stdinStreamer = make_stdin_streamer();
				scanner->streamer = stdinStreamer;
			}
		}
		else{
			/* Note that input_char() has already cleared the input queue, don't need to worry about this issue. */
			printf("The input choice %d is wrong, must be 0, 1, or 2. Try again.\n", choice);
			continue;
		}
		theTokenList = run_scanner_dfa(scanner);
		if(SCANNER_TRACE== TRUE){
			puts("This is the list of tokens:");
			print_token_list(theTokenList);
		}
	}
	puts("Bye");
	return 0;
}




