#ifndef TRIE_HASHAGE_H
#define TRIE_HASHAGE_H

#include <stdlib.h>

#define HASH_SIZE 256
#define MAX_WORD_LENGTH 100

typedef struct List {
    int startNode;
    int targetNode;
    unsigned char letter;
    struct List *next;
} List;

// Structure du Trie
typedef struct _trie {
    int maxNode;
    int nextNode;
    List **transition;   // Tableau de listes pour gérer les transitions
    char *finite;       // Indique si un état est terminal
    int *suffixLink;    // Liens suffixes
    int *outputLink;    // Liens de sortie
} *Trie;

int hash(unsigned char key, int etat);
Trie createTrieHash(int maxNode);
void addTransition(Trie T, int startNode, int targetNode, unsigned char letter);
void insertPattern(Trie T, unsigned char *pattern);
#endif


