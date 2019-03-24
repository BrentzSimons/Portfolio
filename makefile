connectFour.exe: main.o graph.o ui.o AI.o
	gcc main.o graph.o ui.o AI.o -o connectFour
main.o: main.c
	gcc main.c -Wall -c
graph.o: graph.c
	gcc graph.c -Wall -c
ui.o: ui.c
	gcc ui.c -Wall -c
AI.i: AI.c
	gcc AI.c -Wall -c

clear:
	rm *.o

run:
	./connectFour
