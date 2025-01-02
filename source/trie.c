#include <trie.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <limits.h> // Pour pouvoir utiliser UCHAR_MAX


void insertInTrie(Trie* trie, unsigned char* w) {
    int currentState = 0;
    while (*w != '\0') {
        unsigned char letter = *w; // Récupérer le caractère actuel

        int index = (unsigned char)letter; // Assurer l'indexation dans le bon range (0 à 255)

        // Si la transition existe, aller à l'état suivant
        if (trie->transition[currentState][index] != -1) {
            currentState = trie->transition[currentState][index];
        } else {
            // Ajouter une nouvelle transition
            if (trie->nextNode >= trie->maxNode) {
                fprintf(stderr, "Nombre maximal de nœuds atteint dans le trie\n");
                exit(EXIT_FAILURE);
            }
            trie->transition[currentState][index] = trie->nextNode;
            currentState = trie->nextNode;
            trie->nextNode += 1;
        }
        ++w;  // Passer au prochain caractère
    }

    // Marquer l'état final
    trie->finite[currentState] = 1;
    printf("Mot %s inséré avec succès dans le trie\n", w);
}


// isInTrie ne sert a rien dans ce projet
// int isInTrie(Trie* trie, unsigned char* w) {
//     int currentState = 0;
//     printf("Appel de isInTrie avec le mot : %s\n", w);  // Message de débogage
//     unsigned char* originalWord = w;  // Sauvegarder l'adresse initiale de w
    
//     while (*w != '\0') {
//         unsigned char letter = *w; // Récupérer le caractère actuel
//         // Si la transition existe, aller au prochain état
//         if (trie->transition[letter][currentState] != -1) {
//             currentState = trie->transition[letter][currentState];
//         } else {
//             return 0;  // Mot non trouvé
//         }
//         ++w; //Passer au prochain caractère
//     }
//     return trie->finite[currentState]; // Retourner 1 si l'état final est terminal
// }

//Récupérer une transition 

int getTransition(Trie* trie, int startNode, unsigned char w){
    if(startNode >= (*trie)->maxNode || startNode < 0 || w > UCHAR_MAX ){
        fprintf(stderr,"Transition invalide.");
        return -1;
    }
    return (*trie)->transition[w][startNode];
}
