#include "trie_matrice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialisation du Trie
void createTrie(Trie *trie) {
    trie->maxNode = 1000;
    trie->nextNode = 1;  // Commencer à partir de 1, 0 est la racine
    trie->transition = (int **)malloc(trie->maxNode * sizeof(int *));
    trie->fail = (int *)malloc(trie->maxNode * sizeof(int));
    trie->finite = (char *)malloc(trie->maxNode * sizeof(char));

    // Initialiser les matrices de transitions et fail
    for (int i = 0; i < trie->maxNode; i++) {
        trie->transition[i] = (int *)malloc(ALPHABET_SIZE * sizeof(int));
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            trie->transition[i][j] = -1;
        }
        trie->fail[i] = 0;
        trie->finite[i] = 0;
    }
}

// Insérer un mot dans le Trie
void insertInTrie(Trie *trie, const char *word) {
    int currentNode = 0;
    while (*word) {
        unsigned char c = (unsigned char)(*word);
        if (trie->transition[currentNode][c] == -1) {
            trie->transition[currentNode][c] = trie->nextNode++;
        }
        currentNode = trie->transition[currentNode][c];
        word++;
    }
    trie->finite[currentNode] = 1;  // Marquer l'état comme terminal
}
