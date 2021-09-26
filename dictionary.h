typedef struct node {
    struct node* next;

    char* key;
    char** values;
} Node;

typedef struct dictionary {
    Node* first;
} Dictionary;

Dictionary* dictCreate();
void dictDelete(Dictionary* dict);
void dictDestroy(Dictionary* dict);

Node* dictGet(Dictionary* dict, char* key);

int dictAdd(Dictionary* dict, char* key, char** values);
int dictRemove(Dictionary* dict, char* key);

void dictPrint(Dictionary* dict);
void nodePrint(Node* node);