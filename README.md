# Parallélisme et Distribution
Master 1 Informatique

Université de Nice Sophia-Antipolis

## Professeur
*Fabrice Huet* - Chercheur au CNRS à Sophia Antipolis

## Cours
Vous pouvez accéder aux cours et tps depuis ce [lien](https://sites.google.com/site/fabricehuet/teaching/parallelisme-et-distribution).

## Librairie
1. Librairie **omp.h** (openMP) pour lancer des instructions en parallèle.
2. Librairie **mpi.h** (Message Passing Interface) pour l'échanges d'informations interprocesseurs.

## Projet 1
Trouver la sous-séquence maximale à partir d'une séquence d'entiers positifs et/ou négatifs ([ici](https://sites.google.com/site/fabricehuet/teaching/parallelisme-et-distribution/sous-sequence-maximale)).

⇢ Sous séquence maximale ayant la somme des éléments maximale.

Pour chaque étape de l'algorithme, j'utilise un algorithme en 3 phases :
  * 1 ère phase dite algorithme de **la montée**
  * 2 ème phase dite **la descente**
  * 3 ème phase dite **finale**
  
### Contraintes du projet

- Le programme lit les valeurs à partir d'un **fichier texte** passé en paramètre.
- La séquence doit contenir **2^n élements**.
- Chaque élement doit être des entiers (entre **INT_MIN et INT_MAX** inclus).

## Projet 2
Effectuer un produit matriciel en parallèle, en échangeant les informations entres processeurs ([ici](https://sites.google.com/site/fabricehuet/teaching/parallelisme-et-distribution/projet---produit-matriciel-distribue)).

1. C = A * B
2. Les processeurs sont disposés en anneau.
3. On répartis les lignes de A et les colonnes de B a chaque processeurs (en utilisant **MPI_Scatter**) puis effectuent un premier calcul.
4. Les processeurs s'échangeront les lignes de A reçus puis calculent.
5. Ils répètent cette opération jusqu'a avoir calculé toute une colonne de la matrice C.
6. A l'aide d'un gather on ramène toutes les colonnes calculés vers C et le produit matriciel utilisant plusieurs processeus est terminé !

### Contraintes du projet

- Les matrices initiales sont données en paramètre sous forme de **fichiers textes**.
- Ce sont des matrices carrées (de la forme **N*N**).
- Chaque valeur est de types **entiers**.
