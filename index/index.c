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
    FILE *file;
    int lineNumber;
    int i;
    int j;

    printf("Enter the name of the file to index: ");
    scanf("%s", filename);
    printf("You entered: %s\n", filename);

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
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
        printf("%s: ", wordIndex[i].word);
        for (j = 0; j < wordIndex[i].count; j++) {
            printf("%d ", wordIndex[i].lineNumbers[j]);
        }
        printf("\n");
    }

    return 0;
}
