#ifndef INDEX_H
#define INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WORDS_BUFFER 10
#define LINES_BUFFER 5

typedef struct {
    char *word;
    int *lineNumbersArray;
    int lineNumbersSize;
    int lineNumbersCount;
} WordIndex;

/**
 * Initializes a WordIndex structure.
 *
 * @param wordIndex The WordIndex structure to be initialized.
 */
void initWordIndex(WordIndex *wordIndex);

/**
 * Frees the memory allocated for a WordIndex structure.
 *
 * @param wordIndex The WordIndex structure to be freed.
 */
void freeWordIndex(WordIndex *wordIndex);

/**
 * @brief Compares two words.
 *
 * This function is used as a comparison function for sorting words.
 * It compares two words based on their addresses in memory.
 *
 * @param a Pointer to the first word.
 * @param b Pointer to the second word.
 * @return An integer less than, equal to, or greater than zero if the first word is 
 *         found to be less than, equal to, or greater than the second word, respectively.
 */
int compareWords(const void *a, const void *b);

/**
 * Frees the memory allocated for an array of WordIndex structures.
 *
 * @param array The array of WordIndex structures to be freed.
 * @param arraySize The size of the array.
 */
void freeWordIndexArray(WordIndex *array, int *arraySize);

#endif  /* INDEX_H */
