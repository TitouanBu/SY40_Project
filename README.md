# SY40_Project

## Utilisation de notre programme 
- Lancer la commande "make clean" puis "make mrproper" pour nettoyer les différents exécutables du dossier 
- Lancer la commande "make" pour compiler notre projet 
- Lancer le programme avec la commande "./prog"

## Attention 
Pour un meilleur affichage et donc une meilleure compréhension du déroulement du programme, l'utilisation dans un terminal en plein écran est recommandée.

## Les options lors du lancement 
- Le programme peut simplement être lancé sans arguments ("./prog"). Les valeurs par défaut seront alors : Heure de début 14h00 et Nombre de places pour abonné = 2 et Nombre de places pour non abonné = 4.
- On peut aussi quatres arguments :
	- Le premier représente le Nombre de place abonné que l'on souhaite
	- Le deuxième représente le Nombre de place non-abonné que l'on souhaite
	- Le troisième représente l'heure à laquelle on souhaite démarrer la simulation
	- Le quatrième représente les minutes dans l'heure
Exemple : ./prog a b c d --> lancera le programme avec (Nombre Place abonné = a | Nombre Place non-abonné = b | Heure de début : c heures d minutes)

