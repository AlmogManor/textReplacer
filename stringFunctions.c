#include "stringFunctions.h"
#include <malloc.h>
#include <stdio.h>

/*
    Splits a string by a given character.
    Args:
        char* string - the string to split, null terminated.
        char splitChar - the character to split by.
    Return:
        char** - an array containing the splitted strings, both the array and the strings are null terminated.

*/
char** split(char* string, char splitChar) {
    /* calculate array size and initialize it */
    int partCount = 1;

    char* stringPointer = string;
    while (*stringPointer != 0) {
        if (*stringPointer == splitChar) {
            partCount++;
        }
        stringPointer++;
    }

    char** splittedStrings = (char**)malloc((partCount + 1) * sizeof(char*));
    char** splittedStringsPtr = splittedStrings;

    /* copy the splitted strings into the array */
    char* arrayLine;
    int stringLength;

    for (int i = 0; i < partCount; ++i) {
        stringLength = 0;

        char* lineStart = string;

        while (*string != splitChar && *string != 0) {
            string++;
            stringLength++;
        }
        string++; /* skip the splitChar and the null termination */

        arrayLine = *splittedStringsPtr = (char*)malloc(stringLength + 1);

        while (*lineStart != 0 && *lineStart != splitChar) {
            *arrayLine = *lineStart;

            lineStart++;
            arrayLine++;
        }
        *arrayLine = 0;

        splittedStringsPtr++;
    }

    /* terminate the string array */
    *splittedStringsPtr = 0;

    return splittedStrings;
}

/*
    Frees the memory taken up by an array (char**) of strings.
    Args:
        char** arr - the array to free, both the array and the strings themselves must be null terminated.
*/
void freeStringArray(char** arr) {
    while (*arr != 0) {
        free(*arr);
        arr++;
    }

    free(arr);
}

/*
    Compares two strings and returns which one should come first in a dictionary (capitalization is ignored).
    Args:
        char* string1 - the first string to compare, null terminated.
        char* string2 - the first string to compare, null terminated.
    Return:
        int - string1 first/string2 first/equal = 1/-1/0.
*/
int compareStrings(char* string1, char* string2) {
    while (*string1 != 0 && *string2 != 0) {
        /* we do | 0x20 to make sure that the letter is lower case (ascii) */
        char lowerChar1 = *(string1) | 0x20;
        char lowerChar2 = *(string2) | 0x20;

        if (lowerChar1 > lowerChar2) {
            return -1;
        }
        else if (lowerChar1 < lowerChar2) {
            return 1;
        }

        string1++;
        string2++;
    }

    /* string 1 is longer */
    if (*string1 != 0) {
        return -1;
    }

    /* string 2 is longer */
    if (*string2 != 0) {
        return 1;
    }

    /* both strings are the same length */
    return 0;
}

//! this is broken (it edits the original string)
/*
    Removes spaces from the beginning and the end of a string,
    returns a pointer to a newly allocated space. 
    Args:
        char* string - null terminate string to strip of ' ' (0x20).
    Return:
        char* - pointer to the string stripped of space, with new memory allocated.
*/
char* strip(char* string) {
    char* stringStart = string;
    char* stringPtr = string;


    while (*string == ' ') {
        string++;
    }

    while (*string != 0) {
        *stringPtr = *string;
        stringPtr++;
        string++;
    }
    /* copy the null at the end */
    *stringPtr = *string;

    /* pointed to the null at the of the string */
    stringPtr--;

    while (*stringPtr == ' ') {
        stringPtr--;
    }

    stringPtr++;
    *stringPtr = 0;

    char* strippedString = malloc(stringPtr - stringStart + sizeof(char));
    char* strippedStringPtr = strippedString;

    while (*stringStart != 0) {
        *strippedStringPtr = *stringStart;
        stringStart++;
        strippedStringPtr++;
    }
    *strippedStringPtr = *stringStart;

    return strippedString;
}

void stripArray(char** array) {
    while (*array != 0) {
        char* unstriped = *array;
        *array = strip(*array);
        free(unstriped);
        array++;
    }
}

int equalsOne(char letter, char* letters) {
    while (*letters != 0) {
        if (letter == *letters) {
            return 1;
        }
        letters++;
    }

    return 0;
}

char* fixNewline(char* string) {
    char* stringPtr = string;
    int nlOccurrence = 0;

    while (*stringPtr != 0) {
        if (*stringPtr == 0x0D && *(stringPtr + 1) == 0x0A) {
            nlOccurrence++;
        }
        stringPtr++;
    }

    char* fixedString = (char*)malloc(sizeof(char) * (stringPtr - string - nlOccurrence + 1));
    char* fixedStringPtr = fixedString;

    stringPtr = string;
    int counter = 0;
    while (*stringPtr != 0) {
        if (*stringPtr == 0x0D && *(stringPtr + 1) == 0x0A) {
            *fixedStringPtr = 0x0A;
            stringPtr++;
        }
        else {
            *fixedStringPtr = *stringPtr;
        }

        fixedStringPtr++;
        stringPtr++;
        counter++;
    }
    *fixedStringPtr = 0;

    return fixedString;
}