#!/bin/bash

#generation des textes 
# Dossier de sortie pour les textes
output_folder="text"
mkdir -p "$output_folder"

# Génération des textes de 5 000 000 caractères sur différentes tailles d'alphabets
longueur=5000000
alphabet_sizes=(2 4 20 70)

#chemin du fichier output 
for taille_alphabet in "${alphabet_sizes[@]}"; do
    ./genere-texte  "$longueur" "$taille_alphabet" > "$output_folder/texte_${taille_alphabet}.txt"
done

if [ ! -f "genere-mots" ]; then
    echo "Le fichier genere-mots n'existe pas. Veuillez compiler le programme." | tee -a "$LOG_FILE"
    bash make 
fi

# Génération des ensembles de mots pseudo-aléatoires
# Dossier de sortie pour les ensembles de mots
output_folder="mots"
mkdir -p "$output_folder"

nb_mots=100

# Génération des ensembles de mots pour différentes plages de longueurs
word_length_ranges=(
    "5 15"
    "15 30"
    "30 60"
)

#chemin du fichier output 
for taille_alphabet in "${alphabet_sizes[@]}"; do
    for range in "${word_length_ranges[@]}"; do
        IFS=' ' read -r min_length max_length <<< "$range"
        ./genere-mots  "$nb_mots" "$min_length" "$max_length" "$taille_alphabet" > "$output_folder/mots_${min_length}_${max_length}_${taille_alphabet}.txt"
    done
done

# Nettoyage du fichier temporaire
rm -f generateur_temp 

echo "Génération des textes et des ensembles de mots terminée."
