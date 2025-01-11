#ifndef TRIE_HASHAGE_H
#define TRIE_HASHAGE_H

#include <stdlib.h>

#define HASH_SIZE 256
#define MAX_WORD_LENGTH 100

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

int hash(unsigned char key, int etat);
Trie createTrieHash(int maxNode);
void addTransition(Trie T, int startNode, int targetNode, unsigned char letter);
void insertPattern(Trie T, unsigned char *pattern);
#endif
