#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct Matrice{
	int ligne, colonne;
	long long int *matrice;
};


/************************************************/
/************************************************/
/*************** FONCTIONS UTILES ***************/
/************************************************/
/************************************************/

void allocation_matrice(struct Matrice * m, const int l, const int c){
    m->ligne = l;
    m->colonne = c;

	m->matrice = malloc(sizeof(long long int*) * m->ligne * m->colonne);
}

void afficher_matrice(const struct Matrice *m, const char *string, const int integer){
    int i;
    printf("\n\n---- %s ----%d\n", string, integer);
    for(i = 0; i < m->ligne * m->colonne; i++){
        if (i % m->colonne == 0){
            // Si c'est la première valeur de la ligne
            printf("%lld", m->matrice[i]);
        }else {
            // tous les autres
            printf(" %lld", m->matrice[i]);
        }
        if(i % m->colonne == m->colonne-1){
            // sauter une ligne à chaque fin de ligne
            printf("\n");
        }
    }
}

// matrice[i,j]  get(matrice,i,j))
int get_case(const struct Matrice * m, int i, int j){
	 // return i * m->ligne + j;
	 return i * m->colonne + j;
}

void produit_matriciel(struct Matrice * c, const struct Matrice * a, const struct Matrice * b){
    int i, j, k;
    for(i = 0; i < a->ligne; i++){
        for(j = 0; j < b->colonne; j++){
            for(k = 0; k < b->ligne; k++){
                // c->matrice[i*a->ligne + j] = c->matrice[i*a->ligne + j] + a->matrice[i*a->colonne + k] * b->matrice[j + k*b->colonne];
                c->matrice[get_case(c, i, j)] = c->matrice[get_case(c, i, j)] + a->matrice[get_case(a, i, k)] * b->matrice[get_case(b, k, j)];
            }
        }
    }
}

/************************************************/
/************************************************/
/************ GENERATEURS DE MATRICE ************/
/************************************************/
/************************************************/

void generation_matrice_zero(struct Matrice * m){
    int i;
	for(i = 0; i < m->ligne * m->colonne; i++){
		m->matrice[i] = 0;
	}	
}

void generation_matrice_incr(struct Matrice * m){
	int i, valeur = 1;
	
	for(i = 0; i < m->ligne * m->colonne; i++){
        m->matrice[i] = (i+1) * (valeur+1);
        valeur++;
	}
}


int main (int argc, char *argv[]){

	 int size = 4;
	 
	 /* Matrice C = A * B */
    struct Matrice *A = malloc(sizeof(struct Matrice));
    struct Matrice *B = malloc(sizeof(struct Matrice));
    struct Matrice *C = malloc(sizeof(struct Matrice));
    allocation_matrice(A, size, size);
    allocation_matrice(B, size, size);
    allocation_matrice(C, A->ligne, B->colonne);

	generation_matrice_incr(A);
   generation_matrice_incr(B);
            
	produit_matriciel(C, A, B);
	
	afficher_matrice(A, "A", 123);
	afficher_matrice(B, "B", 117);
	afficher_matrice(C, "C", 373);
	

	return 0;
}