#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

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

/* Fonction qui échange les lignes et les colonnes de la matrice m dans destination */
void echange_ligne_colonne(const struct Matrice * m, struct Matrice * destination){
    int i, j;
    for (i = 0; i < m->ligne; i++) {
        for(j = 0; j < m->colonne; j++){
            // destination->matrice[i] = m->matrice[i*m->ligne + c]; // extraction de colonne
            destination->matrice[i*m->colonne + j] = m->matrice[j*m->ligne + i];
        }   
    }
}





void produit_matriciel(struct Matrice * c, const struct Matrice * a, const struct Matrice * b){
    int i, j, k;
    for(i = 0; i < a->ligne; i++){
        for(j = 0; j < b->colonne; j++){
            for(k = 0; k < b->ligne; k++){
                c->matrice[i*a->ligne + j] = c->matrice[i*a->ligne + j] + a->matrice[i*a->colonne + k] * b->matrice[j + k*b->colonne];
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

    int numprocs, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
    int i, send_error, procs;
    int size = 3;

    /* Matrice C = A * B */
    struct Matrice *A = malloc(sizeof(struct Matrice));
    struct Matrice *B = malloc(sizeof(struct Matrice));    
    struct Matrice *extracte = malloc(sizeof(struct Matrice));
    struct Matrice *tmpA = malloc(sizeof (struct Matrice));
    struct Matrice *tmpB = malloc(sizeof (struct Matrice));
    // struct Matrice *tmpResultat = malloc(sizeof (struct Matrice));
    struct Matrice *tmpCalcul = malloc(sizeof (struct Matrice));

    allocation_matrice(A, size, size);
    allocation_matrice(B, size, size);

    /* Création du Datatypen*/
    MPI_Datatype Ligne_TYPE;
    MPI_Type_contiguous(A->colonne, MPI_LONG_LONG, &Ligne_TYPE);
    MPI_Type_commit(&Ligne_TYPE);
	
    if (numprocs >= size){
        /* Allocation de tmpA et tmpB */
        allocation_matrice(tmpA, 1, A->colonne);
        allocation_matrice(tmpB, B->ligne, 1);
        generation_matrice_zero(tmpA);
        generation_matrice_zero(tmpB);

        allocation_matrice(extracte, B->ligne, B->colonne);
        // allocation_matrice(tmpResultat, tmpA->ligne, tmpB->colonne); // stocke le résultat de tous les calculs pour pouvoir Gather
        allocation_matrice(tmpCalcul, tmpA->ligne, tmpB->colonne);   // stocke le résultat après chaque calcul puis on le met dans la matrice tmpResultat

        if (rank == 0){
            struct Matrice *C = malloc(sizeof(struct Matrice));
            allocation_matrice(C, A->ligne, B->colonne);

            generation_matrice_incr(A);
            generation_matrice_incr(B);
            echange_ligne_colonne(B, extracte);

            afficher_matrice(A, "A", 111);
            afficher_matrice(B, "B", 111);
            // afficher_matrice(extracte, "extracte (echange de lignes/colonnes de B)", -111);
        }
        
    }
    else if (size % numprocs == 0){
        //cas où il y a le même nombre de lignes et colonnes pour chaque processus
        // Nombre de ligne multiple du nombre de processus
    }
    else{
        // cas le plus compliqué ! tous les processeurs n'ont pas le même nombre de lignes et colonnes en charge
        // moins de processus que de ligne et ne sont pas multiple
    }













    /* Scatter envoie une ligne par processus */
    send_error = MPI_Scatter(&A->matrice[0], 1, Ligne_TYPE, &tmpA->matrice[0], 1, Ligne_TYPE, 0, MPI_COMM_WORLD);
    if (send_error != MPI_SUCCESS){
        printf("Erreur lors de l'envoi de ligne vers chaque processus avec le scatter");
        exit(1);
    } 
    /* Scatter envoie une colonne par processus */
    send_error = MPI_Scatter(&extracte->matrice[0], 1, Ligne_TYPE, &tmpB->matrice[0], 1, Ligne_TYPE, 0, MPI_COMM_WORLD);
    if (send_error != MPI_SUCCESS){
        printf("Erreur lors de l'envoi de colonne vers chaque processus avec le scatter");
        exit(1);
    }

    produit_matriciel(tmpCalcul, tmpA, tmpB);

    //affichage du résultat
    for(procs = 0; procs < A->ligne; procs++){
        if(rank == procs){
            afficher_matrice(tmpA, "tmpA", rank);
            afficher_matrice(tmpB, "tmpB", rank);
            afficher_matrice(tmpCalcul, "tmpCalcul", rank);
        }
        sleep(1);
    }
































    MPI_Type_free(&Ligne_TYPE);
	MPI_Finalize();
}