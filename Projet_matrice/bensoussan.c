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

// matrice[i,j]  get(matrice,i,j))
int get_case(const struct Matrice * m, int i, int j){
	 // return i * m->ligne + j;
	 return i * m->colonne + j;
}

/* Fonction qui échange les lignes et les colonnes de la matrice m dans destination */
void echange_ligne_colonne(const struct Matrice * m, struct Matrice * destination){
    int i, j;
    for (i = 0; i < m->ligne; i++) {
        for(j = 0; j < m->colonne; j++){
            // destination->matrice[i*m->colonne + j] = m->matrice[j*m->ligne + i];
            destination->matrice[get_case(destination, i, j)] = m->matrice[get_case(m, j, i)];
        }   
    }
}

/* Fonction qui copie la matrice m dans destination (allocation doit être fait avant) */
void copie_matrice(const struct Matrice * m, struct Matrice * destination){
    int i;
    for (i = 0; i < m->ligne * m->colonne; i++){
        destination->matrice[i] = m->matrice[i];
    }
}


/************************************************/
/************************************************/
/*************** PRODUIT MATRICIEL **************/
/************************************************/
/************************************************/

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


long long int produit_matriciel_par_case(const struct Matrice * a, const struct Matrice * b){
    int i, j, k;
    long long int c = 0;
    for(i = 0; i < a->ligne; i++){
        for(j = 0; j < b->colonne; j++){
            for(k = 0; k < b->ligne; k++){
                // c = c + a->matrice[i*a->colonne + k] * b->matrice[j + k*b->colonne];
                c = c + a->matrice[get_case(a, i, k)] * b->matrice[get_case(b, k, j)];
            }
        }
    }
    // printf("resultat calcul c = %d ********\n", c);
    return c;
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

/************************************************/
/************************************************/
/********************* MAIN *********************/
/************************************************/
/************************************************/

int main (int argc, char *argv[]){

    int numprocs, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
    int i, procs;
    int size = 3;
    int suivant;// = (rank + numprocs + 1) % numprocs;
    int precedent;// = (rank + numprocs - 1) % numprocs;

    /* Matrice C = A * B */
    struct Matrice *A = malloc(sizeof(struct Matrice));
    struct Matrice *B = malloc(sizeof(struct Matrice));    
    struct Matrice *extracte = malloc(sizeof(struct Matrice));
    struct Matrice *tmpA = malloc(sizeof (struct Matrice));
    struct Matrice *tmpB = malloc(sizeof (struct Matrice));
    struct Matrice *tmpResultat = malloc(sizeof (struct Matrice));
    struct Matrice *tmpCirculation = malloc(sizeof (struct Matrice));

    allocation_matrice(A, size, size);
    allocation_matrice(B, size, size);

    /* Création du Datatype */
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
        allocation_matrice(tmpResultat, tmpA->ligne, tmpA->colonne); // stocke le résultat de tous les calculs pour pouvoir Gather
        allocation_matrice(tmpCirculation, tmpA->ligne, tmpA->colonne); // stocke la matrice à envoyer pour ne pas écrire par dessus celle à envoyer

		  suivant = (rank + size + 1) % size;
        precedent = (rank + size - 1) % size;
    
        if (rank == 0){
            struct Matrice *C = malloc(sizeof(struct Matrice));
            allocation_matrice(C, A->ligne, B->colonne);

            generation_matrice_incr(A);
            generation_matrice_incr(B);
            echange_ligne_colonne(B, extracte);

            afficher_matrice(A, "A", 111);
            afficher_matrice(B, "B", 111);
        }
        
		  /* Scatter envoie une ligne  et une colonne par processus */
    		MPI_Scatter(&A->matrice[0], 1, Ligne_TYPE, &tmpA->matrice[0], 1, Ligne_TYPE, 0, MPI_COMM_WORLD);
    		MPI_Scatter(&extracte->matrice[0], 1, Ligne_TYPE, &tmpB->matrice[0], 1, Ligne_TYPE, 0, MPI_COMM_WORLD);
    		
    		free(extracte->matrice);
    		free(extracte);
    
			/* Calcul de chaque case et circulation des lignes */
    		for(i = 0; i < A->ligne; i++){
        		tmpResultat->matrice[(rank-i+A->ligne) % A->ligne] = produit_matriciel_par_case(tmpA, tmpB);      
        		
        		copie_matrice(tmpA, tmpCirculation);
        		if (rank == 0){
        			// le premier envoie puis recois
            		MPI_Send(&tmpCirculation->matrice[0], 1, Ligne_TYPE, suivant, 100+rank, MPI_COMM_WORLD);
            		MPI_Recv(&tmpA->matrice[0], 1, Ligne_TYPE, precedent, 100+precedent, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        		}
        		else {
        			// les autres reçoivent puis envoient
            		MPI_Recv(&tmpA->matrice[0], 1, Ligne_TYPE, precedent, 100+precedent, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            		MPI_Send(&tmpCirculation->matrice[0], 1, Ligne_TYPE, suivant, 100+rank, MPI_COMM_WORLD);
       		 }
    		}
			/* On récupère toute les lignes dans B */
			MPI_Gather(&tmpResultat->matrice[0], 1, Ligne_TYPE, &B->matrice[0], 1, Ligne_TYPE, 0, MPI_COMM_WORLD);
			if (rank == 0){
				echange_ligne_colonne(B, A);
				afficher_matrice(A, "C", rank);
			}           
    }
    else if (size % numprocs == 0){
        //cas où il y a le même nombre de lignes et colonnes pour chaque processus
        // Nombre de ligne multiple du nombre de processus
































		/* Scatter envoie une ligne  et une colonne par processus */
    		MPI_Scatter(&A->matrice[0], 1, Ligne_TYPE, &tmpA->matrice[0], 1, Ligne_TYPE, 0, MPI_COMM_WORLD);
    		MPI_Scatter(&extracte->matrice[0], 1, Ligne_TYPE, &tmpB->matrice[0], 1, Ligne_TYPE, 0, MPI_COMM_WORLD);
    		
    		free(extracte->matrice);
    		free(extracte);
    
			// circulation et calcul
    		for(i = 0; i < A->ligne; i++){
        		tmpResultat->matrice[(rank-i+A->ligne) % A->ligne] = produit_matriciel_par_case(tmpA, tmpB);
        		// printf("\ntour %d, rank %d, case %d resultat %lld\n", i, rank, (i+rank+A->ligne) % A->ligne, tmpResultat->matrice[(i+rank+A->ligne) % A->ligne]);
        		// afficher_matrice(tmpResultat, "tmpResultat", rank);
        		// afficher_matrice(tmpA, "tmpA", rank);
        
        		copie_matrice(tmpA, tmpCirculation);

        		if (rank == 0){
            		// printf("SEND****** Rank = %d, envoie à %d, avec tag %d\n", rank, suivant,  100+rank);
            		MPI_Send(&tmpCirculation->matrice[0], 1, Ligne_TYPE, suivant, 100+rank, MPI_COMM_WORLD);
            
            		// printf("RECV****** Rank = %d, recu de %d, avec tag %d\n", rank, precedent,  100+precedent);
            		MPI_Recv(&tmpA->matrice[0], 1, Ligne_TYPE, precedent, 100+precedent, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        		}
        		// else if(rank == numprocs)
        		else {
            		// printf("RECV****** Rank = %d, recu de %d, avec tag %d\n", rank, precedent,  100+precedent);
            		MPI_Recv(&tmpA->matrice[0], 1, Ligne_TYPE, precedent, 100+precedent, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            		// printf("SEND****** Rank = %d, envoie à %d, avec tag %d\n", rank, suivant,  100+rank);
            		MPI_Send(&tmpCirculation->matrice[0], 1, Ligne_TYPE, suivant, 100+rank, MPI_COMM_WORLD);
       		 }
        		//printf("****************** FIN UN TOUR **************************\n");
        			//sleep(1);
    			}
	
	
				/* On récupère toute les lignes dans B */
				MPI_Gather(&tmpResultat->matrice[0], 1, Ligne_TYPE, &B->matrice[0], 1, Ligne_TYPE, 0, MPI_COMM_WORLD);
	
				if (rank == 0){
					echange_ligne_colonne(B, A);
					afficher_matrice(A, "nouveau A", rank);
				} 










































    }
    else{
        // cas le plus compliqué ! tous les processeurs n'ont pas le même nombre de lignes et colonnes en charge
        // moins de processus que de ligne et ne sont pas multiple
    }
	 
	 free(A->matrice);
    free(A);
    free(B->matrice);
    free(B);
    MPI_Type_free(&Ligne_TYPE);
	 MPI_Finalize();
}