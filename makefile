# Compilateur et options de compilation
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -Iinclude

# Répertoires
SRC_DIR = src
INCLUDE_DIR = include

# Exécutables
EXEC_HACHAGE = ac-hachage
EXEC_MATRICE = ac-matrice

# Fichiers sources et objets
SRC_HACHAGE = $(SRC_DIR)/main.c $(SRC_DIR)/aho_corasick_hashage.c $(SRC_DIR)/trie_hashage.c
OBJ_HACHAGE = $(SRC_HACHAGE:.c=.o)

SRC_MATRICE = $(SRC_DIR)/main.c $(SRC_DIR)/aho_corasick_matrice.c $(SRC_DIR)/trie_matrice.c
OBJ_MATRICE = $(SRC_MATRICE:.c=.o)

# Cibles par défaut : pour utiliser la méthode matrice ou hachage
# La macro -DMATRIX ou -DHASHING est définie ici en fonction de la cible

# Règle pour construire ac-hachage
$(EXEC_HACHAGE): CFLAGS += -DHASHING
$(EXEC_HACHAGE): $(OBJ_HACHAGE)
	$(CC) $(CFLAGS) $(OBJ_HACHAGE) -o $(EXEC_HACHAGE)

# Règle pour construire ac-matrice
$(EXEC_MATRICE): CFLAGS += -DMATRIX
$(EXEC_MATRICE): $(OBJ_MATRICE)
	$(CC) $(CFLAGS) $(OBJ_MATRICE) -o $(EXEC_MATRICE)

# Règle générique pour compiler les fichiers objets
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cible par défaut (construire tout)
all: $(EXEC_HACHAGE) $(EXEC_MATRICE)

# Nettoyage
clean:
	rm -f $(SRC_DIR)/*.o $(EXEC_HACHAGE) $(EXEC_MATRICE)
