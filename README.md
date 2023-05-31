# Polytech Lille - Projet GC 2023

Code possible et remplissant entièrement le cahier des charges du sujet de projet de graphes et combinatoires de Polytech Lille pour les IS3 en 2023.
Ce repo vise à aider les élèves des prochaines années en cas de difficultés sur un sujet similaire.

## Contenu

* Rapport final et intermédiaire au format pdf et tex
* Fichier README
* Fichiers sources du projet dans le dossier src
* Fichiers de test dans le dossier ressources

## Utilisation 

Pour utiliser le programme, merci de se placer dans le dossier 'src'.

### Compilation classique

Série de commandes pour un lancement classique du programme :

```
 $ gcc -c *.c
 $ gcc *.o
```

### Compilation orienté débogage

Série de commandes optimisés pour le débogage du programme et les test de valgrind :

``` 
 $ gcc -c -g -Wall -pass-exit-codes *.c
 $ gcc -g -Wall -pass-exit-codes *.o
```

### Lancement orienté performance

Ce lancement optimise le temps d'éxecution du programme et n'affiche que la valeur optimale du graphe.

```
 $ ./a.out "../ressources/nom_du_fichier.txt"  
```

### Lancement orienté débogage et informations

Ce lancement affiche toutes les données du graphe (Table Head, Succ, Pred, date au plus tôt, au plus tard, tâches critiques et marge si non critique) (ajout du paramètre d dans la commande).

```
 $ ./a.out "../ressources/nom_du_fichier.txt" d
```

### Lancement Valgrind

Commande utilisé pour tester les fuites de mémoires et les accès mémoire non correct avec Valgrind (la compilation orienté débogage est recommandée) :
```
 $ valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./a.out "../ressources/nom_du_fichier.txt"
```

## Fichiers disponibles 

Les fichiers de test sont présent dans le dossier ressources.

* "Ex_300000.txt": graphe de 300 000 points
* "Ex_TD.txt": graphe de l'exemple de TD
* "ordo30.txt": graphe de 30 points
* "ordo120.txt": graphe de 120 points
* "Sondage.txt": graphe de l'exemple du sujet
