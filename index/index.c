/**
* TODO - 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORDS_BUFFER 10
#define LINES_BUFFER 10

typedef struct {
    char word[100];
    int *lineNumbersArray;
    int lineNumbersSize;
    int lineNumbersCount;
} WordIndex;

void initWordIndex(WordIndex *WordIndex);
void freeWordIndex(WordIndex *array);
int compareWords(const void *a, const void *b);
void freeWordIndexArray(WordIndex *array, int *arraySize);


int main() {
    char filename[100];
    char line[1000];
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
                    for (i = wordIndexCount; i < wordIndexArraySize; i++) {
                        initWordIndex(&wordIndexArray[i]);
                    }
                }
                strcpy(wordIndexArray[i].word, word);
                wordIndexArray[i].lineNumbersSize = LINES_BUFFER;
                wordIndexArray[i].lineNumbersArray = malloc(wordIndexArray[i].lineNumbersSize * sizeof(int));
                if (wordIndexArray[i].lineNumbersArray == NULL) {
                    fprintf(stderr, "Error: Failed to allocate memory for line numbers.\n");
                    freeWordIndexArray(wordIndexArray, &wordIndexArraySize);  /* Free the originally allocated memory */
                    fclose(file);
                    return 1;
                }
                wordIndexArray[i].lineNumbersArray[0] = lineNumber;
                wordIndexArray[i].lineNumbersCount++;
                wordIndexCount++;
            }
            word = strtok(NULL, " \t\n");
        }
        lineNumber++;
    }

    fclose(file);

    qsort(wordIndexArray, wordIndexCount, sizeof(WordIndex), compareWords);

    for (i = 0; i < wordIndexCount; i++) {
        if (wordIndexArray[i].lineNumbersCount== 1) {
            printf("%-8s appears in line ", wordIndexArray[i].word);
        } else {
            printf("%-8s appears in lines ", wordIndexArray[i].word);
        }
        for (j = 0; j < wordIndexArray[i].lineNumbersCount - 1; j++) {
            printf("%d,", wordIndexArray[i].lineNumbersArray[j]);
        }
        /* Handle the last element separately to avoid trailing comma */
        printf("%d", wordIndexArray[i].lineNumbersArray[j]);
        printf("\n");
    }

    freeWordIndexArray(wordIndexArray, &wordIndexArraySize);
    return 0;
}


void initWordIndex(WordIndex *WordIndex) {
    strcpy(WordIndex->word, "");  /* Initialize the word to an empty string */
    WordIndex->lineNumbersArray = NULL;
    WordIndex->lineNumbersSize = 0;
    WordIndex->lineNumbersCount = 0;
}


void freeWordIndex(WordIndex *WordIndex) {
    free(WordIndex->lineNumbersArray);
    WordIndex->lineNumbersArray = NULL;
    WordIndex->lineNumbersSize = 0;
    WordIndex->lineNumbersCount = 0;
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




