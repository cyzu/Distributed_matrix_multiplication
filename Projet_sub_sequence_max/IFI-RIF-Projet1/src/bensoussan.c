/******************************************/
/* Projet OpenMp : sous-séquence maximale */
/*          Master 1 Informatique         */
/*                                        */
/*            Chloé BENSOUSSAN            */
/******************************************/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <limits.h>

struct tablo {
    long long * tab;
    int size;
};

void printArray(struct tablo * tmp, char *string) {
    printf("\n---- %s ---- \n", string);
    int size = tmp->size;
    int i;
    for (i = 0; i < size; ++i) {
        printf("%lld ", tmp->tab[i]);
    }
    printf("\n");
}

struct tablo * allocateTablo(int size) {
    struct tablo * tmp = malloc(sizeof(struct tablo));
    tmp->size = size;
    tmp->tab = malloc(size*sizeof(long long int));
    tmp->tab[0] = 0;
  
    return tmp;
}

long long max(long long a, long long b){
    if (a > b) {
        return a;
    }
    return b;
}

long long max_tablo(struct tablo * t){
    int i;
    long long max = t->tab[0];
    for(i = 0; i < t->size; i++){
        if (t->tab[i] > max){
            max = t->tab[i];
        }
    }
    return max;
}


/*****************************************************************************************/
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
	int l, imax;;
	for(l = (log(source->size)/log(2))-1; l > -1; l--){
	    imax = pow(2, l+1);
	    #pragma omp parallel for
		for(i = pow(2,l); i < imax; i++){
			destination->tab[i] = destination->tab[2*i] + destination->tab[2*i + 1];
		}
	}
}

void montee_max(struct tablo * source, struct tablo * destination) {
    // remplissage du tableau destination de taille 2*n en
    // copiant les données du tableau source dans destination, 
    // à la bonne position
    // on suppose que le malloc de destination a été fait avant

	int i;
	for (i = 0; i < source->size ; i++){
		destination->tab[i+source->size] = source->tab[i];
	} 

    // Boucle de calcul pour la montée dans l'arbre/tableau
	int l, imax;;
	for(l = (log(source->size)/log(2))-1; l > -1; l--){
	    imax = pow(2, l+1);
	    #pragma omp parallel for
		for(i = pow(2,l); i < imax; i++){
			destination->tab[i] = max(destination->tab[2*i], destination->tab[2*i + 1]);
		}
	}
}

void montee_inversee(struct tablo * source, struct tablo * destination) {
	int i;
	for (i = source->size-1; i >= 0; i--){
		destination->tab[destination->size-i-1] = source->tab[i];
	} 

    // Boucle de calcul pour la montée dans l'arbre/tableau
	int l, imax;
	for(l = (log(source->size)/log(2))-1; l > -1; l--){
	    imax = pow(2, l+1);
	    #pragma omp parallel for
		for(i = pow(2,l); i < imax; i++){
			destination->tab[i] = destination->tab[2*i] + destination->tab[2*i + 1];
		}
	}
}

void montee_inversee_max(struct tablo * source, struct tablo * destination) {
	int i;
	for (i = source->size-1; i >= 0; i--){
		destination->tab[destination->size-i-1] = source->tab[i];
	} 

    // Boucle de calcul pour la montée dans l'arbre/tableau
	int l, imax;
	for(l = (log(source->size)/log(2))-1; l > -1; l--){
	    imax = pow(2, l+1);
	    #pragma omp parallel for
		for(i = pow(2,l); i < imax; i++){
			destination->tab[i] = max(destination->tab[2*i], destination->tab[2*i + 1]);
		}
	}
}


void descente(struct tablo * a, struct tablo * b) {
    int k, i, imax;
    b->tab[1] = 0;
    
    for (k = 1; k < (log(a->size)/log(2)); k++) {
        imax = pow(2, k+1);
        #pragma omp parallel for
        for (i = pow(2, k); i < imax; i++) {
            if (i%2 == 0) {
                b->tab[i] = b->tab[i/2];
            }
            else {
                b->tab[i] = b->tab[i/2] + a->tab[i-1];
            }
        }
    }
}

void descente_max(struct tablo * a, struct tablo * b) {
    int k, i, imax;
    b->tab[1] = LLONG_MIN;
    
    for (k = 1; k < (log(a->size)/log(2)); k++) {
        imax = pow(2, k+1);
        #pragma omp parallel for
        for (i = pow(2, k); i < imax; i++) {
            if (i%2 == 0) {
                b->tab[i] = b->tab[i/2];
            }
            else {
                 b->tab[i] = max(b->tab[i/2], a->tab[i-1]);
            }
        }
    }
}

void final(struct tablo * a, struct tablo *b) {
    int l = log(b->size)/log(2) - 1;
    int i, imax = pow(2, l+1);
    
    #pragma omp parallel for
    for (i = pow(2, l); i < imax; i++) {
        b->tab[i] = b->tab[i] + a->tab[i];
    }
}

void final_max(struct tablo * a, struct tablo *b) {
    int l = log(b->size)/log(2) - 1;
    int i, imax = pow(2, l+1);
    
    #pragma omp parallel for
    for (i = pow(2, l); i < imax; i++) {
        b->tab[i] = max(b->tab[i], a->tab[i]);
    }
}
/*****************************************************************************************/


