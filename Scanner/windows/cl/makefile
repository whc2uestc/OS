all: scanner_test_win_cl.exe

clean:
	rm  scanner_test_win_cl.exe scanner_test.obj scan.obj dfa_for_scanner.obj util.obj

# s_automata.obj is provided 
scanner_test_win_cl.exe:  scanner_test.obj scan.obj dfa_for_scanner.obj streamer_for_scanner_stdin.obj streamer_for_scanner_file.obj util.obj s_automata.obj 
	cl -o scanner_test_win_cl.exe scanner_test.obj scan.obj dfa_for_scanner.obj streamer_for_scanner_stdin.obj streamer_for_scanner_file.obj util.obj s_automata.obj

scanner_test.obj: ../../scanner_test.c
	cl -c ../../scanner_test.c

scan.obj: ../../scan.c
	cl -c ../../scan.c

dfa_for_scanner.obj: ../../dfa_for_scanner.c
	cl -c ../../dfa_for_scanner.c

util.obj: ../../util.c
	cl -c ../../util.c


