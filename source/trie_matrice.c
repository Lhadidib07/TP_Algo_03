//trie_matrice.c
#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Trie* createTrie(int maxNode) {
    // Allouer de la mémoire pour un pointeur de type Trie
    Trie* newTrie = (Trie*)malloc(sizeof(Trie));
    
    if (newTrie == NULL) {
        // Erreur si l'allocation échoue
        return NULL;
    }

    // Initialiser les membres de la structure
    newTrie->maxNode = maxNode;
    newTrie->nextNode = 0;
    
    // Allocation de la matrice de transition (tableau de tableaux)
    newTrie->transition = (int**)malloc(maxNode * sizeof(int*));
    if (newTrie->transition == NULL) {
        free(newTrie); // Libérer la mémoire allouée pour Trie en cas d'erreur
        return NULL;
    }

    // Initialisation des transitions avec -1 (aucune transition par défaut)
    for (int i = 0; i < maxNode; i++) {
        newTrie->transition[i] = (int*)malloc(256 * sizeof(int));  // Supposons qu'il y a 256 caractères possibles
        if (newTrie->transition[i] == NULL) {
            // Si l'allocation échoue, libérer tout ce qui a été alloué jusqu'ici
            for (int j = 0; j < i; j++) {
                free(newTrie->transition[j]);
            }
            free(newTrie->transition);
            free(newTrie);
            return NULL;  // Retourner NULL en cas d'échec d'allocation
        }

        // Initialiser chaque transition à -1 (indique aucune transition)
        for (int j = 0; j < 256; j++) {
            newTrie->transition[i][j] = -1;  // -1 signifie aucune transition existante
        }
    }
    
    // Initialisation de 'finite' (les états finaux)
    newTrie->finite = (char*)malloc(maxNode * sizeof(char));
    if (newTrie->finite == NULL) {
        // Si l'allocation échoue, libérer tout ce qui a été alloué jusqu'ici
        for (int i = 0; i < maxNode; i++) {
            free(newTrie->transition[i]);
        }
        free(newTrie->transition);
        free(newTrie);
        return NULL;
    }
    
    // Initialiser tous les éléments de 'finite' à 0 (non terminal)
    for (int i = 0; i < maxNode; i++) {
        newTrie->finite[i] = 0;  // 0 signifie que l'état n'est pas terminal
    }

    // Retourner le pointeur vers le Trie créé
    return newTrie;
}

   void libererTrie(Trie* trie){
    if(trie != NULL){
        // Libérer chaque ligne de la matrice de transition
        for(int i = 0; i < trie->maxNode; i++){
            free(trie->transition[i]);
        }
        free(trie->transition);

        // Libérer la mémoire de la liste 'finite'
        free(trie->finite);

        // Libérer la mémoire de la structure Trie elle-même
        free(trie);
    }
}

