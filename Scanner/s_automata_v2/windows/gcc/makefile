all: at_test_win_gcc

clean:
	rm  at_test_win_gcc  at_test.o s_automata.o streamer.o util.o 

at_test_win_gcc: at_test.o s_automata.o streamer.o util.o
	gcc -o at_test_win_gcc at_test.o s_automata.o streamer.o util.o

at_test.o: ../../at_test.c
	gcc -c ../../at_test.c

s_automata.o: ../../s_automata.c
	gcc -c ../../s_automata.c

streamer.o: ../../streamer.c
	gcc -c ../../streamer.c

util.o: ../../util.c
	gcc -c ../../util.c

