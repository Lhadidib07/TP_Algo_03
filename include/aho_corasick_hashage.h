#ifndef AHO_CORASICK_HASHAGE_H
#define AHO_CORASICK_HASHAGE_H

#include "trie_hashage.h"


// Déclaration et initialisation de la file 
void initialiserFile(int **queue, int *queueStart, int *queueEnd, int maxNode);

// initialise une file pour la traité les noueds du trie  
void buildSuffixLinks(Trie T);

// parcourt tous les nouds du trie est fait appelle a la fonction traiterNoeud pour chaque nouds
void traiterFile(Trie T, int *queue, int *queueStart, int *queueEnd);

// parcourt tous les transitions d'un nouds et met a jour les liens de suffixe et de sortie 
void traiterNoeud(Trie T, int *queue, int *queueEnd, int currentNode);


/* Mettre à jour le lien de suffixe d'un nœud donné pour une lettre spécifique.
 utilise la fonction trouverTransition pour trouver la transition d'un nœud donné pour une lettre spécifique.
 Si une transition est trouvée, le lien de suffixe du nœud cible pointe vers le nœud cible de la transition.
 Si aucune transition n'est trouvée, le lien de suffixe pointe vers la racine. */
void mettreAJourLienSuffixe(Trie T, int currentNode, int targetNode, unsigned char letter);

// s il ya une transition elle retourn la cible de la transition sinon -1
int trouverTransition(Trie T, int currentNode, unsigned char letter);

// mettre a jour le lien de sortie d'un noeud 
void mettreAJourLienSortie(Trie T, int targetNode);

// fonction utilisé pour la recherches de motifs dans un texte avec le trie d'Aho-Corasick 
int searchPatterns(Trie T, unsigned char *text);

#endif // AHO_CORASICK_HASHAGE_H