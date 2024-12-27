#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *full_alphabet = "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "0123456789"
                            "!@#$%^&*()-_=+[]{}|;:,.<>?/~";

void generer_textes( int longueur, int taille_alphabet)
{
    if (taille_alphabet < 2 || taille_alphabet > 70)
    {
        printf("Erreur: taille_alphabet doit être entre 2 et 70\n");
        return;
    }

    char selected_chars[taille_alphabet];
    int used[70] = {0};

    // Sélection des caractères uniques pour cet alphabet
    for (int j = 0; j < taille_alphabet; j++)
    {
        int index;
        do
        {
            index = rand() % 70;
        } while (used[index]);

        used[index] = 1;
        selected_chars[j] = full_alphabet[index];
    }


   
    // Génération de le text 
    
    for (int j = 0; j < longueur; j++)
    {
        char caractere = selected_chars[rand() % taille_alphabet];
        putchar(caractere);
    }
    
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <longueur> <taille_alphabet>\n", argv[0]);
        return 1;
    }

    int longueur = atoi(argv[1]);
    int taille_alphabet = atoi(argv[2]);

    srand(time(NULL));
    generer_textes( longueur, taille_alphabet);
    return 0;
}