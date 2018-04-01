/* TP2 PROGRAMMATION PARALLELE
 
 Un programme openMP est executé par un unique processus.
 Il va créer des threads quand il rencontre une région parallèle.
 Chaque thread posside un id. (id == 0 c'est le maître)
 
 --> structure comme une PRAM
 
 Le nombre de thread est décidé avant d'entrer dans la région //
 
 Si il y a une région // dans une région // :
    - par défaut elle s'exécute en séquentielle
    - région // réentrante (on/off par option)
 
 Utilisation des pragmas pour les régions //
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>

int carre(int *tab, int n){
	int i = 0, nombre = 0;
	#pragma omp parallel for
		 for(i = 0; i < n; i++){
			tab[i] = tab[i] * tab[i];
			if (i == 0) {
				nombre = omp_get_num_threads();
			}
		}
	return nombre;
}

int main (int argc, char **argv){

	struct timeval start, end;

	if (argc < 2){
		printf("Usage : %s entier\n", argv[0]);
		exit (0);
	}

	int n = atoi(argv[1]);
	int i = 0, count;
	int *tab;

	srand(time(0));

	
	tab = malloc(n*sizeof(int));
	#pragma omp parallel for
	for (i = 0; i < n; i++){
		tab[i] = rand()%101;
	}
	
	gettimeofday(&start, NULL);
	count = carre(tab, n);
	gettimeofday(&end, NULL);

	 printf("%d %f\n", count, ((double)(end.tv_usec - start.tv_usec)/1000000 + (double)(end.tv_sec - start.tv_sec)));
	
	free(tab);

	return 0;
}

/*
 1) la première boucle for prend beaucoup plus de temps en l'exécutant parallèlement qu'en séquentiel.
      Cependant la fonction carré est très plus rapide en parallèle.


 Compiler : gcc -Wall --std=c99 -o tp2_mp tp2.c -fopenmp
 Changer nombre de threads : export OMP_NUM_THREADS=6

par défault 4 coeurs, 4 thread, + rapide == 4 threads
*/
