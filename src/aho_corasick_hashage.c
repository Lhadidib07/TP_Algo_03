#include "aho_corasick_hashage.h"
#include <stdlib.h>
#include <stdio.h>

void initQueue(Queue *q, int size) {
    q->queue = (int *)malloc(size * sizeof(int));
    q->front = 0;
    q->back = 0;
    q->size = size;
}

void enfiler(Queue *q, int value) {
    if ((q->back + 1) % q->size == q->front) {
        // Queue pleine, gestion de l'erreur (ou agrandissement)
        fprintf(stderr, "Erreur : la queue est pleine\n");
        return;
    }
    q->queue[q->back] = value;
    q->back = (q->back + 1) % q->size;
}

int defiler(Queue *q) {
    if (q->front == q->back) {
        // Queue vide
        fprintf(stderr, "Erreur : la queue est vide\n");
        return -1; // ou une autre valeur de signalisation
    }
    int value = q->queue[q->front];
    q->front = (q->front + 1) % q->size;
    return value;
}

void buildSuffixLinks(Trie T) {
    Queue q;
    initQueue(&q, T->maxNode);  // Initialisation de la queue avec la taille appropriée

    enfiler(&q, 0);  // Enfiler le nœud de départ

    while (q.front != q.back) {
        int currentNode = defiler(&q);  // Défiler le nœud actuel

        for (int i = 0; i < HASH_SIZE; i++) {
            int targetNode = -1;
            List *transition = T->transition[hash(i, currentNode)];
            while (transition != NULL) {
                if (transition->letter == i) {
                    targetNode = transition->targetNode;
                    break;
                }
                transition = transition->next;
            }
            if (targetNode != -1) {
                if (currentNode == 0) {
                    T->suffixLink[targetNode] = 0;
                } else {
                    int suffixNode = T->suffixLink[currentNode];
                    while (suffixNode != -1) {
                        List *suffixTransition = T->transition[hash(i, suffixNode)];
                        while (suffixTransition != NULL) {
                            if (suffixTransition->letter == i) {
                                T->suffixLink[targetNode] = suffixTransition->targetNode;
                                break;
                            }
                            suffixTransition = suffixTransition->next;
                        }
                        if (T->suffixLink[targetNode] != -1) {
                            break;
                        }
                        suffixNode = T->suffixLink[suffixNode];
                    }
                    if (suffixNode == -1) {
                        T->suffixLink[targetNode] = 0;
                    }
                }
                if (T->finite[T->suffixLink[targetNode]]) {
                    T->outputLink[targetNode] = T->suffixLink[targetNode];
                } else {
                    T->outputLink[targetNode] = T->outputLink[T->suffixLink[targetNode]];
                }
                enfiler(&q, targetNode);  // Enfiler le nœud suivant
            }
        }
    }

    free(q.queue);  // Libérer la mémoire allouée pour la queue
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