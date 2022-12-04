#include "helper.h"

// Convert a -> 0, b -> 1, etc...
short charAsInt(char charToConvert)
{
    return charToConvert - 'a';
}

characterTree *createNode(void)
{
    characterTree *node = malloc(sizeof(characterTree));
    node->isWord = 0;

    for (int i = 0; i < CHARACTER_COUNT; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

void wordIntoTree(characterTree *root, char *word)
{
    short wordLength = strlen(word); // Avoid computation of word length on each iteration of the loop

    characterTree *current = root;
    for (int i = 0; i < wordLength; i++)
    {
        short index = charAsInt(word[i]);
        if (!current->children[index])
        {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->isWord = 1;
}

char searchTree(characterTree *root, char *word)
{

    short wordLength = strlen(word);
    characterTree *current = root;

    for (int i = 0; i < wordLength; i++)
    {

        short index = charAsInt(word[i]);
        if (!(current->children[index]))
        {
            return false;
        }
        current = current->children[index];
    }
    return current->isWord;
}

characterTree *readWords(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        fprintf(stderr, "Could not open file!\n");
        return NULL;
    }

    characterTree *root = createNode();

    char *str = malloc(sizeof(char) * WORD_UPPER_BOUND);
    while (fgets(str, WORD_UPPER_BOUND, file))
    {
        str[strlen(str) - 1] = '\0'; // Remove newline character lazily
        wordIntoTree(root, str);
    }

    fclose(file);

    return root;
}