void sum_prefix(struct tablo *source, struct tablo * psum){
    int i;
    struct tablo * a = allocateTablo(source->size*2);
    struct tablo * b = allocateTablo(source->size*2);
  
    montee(source, a);
    descente(a, b);
    final(a,b);
   
    #pragma omp parallel for
    for (i = 0; i < psum->size; i++){
        psum->tab[i] = b->tab[i+source->size];
    }
}

void sum_suffix(struct tablo *source, struct tablo * ssum){
    int i;
    struct tablo * a = allocateTablo(source->size*2);
    struct tablo * b = allocateTablo(source->size*2);
  
    montee_inversee(source, a);
    descente(a, b);
    final(a,b);
   
    #pragma omp parallel for
    for (i = 0; i < ssum->size; i++){
        ssum->tab[i] = b->tab[b->size-i-1];
    }
}

void max_suffix(struct tablo * source, struct tablo * smax){
    int i;
    struct tablo * a = allocateTablo(source->size*2);
    struct tablo * b = allocateTablo(source->size*2);
  
    montee_inversee_max(source, a);
    descente_max(a, b);
    final_max(a,b);
   
    #pragma omp parallel for
    for (i = 0; i < smax->size; i++){
        smax->tab[i] = b->tab[b->size-i-1];
    }
}

void max_prefix(struct tablo *source, struct tablo * pmax){
    int i;
    struct tablo * a = allocateTablo(source->size*2);
    struct tablo * b = allocateTablo(source->size*2);
  
    montee_max(source, a);
    descente_max(a, b);
    final_max(a,b);
   
    #pragma omp parallel for
    for (i = 0; i < pmax->size; i++){
        pmax->tab[i] = b->tab[i+source->size];
    }
}

void sum_subsequence(struct tablo *source, struct tablo *pmax, struct tablo *ssum, struct tablo *smax, struct tablo * psum, struct tablo *m){
    int i;
    struct tablo * ms = allocateTablo(source->size);
    struct tablo * mp = allocateTablo(source->size);
    
    #pragma omp parallel for
    for(i = 0; i < m->size; i++){
        ms->tab[i] = pmax->tab[i] - ssum->tab[i] + source->tab[i];
        mp->tab[i] = smax->tab[i] - psum->tab[i] + source->tab[i];
        m->tab[i] = ms->tab[i] + mp->tab[i] - source->tab[i];
    }
}

void find_subsequence_max(struct tablo * source, struct tablo * m){
    int i;
    int sub_sequence = 1; // boolean si on a trouvé la sous-sequence max (pour éviter les multiples sous-sequence max)
    long long max_subsequence_sum = max_tablo(m);
    
    printf("%lld", max_subsequence_sum);
    
    for (i = 0; i < m->size; i++){
        if (m->tab[i] == max_subsequence_sum){
            printf(" %lld", source->tab[i]);
            sub_sequence = 0;
        }
        if (sub_sequence == 0 && m->tab[i] != max_subsequence_sum){
            printf("\n");
            return;
        }
    }
    printf("\n");
}

void generateArray(struct tablo * s) {
    //construction d'un tableau pour tester
    s->size = 16;
    s->tab = malloc(s->size*sizeof(long long));
  
    s->tab[0] = 3;
    s->tab[1] = 2;
    s->tab[2] = -7;
    s->tab[3] = 11;
    s->tab[4] = 10;
    s->tab[5] = -6;
    s->tab[6] = 4;
    s->tab[7] = 9;
    s->tab[8] = -6;
    s->tab[9] = 1;
    s->tab[10]= -2;
    s->tab[11]= -3;
    s->tab[12]= 4;
    s->tab[13]= -3;
    s->tab[14]= 0;
    s->tab[15]= 2;
}

void generateArray_from_file(struct tablo * s, char * file_name){
    FILE *file = fopen(file_name, "r");
    int i, tablo_size = 0;
    long long valeur;
    
    if (file == NULL) {
        printf("Erreur : l'ouverture du fichier '%s' a échoué.\n", file_name);
        exit(1);
    }
    // compteur de nombre d'entier pour connaitre la taille du tableau à allouer    
    while (fscanf(file, "%lld", &valeur) != EOF){
        tablo_size++;
    }
    // Remettre le curseur au début du fichier
    rewind(file); 
    
    // Allocation dynamique du tableau
    s->size = tablo_size;
    s->tab = malloc(s->size*sizeof(long long));
    
    // Insersion des valeurs
    for(i = 0; i < s->size; i++){
        fscanf(file, "%lld", &valeur);
        s->tab[i] = valeur;
    }
    fclose(file);
}

int main(int argc, char **argv) {
    struct tablo source;
    
    if (argc == 1){
        generateArray(&source);
    }
    else {
        generateArray_from_file(&source, argv[1]);
    }
    
    struct tablo * psum = allocateTablo(source.size);
    sum_prefix(&source, psum);
    //printArray(psum, "psum");

    struct tablo * ssum = allocateTablo(source.size);
    sum_suffix(&source, ssum);
    //printArray(ssum, "ssum");

    struct tablo * smax = allocateTablo(source.size);
    max_suffix(psum, smax);
    //printArray(smax, "smax");

    struct tablo * pmax = allocateTablo(source.size);
    max_prefix(ssum, pmax);
    //printArray(pmax, "pmax");

    struct tablo * m = allocateTablo(source.size);
    sum_subsequence(&source, pmax, ssum, smax, psum, m);
    //printArray(m, "m");
  
    find_subsequence_max(&source, m);
    
    return 0;
}
