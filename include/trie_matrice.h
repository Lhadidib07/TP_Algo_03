#ifndef TRIE_MATRICE_H
#define TRIE_MATRICE_H

typedef struct _trie  {
    int maxNode; /* Nombre maximal de noeuds du trie */
    int nextNode; /* Indice du prochain noeud disponible */
    int **transition; /* Matrice de transition */
    char *finite; /* Etats terminaux */
} Trie;

#endif // TRIE_MATRICE_H
