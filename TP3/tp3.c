#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void parallelRegion(){
	#pragma omp parallel
	printf("Bienvenue dans la fonction PARALLELREGION() se trouvant dans le code C écrit pour le TP3\n");
}

void parallelRegionForOutside(){
	#pragma omp parallel
	printf("Wilkommen_%d\n",omp_get_thread_num());
	
	int i = 0;
	for(i = 0; i < 20; i++) printf("Welcome_%d\n", i);
}

void parallelRegionForInside(){
/* La région parallele est executée le nombre de thread fois */
	#pragma omp parallel 
	{
		printf("_WILKOMMEN_%d\n",omp_get_thread_num());
	
		int i = 0;
		for(i = 0; i < 20; i++) printf("Welcome_%d.%d\n", omp_get_thread_num(),i);
	}
	
}

void  parallelRegionForInsideWithPragma(){
	#pragma omp parallel 
	{
		printf("_WILKOMMEN_%d\n",omp_get_thread_num());
	
		int i = 0;
		/* Divise la boucle for en nombre de thread et chaque thread effectue une partie de la boucle (si il y a 10 threads, chaque thread fait 2 opérations) */
		#pragma omp for
		for(i = 0; i < 10000; i++) printf("Welcome_%d.%d\n", omp_get_thread_num(),i);
	}
}

int main (int argc, char** argv){
	//parallelRegionForInside();
	parallelRegionForInsideWithPragma();
	
	return 0;
}

// Après une pragma, l'instruction qui suit est en parallele mais pas le code qui suit
/* c) Toute la section est executée en parallèle donc l'affiche n'est pas dons l'ordre

*/
