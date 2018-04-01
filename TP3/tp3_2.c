#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int global = 565;

void privateScope(){
	printf("Entrée : %d\n", global);
	
	int i = 0;
	#pragma omp parallel for
	for(i = 0; i < 10000; i++) {
		global++;
	}

	printf("Sortie : %d\n", global);	
}

void privateScope1(){
	printf("Entrée : %d\n", global);
	
	int i = 0;
	/* global est modifiée que dans la boucle, mais comme est déclarée en private, après le for elle reprend sa valeur initiale */
	#pragma omp parallel for private(global)
	for(i = 0; i < 1000000; i++) {
		global++;
	}
	printf("Sortie : %d\n", global);	
}

void privateScope2_sub(){
	global++;
}

void privateScope2(){
	printf("Entrée : %d\n", global);
	
	int i = 0;
	/* global est privée dans le for mais non dans privateScope2_sub() donc elle est modifiée en sortie. La privacité n'est appliquée que dans le corps */
	#pragma omp parallel for lastprivate(global)
	for(i = 0; i < 1000000; i++) {
		//privateScope2_sub();
		global++;
	}
	printf("Sortie : %d\n", global);	
}

int main (int argc, char ** argv){

	privateScope2();

	return 0;
}
