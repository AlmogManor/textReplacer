#include "stringFunctions.c"
#include "dictionary.c"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/* const is not needed, it is added for clarity */
const char* outputFileName = "_tempOut.txt";
const char* punctuationText = ".,()-!? ";

char* readFile(FILE* file) {
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    rewind(file);

    char* string = malloc(fsize + sizeof(char));
    fread(string, sizeof(char), fsize, file);
    fclose(file);

    string[fsize] = 0;

    return string;
}

int main(int argc, char const* argv[])
{

    /* to run:
    ./Main.exe TextFile.txt Synonyms.txt
    */


    /* argv[0] is path */
    if (argc != 3) {
        printf("Error: Wrong argument count\n");
        return 1;
    }

    const char* textFileName = "TextFile.txt";
    const char* dictFileName = "Synonyms.txt";

    /* open files */
    FILE* textFile = fopen(textFileName, "rb");
    if (textFile == NULL) {
        printf("Error: can't open the text file\n");
        return 2;
    }
    FILE* dictFile = fopen(dictFileName, "rb");
    if (dictFile == NULL) {
        printf("Error: can't open the dictionary file\n");
        return 2;
    }
    /* we don't directly edit the textFile to protect the data in case of a crash */
    FILE* outputFile = fopen(outputFileName, "wb+");
    if (outputFile == NULL) {
        printf("Error: can't create the output file\n");
        return 2;
    }

    /* build dictionary */
    Dictionary* dict = dictCreate();

    char* dictDataRaw = readFile(dictFile);
    char* dictDataFixed = fixNewline(dictDataRaw);
    free(dictDataRaw);

    char** dictDataLines = split(dictDataFixed, '\n');
    free(dictDataFixed);

    char* line = *dictDataLines;
    int lineNumber = 0;

    while (line != 0) {
        char** keyAndValues = split(line, '-');
        char** values = split(*(keyAndValues + 1), ',');

        stripArray(values);

        dictAdd(dict, strip(*keyAndValues), values);

        lineNumber++;
        line = *(dictDataLines + lineNumber);
        freeStringArray(keyAndValues);

    }
    freeStringArray(dictDataLines);

    /* work on the text file */
    srand((long)time(NULL));
    char* punctuation = (char*)malloc(strlen(punctuationText) + 1);
    memcpy(punctuation, punctuationText, strlen(punctuationText) + 1);

    char* wordPtr = readFile(textFile);
    char* wordStart = wordPtr;

    while (*wordPtr != 0) {
        while (equalsOne(*wordPtr, punctuation) == 0 && *wordPtr != 0) {
            wordPtr++;
        }
        char punctuation = *wordPtr;
        /* replace the punctuation with null for the dictGet and fprintf */
        *wordPtr = 0;

        Node* dictItem = dictGet(dict, wordStart);
        char* replacemnt;

        if (dictItem == NULL) {
            replacemnt = wordStart;
        }
        else {
            char** options = dictItem->values;
            char** optionsPtr = options;
            while (*optionsPtr != 0) {
                optionsPtr++;
            }

            replacemnt = *(options + (rand() % (optionsPtr - options)));
        }

        if (punctuation == 0) {
            fprintf(outputFile, "%s", replacemnt);
        }
        else {
            fprintf(outputFile, "%s%c", replacemnt, punctuation);
        }

        wordPtr++;
        wordStart = wordPtr;
    }

    /* cleanup */
    dictDestroy(dict);

    fclose(textFile);
    fclose(dictFile);
    fclose(outputFile);

    /* "move" the output to the original file */
    if (remove(textFileName)) {
        printf("Error: Can't delete original input file\n");
        return 3;
    }
    
    if (rename(outputFileName, textFileName)) {
        printf(outputFileName);
        printf("\n\n\n");
        printf(textFileName);
        printf("Error: Can't rename output file\n");
        return 4;
    }

    return 0;
}