# Parallélisme et Distribution
Master 1 Informatique

Université de Nice Sophia-Antipolis

## Professeur
*Fabrice Huet*

Chercheur au CNRS à Sophia Antipolis

## Cours
Vous pouvez accéder aux cours et tps depuis ce [lien](https://sites.google.com/site/fabricehuet/teaching/parallelisme-et-distribution).

## Librairie
J'utilise la librairie **omp.h** (openMP) pour lancer des instructions en parallèles.

## Projet
Trouver la sous-séquence maximale à partir d'une séquence d'entiers positifs et/ou négatifs ([ici](https://sites.google.com/site/fabricehuet/teaching/parallelisme-et-distribution/sous-sequence-maximale)).

⇢ Sous séquence maximale ayant la somme des éléments maximale

Pour chaque étape de l'algorithme, j'utilise un algorithme en 3 phases :
  - 1 ère phase **la montée**
  - 2 ème phase **la descente**
  - 3 ème phase **finale**
##### Limites du projet
- Le programme lit les valeurs à partir d'un **fichier texte** passé en paramètre
- La séquence doit contenir **2^n élements**
- Chaque élement doit être des entiers (entre **INT_MIN et INT_MAX**)
