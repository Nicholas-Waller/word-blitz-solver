#include "helper.h"

int main(void)
{
    printf("Preparing...\n");
    characterTree *root = readWords("words.txt");
    if (!root)
    {
        return -1;
    }

    if (!searchTree(root, "throw"))
    {
        fprintf(stderr, "The generation of the tree has failed\n");
        return -1;
    }
    if (searchTree(root, "asdfasdfasdf"))
    {
        fprintf(stderr, "The generation of the tree has failed\n");
        return -1;
    }

    char board[N][N];
    printf("Please input your board: \n");
    char currentChar;
    for (int i = 0; i < N * N; i++)
    {
        scanf("%c ", &currentChar);
        board[i / N][i % N] = currentChar;
        printf("Scanned %c ", currentChar);
    }
    printf("\n");
    findAllWordsInBoard(root, board);
    return 0;
}

int sortWords(const void *a, const void *b)
{
    const char **strA = (const char **)a;
    const char **strB = (const char **)b;
    return strcmp(*strA, *strB);
}

int sortWordsByLength(const void *a, const void *b)
{
    const char **strA = (const char **)a;
    const char **strB = (const char **)b;
    return strlen(*strA) - strlen(*strB);
}

void findAllWordsInBoard(characterTree *root, char board[N][N])
{
    char traversed[N][N];
    memset(&traversed, 0, sizeof(char) * N * N);

    char **allWords = malloc(sizeof(char *));
    int numWords = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            char *str = malloc(sizeof(char) * WORD_UPPER_BOUND);
            strncpy(str, &(board[i][j]), 1);
            str[1] = '\0';
            printf("Starting with character %s\n", str); // Should print board[i][j]
            traverseTree(root->children[charAsInt(board[i][j])], str, board, i, j, traversed, &allWords, &numWords);
            memset(&traversed, 0, sizeof(char) * N * N);
        }
    }

    qsort(allWords, numWords, sizeof(char *), sortWords);
    char **allWordsFiltered = malloc(sizeof(char *) * numWords);
    allWordsFiltered[0] = allWords[0];
    int numCopied = 1;

    for (int i = 1; i < numWords; i++) // Remove all duplicates
    {
        if (strcmp(allWords[i], allWords[i - 1]))
        {
            allWordsFiltered[numCopied++] = allWords[i];
        }
    }

    qsort(allWordsFiltered, numCopied, sizeof(char *), sortWordsByLength);

    for (int i = 0; i < numCopied; i++)
    {
        printf("%s\n", allWordsFiltered[i]);
    }

    // Free all dangling strings
    for (int i = 0; i < numWords; i++) {
        free(allWords[i]);
    }
    free(allWords);
    free(allWordsFiltered);
    freeTree(root);
}

void traverseTree(characterTree *currentNode, char *currentString, char board[N][N], char currentX, char currentY, char traversed[N][N], char ***listOfAllWords, int *numTotalWords)
{
    if (!currentNode)
    {
        free(currentString);
        return;
    }
    if (currentNode->isWord)
    {
        char** newList = realloc(*listOfAllWords, sizeof(char *) * (*numTotalWords + 1));
        if (!newList) {
            printf("Memory error occurred\n");
        }
        newList[*numTotalWords] = currentString;
        *numTotalWords += 1;
        *listOfAllWords = newList;
    }
    traversed[(int)currentX][(int)currentY] = 1;
    
    int numAdjacent = 0;
    adjacentTile *allAdjacentTiles = findAdjacentTiles(currentX, currentY, traversed, &numAdjacent);

    for (int i = 0; i < numAdjacent; i++)
    {
        char newTraversed[N][N];
        int x = allAdjacentTiles[i].x;
        int y = allAdjacentTiles[i].y;
        memcpy(*newTraversed, *traversed, N * N * sizeof(char));
        // recurse
        char *newString = malloc(sizeof(char) * WORD_UPPER_BOUND);
        strcpy(newString, currentString);
        strncat(newString, &(board[x][y]), 1);
        traverseTree(currentNode->children[charAsInt(board[x][y])], newString, board, x, y, newTraversed, listOfAllWords, numTotalWords);
    }

    if (!currentNode->isWord) {
        free(currentString);
    }

    free(allAdjacentTiles);
}

adjacentTile createAdjacentTile(char x, char y)
{
    adjacentTile tile;
    tile.x = x;
    tile.y = y;
    return tile;
}

adjacentTile *findAdjacentTiles(short currentX, short currentY, char traversed[N][N], int *numAdjacent)
{
    adjacentTile *adjacentTiles = malloc(sizeof(adjacentTile) * 8); // Max number of adjacent tiles for any N x N matrix is 8

    if (isAdjacentTile(currentX - 1, currentY - 1, traversed[currentX - 1][currentY - 1]))
    { // Above and to the left of the current tile
        adjacentTiles[*numAdjacent] = createAdjacentTile(currentX - 1, currentY - 1);
        *numAdjacent = *numAdjacent + 1;
    }
    if (isAdjacentTile(currentX, currentY - 1, traversed[currentX][currentY - 1]))
    { // Above the current tile
        adjacentTiles[*numAdjacent] = createAdjacentTile(currentX, currentY - 1);
        *numAdjacent = *numAdjacent + 1;
    }
    if (isAdjacentTile(currentX + 1, currentY - 1, traversed[currentX + 1][currentY - 1]))
    { // Above and to the right of the current tile
        adjacentTiles[*numAdjacent] = createAdjacentTile(currentX + 1, currentY - 1);
        *numAdjacent = *numAdjacent + 1;
    }
    if (isAdjacentTile(currentX - 1, currentY, traversed[currentX - 1][currentY]))
    { // To the left of the current tile
        adjacentTiles[*numAdjacent] = createAdjacentTile(currentX - 1, currentY);
        *numAdjacent = *numAdjacent + 1;
    }
    if (isAdjacentTile(currentX + 1, currentY, traversed[currentX + 1][currentY]))
    { // To the right of the current tile
        adjacentTiles[*numAdjacent] = createAdjacentTile(currentX + 1, currentY);
        *numAdjacent = *numAdjacent + 1;
    }
    if (isAdjacentTile(currentX - 1, currentY + 1, traversed[currentX - 1][currentY + 1]))
    { // Below and to the left of the current tile
        adjacentTiles[*numAdjacent] = createAdjacentTile(currentX - 1, currentY + 1);
        *numAdjacent = *numAdjacent + 1;
    }
    if (isAdjacentTile(currentX, currentY + 1, traversed[currentX][currentY + 1]))
    { // Below the current tile
        adjacentTiles[*numAdjacent] = createAdjacentTile(currentX, currentY + 1);
        *numAdjacent = *numAdjacent + 1;
    }
    if (isAdjacentTile(currentX + 1, currentY + 1, traversed[currentX + 1][currentY + 1]))
    { // Below and to the right fo the current tile
        adjacentTiles[*numAdjacent] = createAdjacentTile(currentX + 1, currentY + 1);
        *numAdjacent = *numAdjacent + 1;
    }
    return adjacentTiles;
}

char isAdjacentTile(char x, char y, char isTraversed)
{
    return !(x < 0 || x >= N || y < 0 || y >= N || isTraversed);
}