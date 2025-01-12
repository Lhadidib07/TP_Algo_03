#include "aho_corasick_matrice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256
// Initialisation de la file circulaire
void initQueue(Queue *q, int size) {
    q->queue = (int *)malloc(size * sizeof(int));
    q->front = 0;
    q->back = 0;
    q->size = size;
}

// Enfiler (ajouter un élément dans la file)
void enfiler(Queue *q, int value) {
    q->queue[q->back] = value;
    q->back = (q->back + 1) % q->size; // pour garantir que notre file ai toujours une taille de 0 a size-1
}

// Défiler (retirer un élément de la file)
int defiler(Queue *q) {
    int value = q->queue[q->front];
    q->front = (q->front + 1) % q->size;
    return value;
}

// Recherche dans le texte pour compter les occurrences des mots
int searchText(Trie *trie, const char *text) {
    int currentNode = 0;
    int count = 0;

    while (*text) {
        unsigned char c = (unsigned char)(*text);

        // Suivre les transitions et les fail states
        while (trie->transition[currentNode][c] == -1) {
            currentNode = trie->fail[currentNode];  // Suivre le lien "fail"
            if (currentNode == 0) {
                break;  // Retour à la racine si aucun état valide
            }
        }

        if (trie->transition[currentNode][c] != -1) {
            currentNode = trie->transition[currentNode][c];  // Suivre la transition valide
        }

        // Vérifier les états terminaux (mot complet trouvé)
        int tempNode = currentNode;
        while (tempNode != 0) {
            if (trie->finite[tempNode]) {
                count++;
            }
            tempNode = trie->fail[tempNode];  // Suivre le lien "fail" pour vérifier les occurrences
        }

        text++;  // Passer au caractère suivant du texte
    }

    return count;
}
// Fonction pour construire les liens suffixes (fail links)


void buildSuffixLinks(Trie *trie) {
    Queue q;
    initQueue(&q, trie->maxNode);

    // Initialiser les liens suffixes pour les transitions de la racine
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (trie->transition[0][i] != -1) {
            trie->fail[trie->transition[0][i]] = 0;
            enfiler(&q, trie->transition[0][i]);
        }
    }

    // Parcours en largeur pour calculer les liens suffixes
    while (q.front != q.back) {
        int currentNode = defiler(&q);
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (trie->transition[currentNode][i] != -1) {
                int failState = trie->fail[currentNode];
                while (trie->transition[failState][i] == -1 && failState != 0) {
                    failState = trie->fail[failState];
                }
                if (trie->transition[failState][i] != -1) {
                    trie->fail[trie->transition[currentNode][i]] = trie->transition[failState][i];
                } else {
                    trie->fail[trie->transition[currentNode][i]] = 0;
                }
                enfiler(&q, trie->transition[currentNode][i]);
            }
        }
    }
    free(q.queue);
}
