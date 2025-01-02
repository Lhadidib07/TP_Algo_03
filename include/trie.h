#ifndef TRIE_H
#define TRIE_H

#include "trie_matrice.h"

/**
 * Crée un trie avec un nombre maximal de noeuds.
 */
Trie* createTrie(int maxNode);

/**
 * Insère un mot dans le trie.
 */
void insertInTrie(Trie* trie, unsigned char* w);

/**
 * Vérifie si un mot existe dans le trie.
 */
int isInTrie(Trie* trie, unsigned char* w);

/**
 * Récupère la transition d'un noeud pour un caractère donné.
 */
int getTransition(Trie* trie, int startNode, unsigned char w);

void libererTrie(Trie* trie);

#endif // TRIE_H
