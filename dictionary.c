#include "dictionary.h"
#include "stringFunctions.h"
#include <stdio.h>
#include <malloc.h>

/*
    Retrieves an item from the dictionary, using the Node* you can get both the key and the values.
    If the key doesn't appear in the list, NULL will be returned.
    Args:
        Dictionary* dict - pointer to the dictionary to sift through.
        char* key - the key to search.
    Returns:
        Node* - the node containing the keys and the values OR NULL if the key isn't contained in the dictionary.
*/
Node* dictGet(Dictionary* dict, char* key) {
    Node* node = dict->first;
    while (node != NULL) {
        if (compareStrings(node->key, key) == 0) {
            return node;
        }

        node = node->next;
    }

    return NULL;
}

/*
    Internal function.
    Returns the node that comes before the node that contains the key,
    the value of exists will be set to 1.

    In the case that there isn't a node before the desired node (e.g: the desired node is the first one),
    NULL will be returned, and the value of exists will be set to 1.

    In the case that there isn't a node matching the key in the dictionary,
    NULL will be returned and exists will be set to 0.

    This is used for node deletion.
    Args:
        Dictionary* dict - pointer to the dictionary to sift through.
        char* key - the key to search.
        int* exists - a pointer to an int which will be used as a flag to indicate if the key exists in the dictionary.
    Return:
        Node*, *exists - read function description.
*/
Node* dictGetPrev(Dictionary* dict, char* key, int* exists) {
    /* there isn't a previous node, but the node with the key does exist
    (as opposed to the key not appearing in the dictionary at all)*/
    if (compareStrings(dict->first->key, key) == 0) {
        *exists = 1;
        return NULL;
    }

    Node* node = dict->first;
    while (node->next != NULL) {
        if (compareStrings(node->next->key, key) == 0) {
            *exists = 1;
            return node;
        }

        node = node->next;
    }

    /* key doesn't appear in the dictionary */
    *exists = 0;
    return NULL;
}

/*
    Creates a dictionary.
    Args:
        -
    Return:
        Dictionary* - pointer to an empty dictionary
*/
Dictionary* dictCreate() {
    Dictionary* dict = (Dictionary*)malloc(sizeof(Dictionary));

    dict->first = NULL;
}

/*
    Deletes a dictionary, this does free the memory used by each node, however,
    this DOES NOT free the memory taken up by the key and values inside the node.
    Args:
        Dictionary* dict - pointer to the dictionary to delete.
    Return:
        -
*/
void dictDelete(Dictionary* dict) {
    Node* head = dict->first;
    Node* next;
    /*
     we are not the ones who allocated the memory for the key or the values
     and therefore we are not the ones responsible to free them
     */
    while (head != NULL) {
        next = head->next;
        free(head);
        head = next;
    }

    free(dict);
}

/*
    Deletes a dictionary, this frees the memory used by each node AND
    the memory taken up by the key and values inside the node.
    Args:
        Dictionary* dict - pointer to the dictionary to delete.
    Return:
        -
*/
void dictDestroy(Dictionary* dict) {
    Node* head = dict->first;
    Node* next;

    while (head != NULL) {
        next = head->next;
        free(head->key);
        freeStringArray(head->values);
        free(head);
        head = next;
    }

    free(dict);
}


/*
    Adds and item to the dictionary.
    Args:
        Dictionary* dict - the dictionary to add the item to.
        char* key - the items' key.
        char** - the items' array of values.
    Return:
        int - 0 if addition was successful, 1 if an error occurred (e.g: the key was already in the list).
*/
int dictAdd(Dictionary* dict, char* key, char** values) {
    /* key is not already in the dictionary */
    if (dictGet(dict, key) == 0) {
        Node* newNode = (Node*)malloc(sizeof(Node));

        newNode->key = key;
        newNode->values = values;
        newNode->next = dict->first;

        dict->first = newNode;

        return 0;
    }
    else {
        /* key is in the dictionary, return error */
        return 1;
    }
}

/*
    Removes an item from the dictionary.
    Args:
        Dictionary* dict - the dictionary to remove the item from.
        char* key - the key of the item to remove.
    Return:
        int - 0 if the item was removed, 1 if the item didn't exist.
*/
int dictRemove(Dictionary* dict, char* key) {
    int exists = 0;
    Node* prev = dictGetPrev(dict, key, &exists);

    if (exists) {
        if (prev == NULL) {
            Node* markedForDeletion = dict->first;

            dict->first = dict->first->next;

            free(markedForDeletion);
        }
        else {
            Node* markedForDeletion = prev->next;

            prev->next = markedForDeletion->next;

            free(markedForDeletion);
        }

        return 0;
    }
    else {
        /* key doesn't exists, return error */
        return 1;
    }
}

/*
    Prints a dictionary.
    Args:
        Dictionary* dict - the dictionary to print.
    Returns:
        -
*/
void dictPrint(Dictionary* dict) {
    Node* head = dict->first;

    printf("Dictionary-----------\n");
    while (head != NULL) {
        nodePrint(head);
        printf("\n");
        head = head->next;
    }
    printf("---------------------\n");
}

/*
    Prints a node.
    Args:
        Node* node - the node to print.
    Returns:
        -
*/
void nodePrint(Node* node) {
    printf("Key:");
    printf(node->key);

    printf("|Values:");
    char** values = node->values;

    /* by changing the condition of the while loop to values+1
    we can avoid the extra comma at the end of the print */
    while (*(values + 1) != NULL) {
        printf(*values);
        printf(",");
        values++;
    }

    printf(*values);
}