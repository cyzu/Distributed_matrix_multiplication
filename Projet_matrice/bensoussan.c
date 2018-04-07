#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

//brew install valgrind -HEAD
// Au début que le P0 connait les matrices et leur taille.

struct Matrice{
	int ligne, colonne;
	int **matrice;
};


/************************************************/
/************************************************/
/*************** FONCTIONS UTILES ***************/
/************************************************/
/************************************************/

void allocation_matrice(struct Matrice * m){
	int i;
	m->matrice = malloc(sizeof(int*) * m->ligne);
	for(i = 0; i < m->ligne; i++){
		m->matrice[i] = malloc(sizeof(int) * m->colonne);
	}
}

void afficher_matrice(struct Matrice *m, char *string){
    int i, j;
    printf("\n\n---- %s ----\n", string);
    for(i = 0; i < m->ligne; i++){
        for(j = 0; j < m->colonne; j++){
            if (j == 0){
                printf("%d", m->matrice[i][j]);
            }else {
                printf(" %d", m->matrice[i][j]);
            }
        }
        printf("\n");
    }
}

/* Fonction qui extracte la colonne c de m dans destination
 
 DESTINATION == UNE SEULE COLONNE ?? extaction de plusieurs colonne ??
 destination doit être alloué au préalable.
 pour l'instant extraction d'une seule colonne !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
void extraction_colonne(struct Matrice * m, struct Matrice * destination, int c){
    int i;
    for (i = 0; i < m->ligne; i++) {
        destination->matrice[i][0] = m->matrice[i][c];
    }
}
// extraction de la ligne de m dans destination. Pour l'instant une seule ligne
void extraction_ligne(struct Matrice * m, struct Matrice * destination, int l){
    int i;
    for (i = 0; i < m->ligne; i++) {
        destination->matrice[i][0] = m->matrice[l][i];
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

void generation_matrice_zero(struct Matrice * m){
	int i, j;
	
	m->ligne = 3;
	m->colonne = 3;
	
	allocation_matrice(m);
	
	for(i = 0; i < m->ligne; i++){
		for(j = 0; j < m->colonne; j++){
			m->matrice[i][j] = 0; //(i+1) * (j+1);
		}
	}	
}

void generation_matrice(struct Matrice * m){
	int i, j, valeur = 1;
	
	m->ligne = 3;
	m->colonne = 3;
	
	allocation_matrice(m);
	
	for(i = 0; i < m->ligne; i++){
		for(j = 0; j < m->colonne; j++){
            m->matrice[i][j] = valeur;//i+j; //(i+1) * (j+1);
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
	generation_matrice(A);
	afficher_matrice(A, "A");

	struct Matrice *B = malloc(sizeof(struct Matrice));
	generation_matrice(B);
	afficher_matrice(B, "B");

	struct Matrice *C = malloc(sizeof(struct Matrice));
	generation_matrice_fixe(C, A->ligne, B->colonne);
	produit_matriciel(C, A, B);
	afficher_matrice(C, "C");
    
    
    
    // test extraction colonne de matrice A
    struct Matrice *colonne1 = malloc(sizeof(struct Matrice));
    generation_matrice_fixe(colonne1, A->ligne, 1);
    struct Matrice *colonne2 = malloc(sizeof(struct Matrice));
    generation_matrice_fixe(colonne2, A->ligne, 1);
    struct Matrice *colonne3 = malloc(sizeof(struct Matrice));
    generation_matrice_fixe(colonne3, A->ligne, 1);
    
    extraction_colonne(A, colonne1, 0);
    extraction_colonne(A, colonne2, 1);
    extraction_colonne(A, colonne3, 2);
    afficher_matrice(colonne1, "Colonne1 de A");
    afficher_matrice(colonne2, "Colonne2 de A");
    afficher_matrice(colonne3, "Colonne3 de A");
    
    extraction_ligne(A, colonne1, 0);
    extraction_ligne(A, colonne2, 1);
    extraction_ligne(A, colonne3, 2);
    afficher_matrice(colonne1, "Ligne1 de A");
    afficher_matrice(colonne2, "Ligne2 de A");
    afficher_matrice(colonne3, "Ligne3 de A");
    
	return 0;
}
