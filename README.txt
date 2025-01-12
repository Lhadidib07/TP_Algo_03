**pour compiler : 
make
**(le script bash n'as pas marché car je suis sur windows donc vaux mieux avoir linux )
**j'ai directement executé ligne par ligne le contenu du script.sh comme ceci :
./genere-texte 50000 3 > texte3.txt
./genere-mots 10 5 25 3 > mots3.txt
./ac-matrice mots3.txt texte3.txt > res-ac-matrice
./ac-hachage mots3.txt texte3.txt > res-ac-hachage
diff res-ac-matrice res-ac-hachage

**sur Linux pour executer le script:
./script.sh 

!!!!!Résultats innatendu 
**************** Le nombre d'occurences trouvées par la matrice et le hashage différent ***************
