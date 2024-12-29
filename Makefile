# Répertoires
SRC_DIR = source
INC_DIR = include

# Fichiers sources et objets
SRC_FILES = $(SRC_DIR)/ahoCorasick.c $(SRC_DIR)/file_d_attente.c $(SRC_DIR)/trie_matrice.c $(SRC_DIR)/trie.c $(SRC_DIR)/main.c
OBJ_FILES = $(SRC_FILES:.c=.o)

# Nom de l'exécutable
EXEC = ac_matrice

# Drapeaux de compilation
CFLAGS = -Wall -I$(INC_DIR) -g -std=c99

# Règle par défaut pour compiler l'exécutable
all: $(EXEC)

# Règle pour créer l'exécutable à partir des fichiers objets
$(EXEC): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(EXEC)

# Règle pour compiler les fichiers .c en fichiers .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJ_FILES) $(EXEC)

# Règle pour recompiler tout (force la reconstruction)
rebuild: clean all
