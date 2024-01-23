/**
* TODO - 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 1000

typedef struct {
    char word[100];
    int lineNumbers[MAX_WORDS];
    int count;
} WordIndex;

int compareWords(const void *a, const void *b) {
    const WordIndex *wordIndexA = (const WordIndex *)a;
    const WordIndex *wordIndexB = (const WordIndex *)b;
    return strcmp(wordIndexA->word, wordIndexB->word);
}

int main() {
    char filename[100];
    char line[100];
    WordIndex wordIndex[MAX_WORDS];
    int wordCount = 0;
    char *token;
    FILE *file;
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

    lineNumber = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        char *word = strtok(line, " \t\n");
        while (word != NULL) {
            for (i = 0; i < wordCount; i++) {
                if (strcmp(wordIndex[i].word, word) == 0) {
                    wordIndex[i].lineNumbers[wordIndex[i].count++] = lineNumber;
                    break;
                }
            }
            if (i == wordCount) {
                strcpy(wordIndex[wordCount].word, word);
                wordIndex[wordCount].lineNumbers[wordIndex[wordCount].count++] = lineNumber;
                wordCount++;
            }
            word = strtok(NULL, " \t\n");
        }
        lineNumber++;
    }

    fclose(file);

    qsort(wordIndex, wordCount, sizeof(WordIndex), compareWords);

    for (i = 0; i < wordCount; i++) {
        if (wordIndex[i].count == 1) {
        printf("%-8s appears in line ", wordIndex[i].word);
        } else {
        printf("%-8s appears in lines ", wordIndex[i].word);
        }
        for (j = 0; j < wordIndex[i].count - 1; j++) {
            printf("%d,", wordIndex[i].lineNumbers[j]);
        }
        /* Handle the last element separately to avoid trailing comma */
        printf("%d", wordIndex[i].lineNumbers[wordIndex[i].count - 1]);
        printf("\n");
    }

    return 0;
}
