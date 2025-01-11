#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie_matrice.h"
#include "aho_corasick_matrice.h"

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
    fread(text, sizeof(char), sizeof(text)-1, texteFile);
    fclose(texteFile);
    text[strlen(text)] = '\0'; // Assurer la fin de chaîne

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
