all: at_test_win_cl

clean:
	rm  at_test_win_cl  at_test.obj s_automata.obj streamer.obj util.obj 

at_test_win_cl: at_test.obj s_automata.obj streamer.obj util.obj
	cl -o at_test_win_gcc at_test.obj s_automata.obj streamer.obj util.obj

at_test.obj: ../../at_test.c
	cl -c ../../at_test.c

s_automata.obj: ../../s_automata.c
	cl -c ../../s_automata.c

streamer.obj: ../../streamer.c
	cl -c ../../streamer.c

util.obj: ../../util.c
	cl -c ../../util.c

