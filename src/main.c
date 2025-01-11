#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusion des headers selon le mode (matrice ou hachage)
#ifdef MATRIX
    #include "trie_matrice.h"
    #include "aho_corasick_matrice.h"
#elif defined(HASHING)
    #include "trie_hashage.h"
    #include "aho_corasick_hashage.h"
#endif

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <mots.txt> <texte.txt>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Ouvrir les fichiers des mots et du texte
    FILE *motsFile = fopen(argv[1], "r");
    FILE *texteFile = fopen(argv[2], "r");
    if (!motsFile || !texteFile) {
        perror("Erreur lors de l'ouverture des fichiers");
        return EXIT_FAILURE;
    }

#ifdef MATRIX
    // Créer et initialiser le Trie pour la matrice
    Trie trie;
    createTrie(&trie);

    // Lire les mots et les insérer dans le Trie
    char word[100];
    while (fgets(word, sizeof(word), motsFile)) {
        word[strcspn(word, "\n")] = '\0';  // Supprimer le saut de ligne
        insertInTrie(&trie, word);
    }
    fclose(motsFile);

    // Construire les liens suffixes
    buildSuffixLinks(&trie);

    // Lire le texte
    char text[10000];
    fread(text, sizeof(char), sizeof(text)-1, texteFile);
    fclose(texteFile);
    text[strlen(text)] = '\0';  // Assurer la fin de chaîne

    // Recherche et comptage des occurrences
    int count = searchText(&trie, text);
    printf("Nombre d'occurrences (matrice): %d\n", count);
#elif defined(HASHING)
    // Créer et initialiser le Trie pour le hachage
    Trie T = createTrieHash(1000);

    // Lire les mots et les insérer dans le Trie
    char word[100];
    while (fgets(word, sizeof(word), motsFile)) {
        word[strcspn(word, "\n")] = '\0';  // Supprimer le saut de ligne
        insertPattern(T, (unsigned char *)word);
    }
    fclose(motsFile);

    // Construire les liens de suffixe
    buildSuffixLinks(T);

    // Lire le texte
    char text[10000];
    fread(text, sizeof(char), sizeof(text) - 1, texteFile);
    fclose(texteFile);
    text[strlen(text)] = '\0';  // Assurer la fin de chaîne

    // Recherche des motifs dans le texte
    int foundCount = searchPatterns(T, (unsigned char *)text);
    printf("\nNombre de mots trouvés (hachage): %d\n", foundCount);
#endif

    return 0;
}
