#ifndef AHO_HASHAGE_H
#define AHO_HASHAGE_H

#include "trie_hashage.h"

void initialiserFile(int **queue, int *queueStart, int *queueEnd, int maxNode);
void mettreAJourLienSortie(Trie T, int targetNode);
int trouverTransition(Trie T, int currentNode, int letter);
void mettreAJourLienSuffixe(Trie T, int currentNode, int targetNode, int letter);
void traiterNoeud(Trie T, int *queue, int *queueStart, int *queueEnd, int currentNode);
void traiterFile(Trie T, int *queue, int *queueStart, int *queueEnd);
void buildSuffixLinks(Trie T);
int searchPatterns(Trie T, unsigned char *text);
#endif
