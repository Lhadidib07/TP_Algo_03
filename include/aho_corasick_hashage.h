#ifndef AHO_CORASICK_HASHAGE_H
#define AHO_CORASICK_HASHAGE_H

#include "trie_hashage.h"

void initialiserFile(int **queue, int *queueStart, int *queueEnd, int maxNode);
void mettreAJourLienSortie(Trie T, int targetNode);
int trouverTransition(Trie T, int currentNode, unsigned char letter);
void mettreAJourLienSuffixe(Trie T, int currentNode, int targetNode, unsigned char letter);
void traiterNoeud(Trie T, int *queue, int *queueEnd, int currentNode);
void traiterFile(Trie T, int *queue, int *queueStart, int *queueEnd);
void buildSuffixLinks(Trie T);
int searchPatterns(Trie T, unsigned char *text);

#endif // AHO_CORASICK_HASHAGE_H