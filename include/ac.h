#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include <stddef.h>
#include "trie.h"
#include "file_d_attente.h"

// Définition de la structure AhoCorasick
typedef struct _ac {
    Trie* trie;               // Pointeur vers la structure Trie
    int* suppleances;         // Tableau contenant les liens de suppléance
    int** outputs;            // Tableau pour stocker une liste de mots associés à chaque noeud terminal
} AhoCorasick;

typedef struct _ac* AhoCorasickPtr;

// Prototypes des fonctions
AhoCorasick* initialiseAC(const char** w, size_t nbrMots);
void buildSuppleanceLinks(AhoCorasick* ac);
unsigned int recherche(AhoCorasick* ac, const char* texte);
void libererAC(AhoCorasick* ac);

#endif // AHO_CORASICK_H
