#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include "trie_matrice.h"

// Déclaration de la structure pour la file 
typedef struct {
    int *queue;
    int front;
    int back;
    int size;
} Queue;

// Déclaration des fonctions pour la file 

void initQueue(Queue *q, int size);
void enfiler(Queue *q, int value);
int defiler(Queue *q) ;

// Déclaration de la fonction de recherche de l'algo aho-corasick
int searchText(Trie *trie, const char *text);
void buildSuffixLinks(Trie *trie);

#endif // AHO_CORASICK_H
