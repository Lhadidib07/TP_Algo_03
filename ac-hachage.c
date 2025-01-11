#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HASH_SIZE 256
#define MAX_WORD_LENGTH 100
#define MAX_TEXT_LENGTH 500010

typedef struct _list {
    int startNode;
    int targetNode;
    unsigned char letter;
    struct _list *next;
} List;

typedef struct _trie {
    int maxNode;
    int nextNode;
    List **transition;
    char *finite;
    int *suffixLink;
    int *outputLink;
} *Trie;

int hash(unsigned char key, int etat) {
    return (key * 31 + etat * 17) % HASH_SIZE;
}

Trie createTrieHash(int maxNode) {
    Trie T = (Trie)malloc(sizeof(struct _trie));
    T->maxNode = maxNode;
    T->nextNode = 1;
    T->transition = (List **)malloc(maxNode * sizeof(List *));
    T->finite = (char *)malloc(maxNode * sizeof(char));
    T->suffixLink = (int *)malloc(maxNode * sizeof(int));
    T->outputLink = (int *)malloc(maxNode * sizeof(int));
    for (int i = 0; i < maxNode; i++) {
        T->transition[i] = NULL;
        T->finite[i] = 0;
        T->suffixLink[i] = -1;
        T->outputLink[i] = -1;
    }
    return T;
}

void addTransition(Trie T, int startNode, int targetNode, unsigned char letter) {
    int hashResult = hash(letter, startNode);
    List *newNode = (List *)malloc(sizeof(List));
    newNode->startNode = startNode;
    newNode->targetNode = targetNode;
    newNode->letter = letter;
    newNode->next = T->transition[hashResult];
    T->transition[hashResult] = newNode;
}

void insertPattern(Trie T, unsigned char *pattern) {
    int currentNode = 0;
    for (int i = 0; pattern[i] != '\0'; i++) {
        int hashResult = hash(pattern[i], currentNode);
        List *transition = T->transition[hashResult];
        while (transition != NULL && transition->letter != pattern[i]) {
            transition = transition->next;
        }
        if (transition == NULL) {
            T->transition[hashResult] = (List *)malloc(sizeof(List));
            T->transition[hashResult]->startNode = currentNode;
            T->transition[hashResult]->targetNode = T->nextNode++;
            T->transition[hashResult]->letter = pattern[i];
            T->transition[hashResult]->next = NULL;
            currentNode = T->transition[hashResult]->targetNode;
        } else {
            currentNode = transition->targetNode;
        }
    }
    T->finite[currentNode] = 1;
}

void buildSuffixLinks(Trie T) {
    int *queue = (int *)malloc(T->maxNode * sizeof(int));
    int queueStart = 0;
    int queueEnd = 0;
    queue[queueEnd++] = 0;
    while (queueStart < queueEnd) {
        int currentNode = queue[queueStart++];
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
                queue[queueEnd++] = targetNode;
            }
        }
    }
}

char **extraire_mots_fichier(const char *fichier, int *nombre_mots) {
    FILE *f = fopen(fichier, "r");
    if (f == NULL) {
        fprintf(stderr, "hello Erreur lors de l'ouverture du fichier %s\n", fichier);
        exit(EXIT_FAILURE);
    }
    char **mots = NULL;
    int count = 0;
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), f)) {
        // Supprimer les retours à la ligne
        ligne[strcspn(ligne, "\r\n")] = 0;

        // Diviser la ligne en mots
        char *mot = strtok(ligne, " ");
        while (mot != NULL) {
            mots = realloc(mots, (count + 1) * sizeof(char *));
            mots[count] = strdup(mot);
            count++;
            mot = strtok(NULL, " ");
        }
    }

    fclose(f);
    *nombre_mots = count;
    return mots;
}

char *lire_texte_fichier(const char *fichier) {

    FILE *f = fopen(fichier, "r");
    if (f == NULL) {
        fprintf(stderr, " ici Erreur lors de l'ouverture du fichier %s\n", fichier);
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    long taille_fichier = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *texte = malloc(taille_fichier + 1);
    if (texte == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    fread(texte, 1, taille_fichier, f);
    texte[taille_fichier] = '\0';

    fclose(f);
    return texte;
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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <patterns_file> <text_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Trie T = createTrieHash(1000);

    // Lire les mots à partir du fichier
    int nombreMots;
    char **patterns; 

    patterns = extraire_mots_fichier(argv[1], &nombreMots);

    // Insérer les mots dans le Trie
    for (int i = 0; i < nombreMots; i++) {
        insertPattern(T, (unsigned char *)patterns[i]);
        free(patterns[i]); // Free each pattern after insertion
    }
    free(patterns); // Free the array of patterns

    // Construire les liens de suffixe
    buildSuffixLinks(T);

    // Lire le texte à partir du fichier
    char *text;
    text = lire_texte_fichier(argv[2]);

    // Rechercher les mots dans le texte
    int foundCount = searchPatterns(T, (unsigned char *)text);
    printf("\n nombre de mot trouve : %d\n", foundCount);

    free(text); // Free the text after searching

    return 0;
}