#include "trie_hashage.h"
#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 256  // Taille du tableau de hachage

// Fonction de hachage
int hash(unsigned char key, int etat) {
    return (key * 31 + etat * 17) % HASH_SIZE;
}

// Fonction pour créer un Trie
Trie createTrieHash(int maxNode) {
    Trie T = (Trie)malloc(sizeof(struct _trie));
    if (T == NULL) {
        perror("Erreur d'allocation mémoire pour Trie");
        exit(EXIT_FAILURE);
    }
    
    T->maxNode = maxNode;
    T->nextNode = 1;  // Commence à partir de 1 (0 est la racine)

    T->transition = (List **)malloc(maxNode * sizeof(List *));
    if (T->transition == NULL) {
        perror("Erreur d'allocation mémoire pour transitions");
        free(T);
        exit(EXIT_FAILURE);
    }

    T->finite = (char *)malloc(maxNode * sizeof(char));
    if (T->finite == NULL) {
        perror("Erreur d'allocation mémoire pour finite");
        free(T->transition);
        free(T);
        exit(EXIT_FAILURE);
    }

    T->suffixLink = (int *)malloc(maxNode * sizeof(int));
    if (T->suffixLink == NULL) {
        perror("Erreur d'allocation mémoire pour suffixLink");
        free(T->finite);
        free(T->transition);
        free(T);
        exit(EXIT_FAILURE);
    }

    T->outputLink = (int *)malloc(maxNode * sizeof(int));
    if (T->outputLink == NULL) {
        perror("Erreur d'allocation mémoire pour outputLink");
        free(T->suffixLink);
        free(T->finite);
        free(T->transition);
        free(T);
        exit(EXIT_FAILURE);
    }

    // Initialisation des transitions et autres tableaux
    for (int i = 0; i < maxNode; i++) {
        T->transition[i] = NULL;  // Aucune transition au départ
        T->finite[i] = 0;          // Aucun état terminal
        T->suffixLink[i] = -1;     // Pas de lien suffixe initialisé
        T->outputLink[i] = -1;     // Pas de lien de sortie initialisé
    }
    return T;
}

// Fonction pour ajouter une transition dans le Trie
void addTransition(Trie T, int startNode, int targetNode, unsigned char letter) {
    int hashResult = hash(letter, startNode);
    List *newNode = (List *)malloc(sizeof(List));
    if (newNode == NULL) {
        perror("Erreur d'allocation mémoire pour newNode");
        exit(EXIT_FAILURE);
    }
    newNode->startNode = startNode;
    newNode->targetNode = targetNode;
    newNode->letter = letter;
    newNode->next = T->transition[hashResult];
    T->transition[hashResult] = newNode;
}

// Fonction pour insérer un motif dans le Trie
void insertPattern(Trie T, unsigned char *pattern) {
    int currentNode = 0;
    for (int i = 0; pattern[i] != '\0'; i++) {
        int hashResult = hash(pattern[i], currentNode);
        List *transition = T->transition[hashResult];

        // Cherche la transition existante
        while (transition != NULL && transition->letter != pattern[i]) {
            transition = transition->next;
        }

        // Si aucune transition existante, en créer une nouvelle
        if (transition == NULL) {
            transition = (List *)malloc(sizeof(List));
            if (transition == NULL) {
                perror("Erreur d'allocation mémoire pour transition");
                exit(EXIT_FAILURE);
            }
            transition->startNode = currentNode;
            transition->targetNode = T->nextNode++;
            transition->letter = pattern[i];
            transition->next = T->transition[hashResult];
            T->transition[hashResult] = transition;
            currentNode = transition->targetNode;
        } else {
            currentNode = transition->targetNode;
        }
    }
    T->finite[currentNode] = 1;  // Marquer le dernier état comme terminal
}