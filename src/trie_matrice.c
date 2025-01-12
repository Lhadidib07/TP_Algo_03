#include "trie_matrice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256

// Initialisation du Trie
void createTrie(Trie *trie) {
    trie->maxNode = 1000;
    trie->nextNode = 1;  // Commencer à partir de 1, 0 est la racine

    trie->transition = (int **)malloc(trie->maxNode * sizeof(int *));
    if (trie->transition == NULL) {
        perror("Erreur d'allocation mémoire pour transition");
        exit(EXIT_FAILURE);
    }

    trie->fail = (int *)malloc(trie->maxNode * sizeof(int));
    if (trie->fail == NULL) {
        perror("Erreur d'allocation mémoire pour fail");
        free(trie->transition);
        exit(EXIT_FAILURE);
    }

    trie->finite = (char *)malloc(trie->maxNode * sizeof(char));
    if (trie->finite == NULL) {
        perror("Erreur d'allocation mémoire pour finite");
        free(trie->transition);
        free(trie->fail);
        exit(EXIT_FAILURE);
    }

    // Initialiser les matrices de transitions et fail
    for (int i = 0; i < trie->maxNode; i++) {
        trie->transition[i] = (int *)malloc(ALPHABET_SIZE * sizeof(int));
        if (trie->transition[i] == NULL) {
            perror("Erreur d'allocation mémoire pour transition[i]");
            for (int k = 0; k < i; k++) {
                free(trie->transition[k]);
            }
            free(trie->transition);
            free(trie->fail);
            free(trie->finite);
            exit(EXIT_FAILURE);
        }
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
            if (trie->nextNode >= trie->maxNode) {
                fprintf(stderr, "Erreur : dépassement de la capacité du Trie\n");
                exit(EXIT_FAILURE);
            }
            trie->transition[currentNode][c] = trie->nextNode++;
        }
        currentNode = trie->transition[currentNode][c];
        word++;
    }
    trie->finite[currentNode] = 1;  // Marquer l'état comme terminal
}

// Libérer la mémoire allouée pour le Trie
void freeTrie(Trie *trie) {
    for (int i = 0; i < trie->maxNode; i++) {
        free(trie->transition[i]);
    }
    free(trie->transition);
    free(trie->fail);
    free(trie->finite);
}