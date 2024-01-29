/**
 * @file index.c
 * @brief This file contains the implementation of the index program.
 *
 * The index program is responsible for creating and managing a word index for a file.
 * It allows users to select a file and make an index for its words, meaning printing the words
 * alphabetically each with the line numbers on which they appear.
 **/

#include "index.h"


int main() {
    char filename[50];
    char line[150];
    WordIndex *wordIndexArray = NULL;
    int wordIndexArraySize = 0;
    int wordIndexCount = 0;
    void *temp = NULL;
    char *token = NULL;
    FILE *file = NULL;
    int lineNumber;
    int i;
    int j;

    printf("Enter the name of the file to index: ");
    fgets(filename, sizeof(filename), stdin);
    printf("You entered: %s\n\n", filename);

    /*  Remove trailing newline character */
    filename[strlen(filename) - 1] = '\0';

    token = strtok(filename, " ");
    if (token == NULL) {
        fprintf(stderr, "Error: No filename provided. Please provide a filename.\n");
        return 1; 
    } else {
        token = strtok(NULL, " ");
        if (token != NULL) {
            fprintf(stderr, "Error: Multiple filenames provided. Please provide only one filename.\n");
            return 1;
        }
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the file.\n");
        return 1;
    }
    
    wordIndexArraySize = WORDS_BUFFER;
    wordIndexArray = malloc(wordIndexArraySize * sizeof(WordIndex));
    if (wordIndexArray == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for word index array.\n");
        fclose(file);
        return 1;
    }
    
    for (i = 0; i < wordIndexArraySize; i++) {
        initWordIndex(&wordIndexArray[i]);
    }

    lineNumber = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *word = strtok(line, " \t\n");
        while (word != NULL) {
            for (i = 0; i < wordIndexCount; i++) {
                if (strcmp(wordIndexArray[i].word, word) == 0) {
                    if (wordIndexArray[i].lineNumbersArray[wordIndexArray[i].lineNumbersCount - 1] == lineNumber) {
                        break;  /* This word already appear earlier in this line */
                    }
                    if (wordIndexArray[i].lineNumbersCount == wordIndexArray[i].lineNumbersSize) {
                        wordIndexArray[i].lineNumbersSize *= 2;
                        temp = realloc(wordIndexArray[i].lineNumbersArray, wordIndexArray[i].lineNumbersSize * sizeof(int));
                        if (temp == NULL) {
                            fprintf(stderr, "Error: Failed to reallocate memory for line numbers array.\n");
                            freeWordIndexArray(wordIndexArray, &wordIndexArraySize);
                            fclose(file);
                            return 1;
                        }
                        wordIndexArray[i].lineNumbersArray = temp;  /* Reassignment is safe because realloc was successful */
                    }
                    wordIndexArray[i].lineNumbersArray[wordIndexArray[i].lineNumbersCount] = lineNumber;
                    wordIndexArray[i].lineNumbersCount++;
                    break;
                }
            }
            if (i == wordIndexCount) {
                if (wordIndexCount == wordIndexArraySize) {
                    wordIndexArraySize *= 2;
                    temp = realloc(wordIndexArray, wordIndexArraySize * sizeof(WordIndex));
                    if (temp == NULL) {
                        fprintf(stderr, "Error: Failed to reallocate memory for word index array.\n");
                        freeWordIndexArray(wordIndexArray, &wordIndexArraySize);  /* Free the originally allocated memory */
                        fclose(file);
                        return 1;
                    }
                    wordIndexArray = temp;  /* Reassignment is safe because realloc was successful */
                    for (j = wordIndexCount; j < wordIndexArraySize; j++) {
                        initWordIndex(&wordIndexArray[j]);
                    }
                }
                temp = malloc((strlen(word) + 1) * sizeof(char));

                if (temp == NULL) {
                    fprintf(stderr, "Error: Failed to allocate memory for word.\n");
                    freeWordIndexArray(wordIndexArray, &wordIndexArraySize);  /* Free the originally allocated memory */
                    fclose(file);
                    return 1;
                }
                wordIndexArray[i].word = temp;  /* Reassignment is safe because malloc was successful */
                strcpy(wordIndexArray[i].word, word);
                /*printf("wordIndexArray[i].word = %s\n", wordIndexArray[i].word);*/


                wordIndexArray[i].lineNumbersSize = LINES_BUFFER;
                temp = malloc(wordIndexArray[i].lineNumbersSize * sizeof(int));
                if (temp == NULL) {
                    fprintf(stderr, "Error: Failed to allocate memory for line numbers.\n");
                    printf("wordIndexArray[i].lineNumbersSize = %d\n", wordIndexArray[i].lineNumbersSize);
                    printf("i = %d\n", i);
                    printf("wordIndexArraySize = %d\n", wordIndexArraySize);
                    printf("wordIndexCount = %d\n", wordIndexCount);
                    printf("wordIndexArray[i].word = %s\n", wordIndexArray[i].word);
                    printf("wordIndexArray[i].lineNumbersArray = %p\n", (void *)wordIndexArray[i].lineNumbersArray);
                    freeWordIndexArray(wordIndexArray, &wordIndexArraySize);  /* Free the originally allocated memory */
                    fclose(file);
                    return 1;
                }
                wordIndexArray[i].lineNumbersArray = temp;
                wordIndexArray[i].lineNumbersArray[0] = lineNumber;
                wordIndexArray[i].lineNumbersCount++;
                wordIndexCount++;
            }
            word = strtok(NULL, " \t\n");
            /*printf("wordIndexArray[i].word strtok= %s\n", wordIndexArray[i].word);*/
        }   
        lineNumber++;
        /*printf("wordIndexArray[i].word linnumber i: %d, %s\n", i, wordIndexArray[i].word);*/
    }

    fclose(file);

    qsort(wordIndexArray, wordIndexCount, sizeof(WordIndex), compareWords);

    for (i = 0; i < wordIndexCount; i++) {
        if (wordIndexArray[i].lineNumbersCount == 1) {
            printf("%-8s appears in line ", wordIndexArray[i].word);
        } else {
            printf("%-8s appears in lines ", wordIndexArray[i].word);
        }
        for (j = 0; j < wordIndexArray[i].lineNumbersCount - 1; j++) {
            printf("%d,", wordIndexArray[i].lineNumbersArray[j]);
        }
        /* Handle the last element separately to avoid trailing comma */
        /*printf("i = %d, j= %d\n", i, j);*/
        printf("%d", wordIndexArray[i].lineNumbersArray[j]);
        /*printf("wordIndexCount = %d\n", wordIndexCount);
        printf("wordIndexArraySize = %d\n", wordIndexArraySize);*/
        printf("\n");
    }

    freeWordIndexArray(wordIndexArray, &wordIndexArraySize);
    return 0;
}


void initWordIndex(WordIndex *wordIndex) {
    wordIndex->word = NULL;
    wordIndex->lineNumbersArray = NULL;
    wordIndex->lineNumbersSize = 0;
    wordIndex->lineNumbersCount = 0;
}


void freeWordIndex(WordIndex *wordIndex) {
    free(wordIndex->word);
    free(wordIndex->lineNumbersArray);
    wordIndex->lineNumbersArray = NULL;
    wordIndex->lineNumbersSize = 0;
    wordIndex->lineNumbersCount = 0;
}


int compareWords(const void *a, const void *b) {
    const WordIndex *wordIndexA = (const WordIndex *)a;
    const WordIndex *wordIndexB = (const WordIndex *)b;
    return strcmp(wordIndexA->word, wordIndexB->word);
}


void freeWordIndexArray(WordIndex *array, int *arraySize) {
    int i;
    for (i = 0; i < *arraySize; i++) {
        freeWordIndex(&array[i]);
    }
    free(array);
    array = NULL;
    *arraySize = 0;
}
