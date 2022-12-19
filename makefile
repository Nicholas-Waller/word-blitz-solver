SRC = src/
INC = include/
BIN = bin/

solver: $(BIN)wordBlitzSolver.o $(BIN)wordTree.o
	gcc -Wall -std=c99 $(BIN)wordBlitzSolver.o $(BIN)wordTree.o -o solver

$(BIN)wordBlitzSolver.o: $(INC)helper.h $(SRC)wordBlitzSolver.c 
	gcc -Wall -std=c99 -I$(INC) -c $(SRC)wordBlitzSolver.c -o $(BIN)wordBlitzSolver.o

$(BIN)wordTree.o: $(SRC)wordTree.c $(INC)helper.h
	gcc -Wall -std=c99 -I$(INC) -c $(SRC)wordTree.c -o $(BIN)wordTree.o
	
clean:
	rm *.o $(BIN)*.o solver
