#include "aho_corasick_hashage.h"
#include <stdlib.h>
#include <stdio.h>

void initialiserFile(int **queue, int *queueStart, int *queueEnd, int maxNode) {
    *queue = (int *)malloc(maxNode * sizeof(int));
    *queueStart = 0;
    *queueEnd = 0;
    (*queue)[(*queueEnd)++] = 0;
}

void mettreAJourLienSortie(Trie T, int targetNode) {
    if (T->finite[T->suffixLink[targetNode]]) {
        T->outputLink[targetNode] = T->suffixLink[targetNode];
    } else {
        T->outputLink[targetNode] = T->outputLink[T->suffixLink[targetNode]];
    }
}

int trouverTransition(Trie T, int currentNode, int letter) {
    List *transition = T->transition[hash(letter, currentNode)];
    while (transition != NULL) {
        if (transition->letter == letter) {
            return transition->targetNode;
        }
        transition = transition->next;
    }
    return -1;
}

void mettreAJourLienSuffixe(Trie T, int currentNode, int targetNode, int letter) {
    // Si le noeud courant est la racine alors le lien de suffixe est 0
    if (currentNode == 0) {
        T->suffixLink[targetNode] = 0;
    } else {
        // Sinon, chercher le lien de suffixe du noeud courant
        int suffixNode = T->suffixLink[currentNode];
        while (suffixNode != -1) {
            int suffixTarget = trouverTransition(T, suffixNode, letter);
            if (suffixTarget != -1) {
                T->suffixLink[targetNode] = suffixTarget;
                break;
            }
            suffixNode = T->suffixLink[suffixNode];
        }
        if (suffixNode == -1) {
            T->suffixLink[targetNode] = 0;
        }
    }
}


void traiterNoeud(Trie T, int *queue, int *queueStart, int *queueEnd, int currentNode) {
    // Parcourir les transitions du noeud
    for (int i = 0; i < HASH_SIZE; i++) { // parcourir toutes la table de hachage 
        int targetNode = trouverTransition(T, currentNode, i);
        // Si la transition existe alors mettre à jour les liens de suffixe et de sortie 
        if (targetNode != -1) {
            mettreAJourLienSuffixe(T, currentNode, targetNode, i);
            mettreAJourLienSortie(T, targetNode);
            queue[(*queueEnd)++] = targetNode;
        }
    }
}



void traiterFile(Trie T, int *queue, int *queueStart, int *queueEnd) {

    // Parcourir la file
    // tant que la file n'est pas fini
    while (*queueStart < *queueEnd) {
        // Prendre le premier noeud de la file
        int currentNode = queue[(*queueStart)++];
        // Traiter le noeud , mettre à jour les liens 
        traiterNoeud(T, queue, queueStart, queueEnd, currentNode);
    }
}





void buildSuffixLinks(Trie T) {
    int *queue;
    int queueStart, queueEnd;
    initialiserFile(&queue, &queueStart, &queueEnd, T->maxNode);
    traiterFile(T, queue, &queueStart, &queueEnd);
    free(queue);
}

int searchPatterns(Trie T, unsigned char *text) {
    int currentNode = 0;
    int foundCount = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        while (currentNode != -1 && T->transition[hash(text[i], currentNode)] == NULL) {
            currentNode = T->suffixLink[currentNode];
        }
        if (currentNode == -1) {
            currentNode = 0;
        } else {
            currentNode = T->transition[hash(text[i], currentNode)]->targetNode;
        }
        int outputNode = currentNode;
        while (outputNode != -1) {
            if (T->finite[outputNode]) {
                foundCount++;
            }
            outputNode = T->outputLink[outputNode];
        }
    }
    return foundCount;
}