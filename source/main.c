// main.c
#include "ac.h"
#include <stdio.h>
#include <string.h>

int main() {
    const char* mots[] = {"he", "she", "his", "hers"};
    size_t nbrMots = sizeof(mots) / sizeof(mots[0]);

    AhoCorasick* ac = initialiseAC(mots, nbrMots);
    if (!ac) {
        printf("Erreur d'initialisation de l'automate Aho-Corasick\n");
        return 1;
    }

    buildSuppleanceLinks(ac);

    const char* texte = "ushers";
    unsigned int count = recherche(ac, texte);
    printf("Nombre de mots trouvés dans '%s' : %u\n", texte, count);

    libererAC(ac);
    return 0;
}
