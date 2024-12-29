#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HASH_SIZE 256

typedef struct _list {
    int startNode;
    int targetNode;
    unsigned char letter;
    struct _list *next;
} List;

typedef struct _trie {
    int maxNode;
    int nextNode;
    List **transition;
    char *finite;
    int *suffixLink;
    int *outputLink;
} *Trie;

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

void buildSuffixLinks(Trie T) {
    int *queue = (int *)malloc(T->maxNode * sizeof(int));
    int queueStart = 0;
    int queueEnd = 0;
    queue[queueEnd++] = 0;
    while (queueStart < queueEnd) {
        int currentNode = queue[queueStart++];
        for (int i = 0; i < HASH_SIZE; i++) {
            int targetNode = -1;
            List *transition = T->transition[hash(i, currentNode)];
            while (transition != NULL) {
                if (transition->letter == i) {
                    targetNode = transition->targetNode;
                    break;
                }
                transition = transition->next;
            }
            if (targetNode != -1) {
                if (currentNode == 0) {
                    T->suffixLink[targetNode] = 0;
                } else {
                    int suffixNode = T->suffixLink[currentNode];
                    while (suffixNode != -1) {
                        List *suffixTransition = T->transition[hash(i, suffixNode)];
                        while (suffixTransition != NULL) {
                            if (suffixTransition->letter == i) {
                                T->suffixLink[targetNode] = suffixTransition->targetNode;
                                break;
                            }
                            suffixTransition = suffixTransition->next;
                        }
                        if (T->suffixLink[targetNode] != -1) {
                            break;
                        }
                        suffixNode = T->suffixLink[suffixNode];
                    }
                    if (suffixNode == -1) {
                        T->suffixLink[targetNode] = 0;
                    }
                }
                if (T->finite[T->suffixLink[targetNode]]) {
                    T->outputLink[targetNode] = T->suffixLink[targetNode];
                } else {
                    T->outputLink[targetNode] = T->outputLink[T->suffixLink[targetNode]];
                }
                queue[queueEnd++] = targetNode;
            }
        }
    }
}

void searchPatterns(Trie T, unsigned char *text) {
    int currentNode = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        while (currentNode != -1 && T->transition[hash(text[i], currentNode)] == NULL) {
            currentNode = T->suffixLink[currentNode];
        }
        if (currentNode == -1) {
            currentNode = 0;
        } else {
            currentNode = T->transition[hash(text[i], currentNode)]->targetNode;
        }
        int outputNode = currentNode;
        while (outputNode != -1) {
            if (T->finite[outputNode]) {
                printf("Pattern found at index %d\n", i);
            }
            outputNode = T->outputLink[outputNode];
        }
    }
}

int main() {
    Trie T = createTrieHash(1000);
    insertPattern(T, (unsigned char *)"he");
    insertPattern(T, (unsigned char *)"she");
    insertPattern(T, (unsigned char *)"his");
    insertPattern(T, (unsigned char *)"hers");
    buildSuffixLinks(T);
    searchPatterns(T, (unsigned char *)"ahishers");
    return 0;
}