#include <stdlib.h>
#include <stdio.h>

struct Matrice{
 int ligne, colonne;
 long long int **matrice;
};


 /************************************************/
 /************************************************/
 /*************** FONCTIONS UTILES ***************/
 /************************************************/
 /************************************************/

 void allocation_matrice(struct Matrice * m){
 	int i;
 	m->matrice = malloc(sizeof(long long int*) * m->ligne);
 	for(i = 0; i < m->ligne; i++){
 		m->matrice[i] = malloc(sizeof(long long int) * m->colonne);
 	}
 }

 void afficher_matrice(struct Matrice *m, char *string){
     int i, j;
     printf("\n\n---- %s ----\n", string);
     for(i = 0; i < m->ligne; i++){
         for(j = 0; j < m->colonne; j++){
             if (j == 0){
                 printf("%lld", m->matrice[i][j]);
             }else {
                 printf(" %lld", m->matrice[i][j]);
             }
         }
         printf("\n");
     }
 }

 void produit_matriciel(struct Matrice * c, struct Matrice * a, struct Matrice * b){
     int i, j, k;
     for(i = 0; i < a->colonne; i++){
         for(j = 0; j < b->ligne; j++){
             for(k = 0; k < b->ligne; k++){
                 c->matrice[i][j] = c->matrice[i][j] + a->matrice[i][k] * b->matrice[k][j];
             }
         }
     }
 }


 /************************************************/
 /************************************************/
 /************ GENERATEURS DE MATRICE ************/
 /************************************************/
 /************************************************/

 void generation_matrice_zero(struct Matrice * m, int size){
 	int i, j;

  m->ligne = size;
 	m->colonne = size;

 	allocation_matrice(m);

 	for(i = 0; i < m->ligne; i++){
 		for(j = 0; j < m->colonne; j++){
 			m->matrice[i][j] = 0; //(i+1) * (j+1);
 		}
 	}
 }

 void generation_matrice(struct Matrice * m, int size){
 	int i, j;
  long long int valeur = 1;

  m->ligne = size;
  m->colonne = size;
 	allocation_matrice(m);

 	for(i = 0; i < m->ligne; i++){
 		for(j = 0; j < m->colonne; j++){
             m->matrice[i][j] = valeur;//i+j; //(i+1) * (j+1);
             valeur++;
 		}
 	}
 }

 void generation_matrice_incr(struct Matrice * m, int size){
 	int i, j;
  long long int valeur = 1;

 	m->ligne = size;
 	m->colonne = size;
  allocation_matrice(m);

 	for(i = 0; i < m->ligne; i++){
    for(j = 0; j < m->colonne; j++){
        m->matrice[i][j] = valeur * (valeur+1);
        valeur++;
    }
 	}
 }

 void generation_matrice_fixe(struct Matrice * m, int l, int c){
     int i, j;

     m->ligne = l;
     m->colonne = c;
     allocation_matrice(m);

     for(i = 0; i < m->ligne; i++){
         for(j = 0; j < m->colonne; j++){
             m->matrice[i][j] = 0;
         }
     }
 }


int main (int argc, char *argv[]){
   struct Matrice *A = malloc(sizeof(struct Matrice));
 	 generation_matrice_incr(A, 4);
 	 afficher_matrice(A, "A");

 	 struct Matrice *B = malloc(sizeof(struct Matrice));
 	 generation_matrice_incr(B, 4);
 	 afficher_matrice(B, "B");

 	 struct Matrice *C = malloc(sizeof(struct Matrice));
 	 generation_matrice_fixe(C, A->ligne, B->colonne);
 	 produit_matriciel(C, A, B);
 	 afficher_matrice(C, "C");

 	 return 0;
}
