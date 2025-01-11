#include "trie_hashage.h"

int hash(unsigned char key, int etat) {
    return (key * 31 + etat * 17) % HASH_SIZE;
}

Trie createTrieHash(int maxNode) {
    Trie T = (Trie)malloc(sizeof(struct _trie));
    T->maxNode = maxNode;
    T->nextNode = 1;
    T->transition = (List **)malloc(maxNode * sizeof(List *));
    T->finite = (char *)malloc(maxNode * sizeof(char));
    T->suffixLink = (int *)malloc(maxNode * sizeof(int));
    T->outputLink = (int *)malloc(maxNode * sizeof(int));
    for (int i = 0; i < maxNode; i++) {
        T->transition[i] = NULL;
        T->finite[i] = 0;
        T->suffixLink[i] = -1;
        T->outputLink[i] = -1;
    }
    return T;
}

void addTransition(Trie T, int startNode, int targetNode, unsigned char letter) {
    int hashResult = hash(letter, startNode);
    List *newNode = (List *)malloc(sizeof(List));
    newNode->startNode = startNode;
    newNode->targetNode = targetNode;
    newNode->letter = letter;
    newNode->next = T->transition[hashResult];
    T->transition[hashResult] = newNode;
}

void insertPattern(Trie T, unsigned char *pattern) {
    int currentNode = 0;
    for (int i = 0; pattern[i] != '\0'; i++) {
        int hashResult = hash(pattern[i], currentNode);
        List *transition = T->transition[hashResult];
        while (transition != NULL && transition->letter != pattern[i]) {
            transition = transition->next;
        }
        if (transition == NULL) {
            T->transition[hashResult] = (List *)malloc(sizeof(List));
            T->transition[hashResult]->startNode = currentNode;
            T->transition[hashResult]->targetNode = T->nextNode++;
            T->transition[hashResult]->letter = pattern[i];
            T->transition[hashResult]->next = NULL;
            currentNode = T->transition[hashResult]->targetNode;
        } else {
            currentNode = transition->targetNode;
        }
    }
    T->finite[currentNode] = 1;
}
