import os
import pandas as pd
import matplotlib.pyplot as plt

# Répertoires
RESULTS_DIR = "./results"
GRAPH_DIR = "./graph"

# Créer le répertoire des graphiques s'il n'existe pas
os.makedirs(GRAPH_DIR, exist_ok=True)

# Fonction pour supprimer les valeurs aberrantes d'un algorithme
def supprimer_outliers(data, col):
    Q1 = data[col].quantile(0.25)  # Premier quartile
    Q3 = data[col].quantile(0.75)  # Troisième quartile
    IQR = Q3 - Q1  # Écart interquartile
    limite_basse = Q1 - 1.5 * IQR
    limite_haute = Q3 + 1.5 * IQR
    return data[(data[col] >= limite_basse) & (data[col] <= limite_haute)]

# Fonction pour créer les intervalles
def definir_intervalles(row):
    if row['Min_Length'] < 15:
        return '15'
    elif row['Min_Length'] < 30:
        return '30'
    else:
        return '60'

# Parcourir tous les fichiers CSV dans le répertoire des résultats
for fichier in os.listdir(RESULTS_DIR):
    if fichier.endswith(".csv"):
        chemin_fichier = os.path.join(RESULTS_DIR, fichier)
        
        # Charger les données du fichier CSV
        colonnes = ["Algorithme", "Min_Length", "Max_Length", "Alphabet", "Execution_Time_ms"]
        donnees = pd.read_csv(chemin_fichier, names=colonnes, skiprows=1)

        

        # Convertir les colonnes nécessaires en numérique
        donnees["Min_Length"] = pd.to_numeric(donnees["Min_Length"], errors='coerce')
        donnees["Max_Length"] = pd.to_numeric(donnees["Max_Length"], errors='coerce')
        donnees["Execution_Time_ms"] = pd.to_numeric(donnees["Execution_Time_ms"], errors='coerce')

       
        # Ajouter une colonne pour les intervalles
        donnees['Intervalle'] = donnees.apply(definir_intervalles, axis=1)

        # Obtenir la liste unique des algorithmes
        algorithmes = donnees["Algorithme"].unique()

        # Définir les intervalles de longueur de mots
        intervals = ['15', '30', '60']

        # Tracer les courbes pour chaque algorithme
        plt.figure(figsize=(12, 8))
        has_data = False  # Indicateur pour vérifier si des données ont été tracées
        for algo in algorithmes:
            data_algo = donnees[donnees["Algorithme"] == algo]
            data_algo = supprimer_outliers(data_algo, "Execution_Time_ms")
            
            # Vérifier que les données ne sont pas vides avant de tracer
            if not data_algo.empty:
                plt.plot(data_algo['Intervalle'], data_algo["Execution_Time_ms"], label=algo, marker='o')
                has_data = True  # Mettre à jour l'indicateur si des données ont été tracées

        if has_data:
            plt.xlabel("Intervalle de Longueur de Mot")
            plt.ylabel("Temps d'Exécution (ms)")
            alphabet_size = donnees['Alphabet'].iloc[0]
            plt.title(f"Comparaison des Algorithmes pour l'Alphabet de taille {alphabet_size}")
            plt.legend()
            plt.grid(True)

            # Sauvegarder le graphique
            output_file = os.path.join(GRAPH_DIR, f"graph_{alphabet_size}.png")
            plt.savefig(output_file)
            print(f"Graphique sauvegardé dans {output_file}")
        else:
            print(f"Aucune donnée tracée pour le fichier {fichier}")
        plt.close()