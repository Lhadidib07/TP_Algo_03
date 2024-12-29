#ifndef FILE_D_ATTENTE_H
#define FILE_D_ATTENTE_H
#include <stddef.h>

// Définition de la structure FILEATTENTE
typedef struct _fileAttente {
    int* elements;  // Tableau des éléments dans la file
    int front;      // Index du premier élément
    int rear;       // Index du dernier élément
    int capacity;   // Capacité de la file
    int tailleFile; // Taille actuelle de la file
} FILEATTENTE;

// Prototypes des fonctions liées à la file d'attente
FILEATTENTE* creationFileAttente(size_t capaciteFile);
int etatFile(FILEATTENTE* fileAttente);
int enfiler(FILEATTENTE* fileAttente, int element);
int defiler(FILEATTENTE* fileAttente);
void libererFile(FILEATTENTE* fileAttente);

#endif // FILE_D_ATTENTE_H
