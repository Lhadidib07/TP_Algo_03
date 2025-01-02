#include "file_d_attente.h"
#include <stdio.h>
#include <stdlib.h>

FILEATTENTE* creationFileAttente(size_t capaciteFile) {
    FILEATTENTE* fileAttente = malloc(sizeof(FILEATTENTE));
    if (fileAttente == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation de la file d'attente.\n");
        return NULL;
    }
    fileAttente->elements = malloc(capaciteFile * sizeof(int));
    if (fileAttente->elements == NULL) {
        free(fileAttente);
        fprintf(stderr, "Erreur lors de l'allocation du tableau des éléments.\n");
        return NULL;
    }

    fileAttente->front = 0;
    fileAttente->rear = 0;
    fileAttente->capacity = capaciteFile;
    fileAttente->tailleFile = 0;

    return fileAttente;
}

int etatFile(FILEATTENTE* fileAttente) {
    if (fileAttente->tailleFile == 0) {
        return 0; // File vide
    }
    if (fileAttente->tailleFile == fileAttente->capacity) {
        return 1; // File pleine
    }
    return -1; // File en cours
}

int enfiler(FILEATTENTE* fileAttente, int element) {
    if (etatFile(fileAttente) == 1) {
        return -1; // File pleine
    }

    fileAttente->elements[fileAttente->rear] = element;
    fileAttente->rear = (fileAttente->rear + 1) % fileAttente->capacity;
    fileAttente->tailleFile++;
    return 0;
}

int defiler(FILEATTENTE* fileAttente) {
    if (etatFile(fileAttente) == 0) {
        return -1; // File vide
    }

    int element = fileAttente->elements[fileAttente->front];
    fileAttente->front = (fileAttente->front + 1) % fileAttente->capacity;
    fileAttente->tailleFile--;
    return element;
}

void libererFile(FILEATTENTE* fileAttente) {
    free(fileAttente->elements);
    free(fileAttente);
}
