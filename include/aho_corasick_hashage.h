#ifndef AHO_HASHAGE_H
#define AHO_HASHAGE_H

#include "trie_hashage.h"

typedef struct {
    int *queue;
    int front;
    int back;
    int size;
} Queue;

// Déclaration des fonctions pour la gestion de la file circulaire
void initQueue(Queue *q, int size);
void enfiler(Queue *q, int value);
int defiler(Queue *q);

// Autres déclarations de fonctions
void buildSuffixLinks(Trie T);
int searchPatterns(Trie T, unsigned char *text);

#endif
