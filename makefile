solver: wordBlitzSolver.o wordTree.o
	gcc -Wall -std=c99 wordBlitzSolver.o wordTree.o -o solver

wordBlitzSolver.o: wordBlitzSolver.c helper.h
	gcc -Wall -std=c99 -c wordBlitzSolver.c

wordTree.o: wordTree.c helper.h
	gcc -Wall -std=c99 -c wordTree.c
	
clean:
	rm *.o solver a.out
