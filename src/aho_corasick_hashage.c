#include "aho_corasick_hashage.h"
#include <stdlib.h>
#include <stdio.h>

void initialiserFile(int **queue, int *queueStart, int *queueEnd, int maxNode) {
    *queue = (int *)malloc(maxNode * sizeof(int));
    if (*queue == NULL) {
        perror("Memory allocation failed for queue");
        exit(EXIT_FAILURE);
    }
    *queueStart = 0;
    *queueEnd = 0;
    (*queue)[(*queueEnd)++] = 0; // Ajouter la racine (nœud 0)
}

void mettreAJourLienSortie(Trie T, int targetNode) {
    int suffixNode = T->suffixLink[targetNode];
    if (T->finite[suffixNode]) {
        T->outputLink[targetNode] = suffixNode;
    } else {
        T->outputLink[targetNode] = T->outputLink[suffixNode];
    }
}

int trouverTransition(Trie T, int currentNode, unsigned char letter) {
    List *transition = T->transition[hash(letter, currentNode)];
    while (transition != NULL) {
        if (transition->letter == letter) {
            return transition->targetNode;
        }
        transition = transition->next;
    }
    return -1; // Pas de transition trouvée
}

void mettreAJourLienSuffixe(Trie T, int currentNode, int targetNode, unsigned char letter) {
    if (currentNode == 0) {
        T->suffixLink[targetNode] = 0;
    } else {
        int suffixNode = T->suffixLink[currentNode];
        while (suffixNode != -1) {
            int suffixTarget = trouverTransition(T, suffixNode, letter);
            if (suffixTarget != -1) {
                T->suffixLink[targetNode] = suffixTarget;
                return;
            }
            suffixNode = T->suffixLink[suffixNode];
        }
        T->suffixLink[targetNode] = 0;
    }
}

void traiterNoeud(Trie T, int *queue, int *queueEnd, int currentNode) {
    List *transition;
    for (int i = 0; i < HASH_SIZE; i++) {
        transition = T->transition[i];
        while (transition != NULL) {
            if (transition->startNode == currentNode) {
                int targetNode = transition->targetNode;
                mettreAJourLienSuffixe(T, currentNode, targetNode, transition->letter);
                mettreAJourLienSortie(T, targetNode);
                queue[(*queueEnd)++] = targetNode;
            }
            transition = transition->next;
        }
    }
}

void traiterFile(Trie T, int *queue, int *queueStart, int *queueEnd) {
    while (*queueStart < *queueEnd) {
        int currentNode = queue[(*queueStart)++];
        traiterNoeud(T, queue, queueEnd, currentNode);
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
        while (currentNode != -1 && trouverTransition(T, currentNode, text[i]) == -1) {
            currentNode = T->suffixLink[currentNode];
        }
        if (currentNode == -1) {
            currentNode = 0;
        } else {
            currentNode = trouverTransition(T, currentNode, text[i]);
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
