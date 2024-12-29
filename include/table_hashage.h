#ifndef TABLE_HASHAGE_H
#define TABLE_HASHAGE_H

    #ifdef HACHAGE

        struct _trie {
            int maxNode;        /* Nombre maximal de noeuds du trie */
            int nextNode;       /* Indice du prochain noeud disponible */
            List *transition;   /* listes d’adjacence */
            char *finite;       /* etats terminaux */
        };

        typedef struct _trie *Trie ;

        struct _list {
            int startNode;       // État de départ de la transition
            int targetNode;      // Cible de la transition
            unsigned char letter; // Lettre de la transition
            struct _list *next;  // Maillon suivant dans la liste
        };

        typedef struct _list *List;

        extern int fonctionDeHashage(unsigned char w, int maxNode);

        void ajouterTrans(List *liste, int startNode, int targetNode, unsigned char w);

        int hasNextTrans(List liste);

        List findTrans(List liste, int startNode, unsigned char w);

        void libererListe(List liste);

    #endif


#endif