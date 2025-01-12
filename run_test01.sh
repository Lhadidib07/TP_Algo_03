#!/bin/bash

# Variables globales
ALPHABETS=(2 4 20 70)
TAILLES_MOTS=(4 5 6 7 8 9 10 15 20 30 40 50)

TEXT_DIR="./text"
WORD_DIR="./mots"
RESULTS_DIR="./results"

# change les chemins des fichiers
# Définir le nom de l'exécutable
EXECUTABLE_AC_HACHAGE="./ac-hachage"
EXECUTABLE_AC_MATRICE="./ac-matrice"

mkdir -p "$RESULTS_DIR"

# Vérifier si les exécutables existent
if [ ! -f "$EXECUTABLE_AC_HACHAGE" ]; then
    echo "Exécutable $EXECUTABLE_AC_HACHAGE non trouvé. Veuillez compiler le programme." | tee -a "$LOG_FILE"
    exit 1    
fi

if [ ! -f "$EXECUTABLE_AC_MATRICE" ]; then
    echo "Exécutable $EXECUTABLE_AC_MATRICE non trouvé. Veuillez compiler le programme." | tee -a "$LOG_FILE"
    exit 1    
fi

# Log d'exécution
LOG_FILE="$RESULTS_DIR/execution.log"
echo "Début de l'exécution : $(date)" > "$LOG_FILE"

word_length_ranges=(
    "5 15"
    "15 30"
    "30 60"
)

# Algorithmes à tester
ALGORITHMES=("hachage" "matrice")

# Exécution des tests
for alphabet in "${ALPHABETS[@]}"; do
    text_file="$TEXT_DIR/texte_${alphabet}.txt"
    if [ ! -f "$text_file" ]; then
        echo "Fichier texte $text_file non trouvé." | tee -a "$LOG_FILE"
        continue
    fi

    # Initialiser le fichier CSV pour cet alphabet avec des en-têtes
    CSV_FILE="$RESULTS_DIR/results_${alphabet}.csv"
    echo "Algorithme,Min_Length,Max_Length,Alphabet,Execution_Time_ms" > "$CSV_FILE"

    for range in "${word_length_ranges[@]}"; do
        IFS=' ' read -r min_length max_length <<< "$range"
        word_file="$WORD_DIR/mots_${min_length}_${max_length}_${alphabet}.txt"
        if [ ! -f "$word_file" ]; then
            echo "Fichier de mots $word_file non trouvé." | tee -a "$LOG_FILE"
            continue
        fi

        for algo in "${ALGORITHMES[@]}"; do
            if [ "$algo" == "hachage" ]; then
                EXECUTABLE=$EXECUTABLE_AC_HACHAGE
            else
                EXECUTABLE=$EXECUTABLE_AC_MATRICE
            fi

            echo "Exécution : Algorithme=$algo, Texte=$text_file, Mots=$word_file" | tee -a "$LOG_FILE"

            # Mesurer le temps d'exécution
            START_TIME=$(date +%s%3N)
            OUTPUT=$($EXECUTABLE "$text_file" "$word_file")
            END_TIME=$(date +%s%3N)
            EXECUTION_TIME=$((END_TIME - START_TIME))

            echo "Résultat : $OUTPUT, Temps d'exécution : $EXECUTION_TIME ms" | tee -a "$LOG_FILE"

            # Enregistrer dans le fichier CSV pour cet alphabet
            printf "%s,%s,%s,%s,%s,%s\n" "$algo" "$min_length" "$max_length" "$alphabet" "$EXECUTION_TIME" >> "$CSV_FILE"
        done
    done
done

echo "Fin de l'exécution : $(date)" >> "$LOG_FILE"