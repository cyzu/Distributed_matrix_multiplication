#include <stdlib.h>
#include <stdio.h>
//#include <omp.h>
#include <math.h>

struct tablo {
  int * tab;
  int size;
};

void printArray(struct tablo * tmp) {
  printf("---- Array of size %i ---- \n", tmp->size);
  int size = tmp->size;
  int i;
  for (i = 0; i < size; ++i) {
    printf("%i ", tmp->tab[i]);
  }
  printf("\n");
}

struct tablo * allocateTablo(int size) {
  struct tablo * tmp = malloc(sizeof(struct tablo));
  tmp->size = size;
  tmp->tab = malloc(size*sizeof(int));
  return tmp;
}

void montee(struct tablo * source, struct tablo * destination) {
  // remplissage du tableau destination de taille 2*n en
  // copiant les données du tableau source dans destination, 
  // à la bonne position
  // on suppose que le malloc de destination a été fait avant

	int i;
	for (i = 0; i < source->size ; i++){
		destination->tab[i+source->size] = source->tab[i];
	} 

  // Boucle de calcul pour la montée dans l'arbre/tableau
	int l;
	for(l = (log(source->size)/log(2))-1; l > -1; l--){
		for(i = pow(2,l); i < pow(2,l+1); i++){
			destination->tab[i] = destination->tab[2*i] + destination->tab[2*i + 1];
		}
	}
}

void descente(struct tablo * a, struct tablo * b) {
  // Implementation de la phase de descente
    int k, i;
    b->tab[1] = 0;
    for (k = 1; k < (log(a->size)/log(2)); k++) {
        //En parallele ↓
        for (i = pow(2, k); i < pow(2, k+1); i++) {
            if (i%2 == 0) b->tab[i] = b->tab[i/2];
            else b->tab[i] = b->tab[i/2] + a->tab[i-1];
        }
    }
}

void final(struct tablo * a, struct tablo *b) {
  // Phase finale
    int i;
    int l = log(b->size)/log(2) - 1;
    // En parallele ↓
    for (i = pow(2, l); i < pow(2, l+1); i++) {
        b->tab[i] = b->tab[i] + a->tab[i];
    }
}

void generateArray(struct tablo * s) {
  //construction d'un tableau pour tester
  s->size=8;
  s->tab=malloc(s->size*sizeof(int));
  s->tab[0]=3;
  s->tab[1]=1;
  s->tab[2]=7;
  s->tab[3]=0;
  s->tab[4]=4;
  s->tab[5]=1;
  s->tab[6]=6;
  s->tab[7]=3;
}

// gcc -Wall -o tp3_3 tp3_3.c -lm
int main(int argc, char **argv) {
  struct tablo source;

  generateArray(&source);

  struct tablo * a = allocateTablo(source.size*2);
  montee(&source, a);
  printArray(a);

  struct tablo * b = allocateTablo(source.size*2);
  descente(a, b);
  printArray(b);
   
  final(a,b);
  printArray(b);

  return 0;
}

