#include "trie_hashage.h"
#include "aho_corasick_hashage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <patterns_file> <text_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Ouvrir les fichiers des mots et du texte
    FILE *motsFile = fopen(argv[1], "r");
    FILE *texteFile = fopen(argv[2], "r");
    if (!motsFile || !texteFile) {
        perror("Erreur lors de l'ouverture des fichiers");
        return EXIT_FAILURE;
    }

    // Créer et initialiser le Trie
    Trie T = createTrieHash(1000);

    // Lire les mots à partir du fichier des motifs et les insérer dans le Trie
    char word[100];
    while (fgets(word, sizeof(word), motsFile)) {
        word[strcspn(word, "\n")] = '\0';  // Supprimer le saut de ligne
        insertPattern(T, (unsigned char *)word);
    }
    fclose(motsFile);

    // Construire les liens de suffixe
    buildSuffixLinks(T);

    // Lire le texte à partir du fichier
    char text[10000];
    fread(text, sizeof(char), sizeof(text) - 1, texteFile);
    fclose(texteFile);
    text[strlen(text)] = '\0';  // Assurer la fin de chaîne

    // Recherche des motifs dans le texte
    int foundCount = searchPatterns(T, (unsigned char *)text);
    printf("\nNombre de mots trouvés : %d\n", foundCount);

    return 0;
}
