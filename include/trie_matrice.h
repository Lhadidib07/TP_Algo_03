#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 256 // Taille de l'alphabet (caractères possibles)

// Déclaration de la structure Trie
typedef struct _trie {
    int maxNode;         // Nombre maximal de noeuds
    int nextNode;        // Indice du prochain noeud
    int **transition;    // Matrice de transitions
    int *fail;           // Pointeurs vers le tableau des suppléants
    char *finite;        // Tableau des états terminaux
} Trie;


// Prototypes des fonctions relatives au Trie
void createTrie(Trie *trie);
void insertInTrie(Trie *trie, const char *word);


#endif // TRIE_H
