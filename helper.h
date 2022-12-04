#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define WORD_UPPER_BOUND 20
#define CHARACTER_COUNT 26
#define N 4

/*
    Used as a word tree, which can be recursively searched through in log(n) time.
    words - The subwords of the current character. If current character is 'a', then a subcharacter could be 'b', all the way down to the word "absolutely"
    isWord - Whether or not the current iteration is a word.
*/
typedef struct characterNode
{
    struct characterNode *children[CHARACTER_COUNT];
    char isWord;
} characterTree;

typedef struct adjacentTile
{
    int x;
    int y;
} adjacentTile;

// ------------------------ wordTree.c functions -----------------------------------
short charAsInt(char charToConvert);
// Reads the words in fileName and inserts them into the characterTree
characterTree *readWords(char *fileName);
// Creates an individual node in the tree and initializes its children.
characterTree *createNode(void);
// Inserts word into the characterTree root
void wordIntoTree(characterTree *root, char *word);
// Search the tree to see if word is a word given in the characterTree root
char searchTree(characterTree *root, char *word);

// // ------------------------ wordBlitzSolver.c functions -----------------------------------
void findAllWordsInBoard(characterTree *root, char board[N][N]);
void traverseTree(characterTree* currentNode, char* currentString, char board[N][N], char currentX, char currentY, char traversed[N][N], char*** listOfAllWords, int *numTotalWords);
adjacentTile* findAdjacentTiles(short currentX, short currentY, char traversed[N][N], int* numAdjacent);
char isAdjacentTile(char x, char y, char isTraversed);
