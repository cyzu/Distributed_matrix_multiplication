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
    printf("\n\n---- %s (%d x %d)----%d\n", string, m->ligne, m->colonne, integer);
    for (i = 0; i < m->ligne * m->colonne; i++){
			// printf("\n*** rank = %d    ", integer);
			// usleep(100000);
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
	 return i * m->colonne + j;
}

/* Fonction qui échange les lignes et les colonnes de la matrice m dans destination */
void echange_ligne_colonne(const struct Matrice * m, struct Matrice * destination){
    int i, j;
    for (i = 0; i < destination->ligne; i++) {
        for(j = 0; j < destination->colonne; j++){
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

void scatterv(int * lignes, int * dataCount, int * adresseData, int size, int numprocs){
	int i, partage = size;

	/* Calcul de nombre de ligne à envoyer par colonne (pour tous les cas)*/
	for (i = 0; i < numprocs; i++) {
		lignes[i] = partage / numprocs;
	}
	partage = partage % numprocs;
	for (i = 0; i < partage; i++) {
		lignes[i]++;
	}

	/* Calcul de nombre de cases à envoyer et leur adresse pour MPI_Scatterv */
	int accumulateur = 0;
	for(i = 0; i < numprocs; i++){
		adresseData[i] = accumulateur;
		dataCount[i] = lignes[i] * size;
		accumulateur += lignes[i] * size;
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


/*long long int produit_matriciel_par_case(const struct Matrice * a, const struct Matrice * b){
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
}*/

long long int produit_matriciel_par_case(const struct Matrice * a, const struct Matrice * b, const int ligne, const int colonne){
    int k;
    long long int c = 0;
		// printf("******** c = 0 ");
    for(k = 0; k < a->colonne; k++){
        c = c + a->matrice[get_case(a, ligne, k)] * b->matrice[get_case(b, k, colonne)];
				// printf("+= a[%d] * b[%d] ", get_case(a, ligne, k), get_case(b, k, colonne));
    }
		// printf("\n");
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

    int size = 4, i, j;

    /* Matrice C = A * B */
    struct Matrice *A = (struct Matrice *) malloc(sizeof(struct Matrice));
    struct Matrice *B = (struct Matrice *) malloc(sizeof(struct Matrice));
    struct Matrice *extracte = (struct Matrice *) malloc(sizeof(struct Matrice));

    /* Répartiton des lignes en fonction de la taille de la matrice et du nombre de processus */
    int dataCount[numprocs], adresseData[numprocs], lignes[numprocs];
    scatterv(lignes, dataCount, adresseData, size, numprocs);

    if (rank == 0){
        allocation_matrice(A, size, size);
        allocation_matrice(B, size, size);
        allocation_matrice(extracte, B->ligne, B->colonne);

        struct Matrice *C = (struct Matrice *) malloc(sizeof(struct Matrice));
        allocation_matrice(C, A->ligne, B->colonne);

        generation_matrice_incr(A);
        generation_matrice_incr(B);
        echange_ligne_colonne(B, extracte);

        afficher_matrice(A, "A", 111);
        afficher_matrice(B, "B", 111);

        A->ligne = lignes[0];	// je redimensionne la matrice A pour le scatterv (du root)
        B->colonne = lignes[0];

        printf("\n\nRépartition des lignes : \n");
        for(i = 0; i < numprocs; i++){
            printf("dataCount[%d] = %d     adresseData[%d] = %d\n", i, dataCount[i], i, adresseData[i]);
        }
    }
    else {
        allocation_matrice(A, lignes[0], size); // j'alloue les tableaux avec le nombre max de ligne qu'ils vont recevoir en circulation
        allocation_matrice(B, size, lignes[0]);
        A->ligne = lignes[rank];								// je définie les nombres de lignes correcte pour le premier tour pour tous les processus
        B->colonne = lignes[rank];
        generation_matrice_zero(A);
        generation_matrice_zero(B);
    }
		struct Matrice *tmpB = (struct Matrice *) malloc(sizeof(struct Matrice));
		allocation_matrice(tmpB, B->colonne, B->ligne); // on met dans cette matrice les colonnes de B avec lignes et colonnes inversées

    MPI_Scatterv(A->matrice,	 dataCount, adresseData, MPI_LONG_LONG, A->matrice, dataCount[rank], MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Scatterv(extracte->matrice, dataCount, adresseData, MPI_LONG_LONG, tmpB->matrice, dataCount[rank], MPI_LONG_LONG, 0, MPI_COMM_WORLD);
		echange_ligne_colonne(tmpB, B); // On met la bonne matrice dans B
		sleep(rank);
		// afficher_matrice(A, "A après Scatterv", rank);
		// afficher_matrice(B, "B après Scatterv", rank);

    free(tmpB->matrice);
		if (rank == 0){
			free(extracte->matrice);
		}
    free(extracte);
		allocation_matrice(tmpB, B->ligne, B->colonne); // on va mettre dans cette matrice le résultat de calcul de chaque processus (les colonnes de la matrice C)
		generation_matrice_zero(tmpB);

		for(i = 0; i < A->ligne; i++){
			for (j = 0; j < B->colonne; j++) {
				tmpB->matrice[get_case(tmpB, i, j)] = produit_matriciel_par_case(A, B, i, j);
			}
		}
		sleep(rank);
		afficher_matrice(tmpB, "tmpB après premier calcul", rank);
		free(tmpB);



    free(A->matrice);
    free(A);
    free(B->matrice);
    free(B);
    MPI_Finalize();
}
