# Compilateur et options
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -Iinclude

# Répertoires
SRC_DIR = src
INCLUDE_DIR = include

# Exécutables
EXEC = ac-matrice ac-hachage genere-mots genere-texte

# Sources pour chaque exécutable
SRC_AC_MATRICE = $(SRC_DIR)/main_matrice.c $(SRC_DIR)/trie_matrice.c $(SRC_DIR)/aho_corasick_matrice.c
SRC_AC_HACHAGE = $(SRC_DIR)/main_hashage.c $(SRC_DIR)/trie_hashage.c $(SRC_DIR)/aho_corasick_hashage.c
SRC_GENERE_MOTS = genere-mots.c
SRC_GENERE_TEXTE = genere-texte.c

# Cible par défaut : génère tous les exécutables
all: $(EXEC)

# Règles pour ac-matrice
ac-matrice: $(SRC_AC_MATRICE)
	$(CC) $(CFLAGS) $^ -o $@

# Règles pour ac-hachage
ac-hachage: $(SRC_AC_HACHAGE)
	$(CC) $(CFLAGS) $^ -o $@

# Règles pour genere-mots
genere-mots: $(SRC_GENERE_MOTS)
	$(CC) $(CFLAGS) $^ -o $@

# Règles pour genere-texte
genere-texte: $(SRC_GENERE_TEXTE)
	$(CC) $(CFLAGS) $^ -o $@

# Nettoyage des fichiers objets et exécutables
clean:
	rm -f $(EXEC) 

# Nettoyage complet
mrproper: clean
	rm -f texte3.txt mots3.txt res-ac-matrice res-ac-hachage
