#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *full_alphabet = "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "!@#$%^&*()-_=+[]{}|;:,.<>?/~";

void generer_textes(int nb_mots, int longueur_min, int longueur_max, int taille_alphabet)
{
    if (taille_alphabet < 2 || taille_alphabet > 70)
    {
        printf("Erreur: taille_alphabet doit être entre 2 et 70\n");
        return;
    }

    if (longueur_min < 1 || longueur_max < longueur_min)
    {
        printf("Erreur: longueur_min doit être >= 1 et longueur_max doit être >= longueur_min\n");
        return;
    }

    srand(time(NULL)); // Initialise le générateur de nombres aléatoires

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

    // Génération des mots
    for (int i = 0; i < nb_mots; i++)
    {
        int longueur = longueur_min + rand() % (longueur_max - longueur_min + 1);
        for (int j = 0; j < longueur; j++)
        {
            char caractere = selected_chars[rand() % taille_alphabet];
            putchar(caractere);
        }
        // Ajoute un saut de ligne après chaque mot
        putchar('\n');
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        printf("Usage: %s <nb_mots> <longueur_min> <longueur_max> <taille_alphabet>\n", argv[0]);
        return 1;
    }

    int nb_mots = atoi(argv[1]);
    int longueur_min = atoi(argv[2]);
    int longueur_max = atoi(argv[3]);
    int taille_alphabet = atoi(argv[4]);

    generer_textes(nb_mots, longueur_min, longueur_max, taille_alphabet);
    return 0;
}