all: scanner_test_mac

clean:
	rm  scanner_test_mac scanner_test.o scan.o dfa_for_scanner.o util.o

# s_automata.o is provided 
scanner_test_mac:  scanner_test.o scan.o dfa_for_scanner.o streamer_for_scanner_stdin.o streamer_for_scanner_file.o util.o s_automata.o 
	gcc -o scanner_test_mac scanner_test.o scan.o dfa_for_scanner.o streamer_for_scanner_stdin.o streamer_for_scanner_file.o util.o s_automata.o

scanner_test.o: ../scanner_test.c
	gcc -c ../scanner_test.c

scan.o: ../scan.c
	gcc -c ../scan.c

dfa_for_scanner.o: ../dfa_for_scanner.c
	gcc -c ../dfa_for_scanner.c

util.o: ../util.c
	gcc -c ../util.c


