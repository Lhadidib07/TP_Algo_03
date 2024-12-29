#include "ac.h"
#include "trie.h"
#include "file_d_attente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AhoCorasick* initialiseAC(const char** w, size_t nbrMots) {
    printf("Initialisation de l'automate Aho-Corasick\n");
    AhoCorasick* ac = malloc(sizeof(AhoCorasick));
    if (ac == NULL) {
        fprintf(stderr, "L'allocation dynamique pour l'automate Aho-Corasick a échoué\n");
        return NULL;
    }
    
    // Calculer la taille maximale du trie
    size_t tailleMax = 0;
    for (size_t i = 0; i < nbrMots; i++) {
        tailleMax += strlen(w[i]);
    }
    printf("Taille maximale du trie : %zu\n", tailleMax);

    ac->trie = createTrie(tailleMax);  // createTrie retourne un Trie, pas un Trie**

    if (ac->trie == NULL) {
        fprintf(stderr, "L'allocation du trie a échoué\n");
        free(ac);
        return NULL;
    }

    printf("Trie créé avec succès\n");

    // Insertion de chaque mot dans le trie
    for (size_t i = 0; i < nbrMots; i++) {
        insertInTrie(ac->trie, (unsigned char*)w[i]);
    }
    printf("Mots insérés dans le trie\n");

    // Allocation pour les suppléances
    ac->suppleances = malloc(ac->trie->maxNode * sizeof(int));
    if (ac->suppleances == NULL) {
        fprintf(stderr, "L'allocation mémoire pour les suppléances a échoué\n");
        free(ac);
        return NULL;
    }

    // Allocation pour les sorties
    ac->outputs = calloc(ac->trie->maxNode, sizeof(int*));
    if (ac->outputs == NULL) {
        fprintf(stderr, "L'allocation mémoire pour le tableau de sortie a échoué\n");
        free(ac->suppleances);
        free(ac);
        return NULL;
    }

    printf("Automate Aho-Corasick initialisé avec succès\n");

    return ac;
}

void buildSuppleanceLinks(AhoCorasick* ac) {
    printf("Démarrage de la construction des liens de suppléance...\n");
    FILEATTENTE* fileAttente = creationFileAttente(ac->trie->maxNode);
    if (fileAttente == NULL) {
        fprintf(stderr, "Erreur lors de la création de la file d'attente\n");
        return;
    }

    ac->suppleances[0] = 0;  // La racine pointe sur elle-même

    // Initialiser les liens de suppléance pour les transitions depuis la racine
    for (int i = 0; i < 256; i++) {
        int noeud = getTransition(ac->trie, 0, i);

        if (noeud != -1) {
            ac->suppleances[noeud] = 0;
            enfiler(fileAttente, noeud);
        }
    }

    // Parcours en largeur
    while (etatFile(fileAttente) > 0) {
        int noeudCourant = defiler(fileAttente);

        for (int i = 0; i < 256; i++) {
            int prochainNoeud = getTransition(ac->trie, noeudCourant, i);

            if (prochainNoeud != -1) {
                int supp = ac->suppleances[noeudCourant];
                while (supp != 0 && getTransition(ac->trie, supp, i) == -1) {
                    supp = ac->suppleances[supp];
                }

                ac->suppleances[prochainNoeud] = getTransition(ac->trie, supp, i);
                if (ac->suppleances[prochainNoeud] == -1) {
                    ac->suppleances[prochainNoeud] = 0;  // Retour à la racine si aucune transition valide
                }

                // Fusion des sorties
                if (ac->outputs[prochainNoeud] == NULL) {
                    ac->outputs[prochainNoeud] = malloc(sizeof(int) * 2);
                    ac->outputs[prochainNoeud][0] = -1;  // Liste terminée par -1
                }

                if (ac->outputs[ac->suppleances[prochainNoeud]] != NULL) {
                    for (int* output = ac->outputs[ac->suppleances[prochainNoeud]];
                         *output != -1; output++) {
                        // Ajoutez chaque sortie
                    }
                }

                enfiler(fileAttente, prochainNoeud);
            }
        }
    }

    printf("Liens de suppléance construits\n");
    libererFile(fileAttente);
}



unsigned int recherche(AhoCorasick* ac, const char* texte) {
    int noeudCourant = 0;
    unsigned int count = 0;
    int transition;  // Déclaration de la variable ici

    for (size_t i = 0; texte[i] != '\0'; i++) {
        int index = (unsigned char)texte[i];

        while (noeudCourant != 0 && getTransition(ac->trie, noeudCourant, index) == -1) {
            noeudCourant = ac->suppleances[noeudCourant];
        }

        transition = getTransition(ac->trie, noeudCourant, index); // Utilisation après déclaration

        noeudCourant = transition;
        if (noeudCourant == -1) {
            noeudCourant = 0;  // Retour à la racine si aucune transition valide
        }

        if (ac->outputs[noeudCourant] != NULL) {
            for (int j = 0; ac->outputs[noeudCourant][j] != -1; j++) {
                count++;
            }
        }
    }
    return count;
}


void libererAC(AhoCorasick* ac) {
    if (ac == NULL) {
        return;  // Si le pointeur est NULL, rien à libérer
    }

    // Libérer la mémoire du trie (s'il est utilisé)
    if (ac->trie != NULL) {
        libererTrie(ac->trie);  // Assurez-vous d'avoir une fonction libererTrie pour libérer la mémoire du trie
    }

    // Libérer la mémoire des suppléances si elles existent
    if (ac->suppleances != NULL) {
        free(ac->suppleances);
    }

    // Libérer la mémoire des sorties si elles existent
    if (ac->outputs != NULL) {
        free(ac->outputs);
    }

    // Libérer la structure AhoCorasick elle-même
    free(ac);
}