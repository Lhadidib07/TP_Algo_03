#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256

// Déclaration de la structure pour la file circulaire
typedef struct {
    int *queue;
    int front;
    int back;
    int size;
} Queue;

typedef struct _trie {
    int maxNode;         // Nombre maximal de noeuds
    int nextNode;        // Indice du prochain noeud
    int **transition;    // Matrice de transitions
    int *fail;           // Pointeurs vers le tableau des suppléants
    char *finite;        // Tableau des états terminaux
} Trie;

// Initialisation du Trie
void createTrie(Trie *trie) {
    trie->maxNode = 1000;
    trie->nextNode = 1;  // Commencer à partir de 1, 0 est la racine

    trie->transition = (int **)malloc(trie->maxNode * sizeof(int *));
    if (trie->transition == NULL) {
        perror("Erreur d'allocation mémoire pour transition");
        exit(EXIT_FAILURE);
    }

    trie->fail = (int *)malloc(trie->maxNode * sizeof(int));
    if (trie->fail == NULL) {
        perror("Erreur d'allocation mémoire pour fail");
        free(trie->transition);
        exit(EXIT_FAILURE);
    }

    trie->finite = (char *)malloc(trie->maxNode * sizeof(char));
    if (trie->finite == NULL) {
        perror("Erreur d'allocation mémoire pour finite");
        free(trie->transition);
        free(trie->fail);
        exit(EXIT_FAILURE);
    }

    // Initialiser les matrices de transitions et fail
    for (int i = 0; i < trie->maxNode; i++) {
        trie->transition[i] = (int *)malloc(ALPHABET_SIZE * sizeof(int));
        if (trie->transition[i] == NULL) {
            perror("Erreur d'allocation mémoire pour transition[i]");
            for (int k = 0; k < i; k++) {
                free(trie->transition[k]);
            }
            free(trie->transition);
            free(trie->fail);
            free(trie->finite);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            trie->transition[i][j] = -1;
        }
        trie->fail[i] = 0;
        trie->finite[i] = 0;
    }
}

// Insérer un mot dans le Trie
void insertInTrie(Trie *trie, const char *word) {
    int currentNode = 0;
    while (*word) {
        unsigned char c = (unsigned char)(*word);
        if (trie->transition[currentNode][c] == -1) {
            trie->transition[currentNode][c] = trie->nextNode++;
        }
        currentNode = trie->transition[currentNode][c];
        word++;
    }
    trie->finite[currentNode] = 1;  // Marquer l'état comme terminal
}

// Initialisation de la file circulaire
void initQueue(Queue *q, int size) {
    q->queue = (int *)malloc(size * sizeof(int));
    q->front = 0;
    q->back = 0;
    q->size = size;
}

// Enfiler (ajouter un élément dans la file)
void enfiler(Queue *q, int value) {
    q->queue[q->back] = value;
    q->back = (q->back + 1) % q->size;
}

// Défiler (retirer un élément de la file)
int defiler(Queue *q) {
    int value = q->queue[q->front];
    q->front = (q->front + 1) % q->size;
    return value;
}

// Fonction pour construire les liens suffixes (fail links)
void buildSuffixLinks(Trie *trie) {
    Queue q;
    initQueue(&q, trie->maxNode);

    // Initialiser les liens suffixes pour les transitions de la racine
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (trie->transition[0][i] != -1) {
            trie->fail[trie->transition[0][i]] = 0;
            enfiler(&q, trie->transition[0][i]);
        }
    }

    // Parcours en largeur pour calculer les liens suffixes
    while (q.front != q.back) {
        int currentNode = defiler(&q);
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (trie->transition[currentNode][i] != -1) {
                int failState = trie->fail[currentNode];
                while (trie->transition[failState][i] == -1 && failState != 0) {
                    failState = trie->fail[failState];
                }
                if (trie->transition[failState][i] != -1) {
                    trie->fail[trie->transition[currentNode][i]] = trie->transition[failState][i];
                } else {
                    trie->fail[trie->transition[currentNode][i]] = 0;
                }
                enfiler(&q, trie->transition[currentNode][i]);
            }
        }
    }
    free(q.queue);
}

// Recherche dans le texte pour compter les occurrences des mots
int searchText(Trie *trie, const char *text) {
    int currentNode = 0;
    int count = 0;

    while (*text) {
        unsigned char c = (unsigned char)(*text);

        // Suivre les transitions et les fail states
        while (trie->transition[currentNode][c] == -1) {
            currentNode = trie->fail[currentNode];  // Suivre le lien "fail"
            if (currentNode == 0) {
                break;  // Retour à la racine si aucun état valide
            }
        }

        if (trie->transition[currentNode][c] != -1) {
            currentNode = trie->transition[currentNode][c];  // Suivre la transition valide
        }

        // Vérifier les états terminaux (mot complet trouvé)
        int tempNode = currentNode;
        while (tempNode != 0) {
            if (trie->finite[tempNode]) {
                count++;
            }
            tempNode = trie->fail[tempNode];  // Suivre le lien "fail" pour vérifier les occurrences
        }

        text++;  // Passer au caractère suivant du texte
    }

    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <mots.txt> <texte.txt>\n", argv[0]);
        return 1;
    }

    // Ouvrir les fichiers mots.txt et texte.txt
    FILE *motsFile = fopen(argv[1], "r");
    FILE *texteFile = fopen(argv[2], "r");
    if (!motsFile || !texteFile) {
        perror("Erreur lors de l'ouverture des fichiers");
        return 1;
    }

    // Initialiser le Trie
    Trie trie;
    createTrie(&trie);

    // Insérer les mots dans le Trie
    char word[100];
    while (fgets(word, sizeof(word), motsFile)) {
        word[strcspn(word, "\n")] = '\0'; // Enlever le saut de ligne
        insertInTrie(&trie, word);
    }
    fclose(motsFile);

    // Construire les liens suffixes pour l'automate Aho-Corasick
    buildSuffixLinks(&trie);

    // Lire le texte et rechercher les mots
    char text[10000];
    size_t bytesRead = fread(text, sizeof(char), sizeof(text) - 1, texteFile);
    fclose(texteFile);
    text[bytesRead] = '\0'; // Assurer la fin de chaîne

    // Recherche et comptage des occurrences
    int count = searchText(&trie, text);
    printf("Nombre d'occurrences: %d\n", count);

    // Libérer la mémoire
    for (int i = 0; i < trie.maxNode; i++) {
        free(trie.transition[i]);
    }
    free(trie.transition);
    free(trie.fail);
    free(trie.finite);

    return 0;
}