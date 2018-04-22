#include <mpi.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

struct Matrice{
	int ligne, colonne;
	long long int *matrice;
};

/**/
/**/
/**/
/*			==  ==	======	==	==		=====			*/
/*			==  ==   	== 	 	== 	==		=					*/
/*			==  ==   	== 	 	== 	==		===				*/
/*			 ====	  	==	 	== 	=====	=====			*/
/**/
/**/
/**/

void allocation_matrice(struct Matrice * m, const int l, const int c){
    m->ligne = l;
    m->colonne = c;
		m->matrice = malloc(sizeof(long long int*) * m->ligne * m->colonne);
}

void afficher_matrice(const struct Matrice *m){
    int i;
    for (i = 0; i < m->ligne * m->colonne; i++){
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

int get_case(const struct Matrice * m, int i, int j){
	 return i * m->colonne + j;
}

/* Fonction qui échange les lignes et les colonnes de la matrice m dans destination */
void echange_ligne_colonne(const struct Matrice * m, struct Matrice * destination){
    int i, j;
		#pragma omp parallel for
    for (i = 0; i < destination->ligne; i++) {
				#pragma omp parallel for
        for(j = 0; j < destination->colonne; j++){
            destination->matrice[get_case(destination, i, j)] = m->matrice[get_case(m, j, i)];
        }
    }
}

/* Fonction qui copie la matrice m dans destination (allocation doit être fait avant) */
void copie_matrice(const struct Matrice * m, struct Matrice * destination){
    int i;
		#pragma omp parallel for
    for (i = 0; i < m->ligne * m->colonne; i++){
        destination->matrice[i] = m->matrice[i];
    }
}

void scatterv(int * lignes, int * dataCount, int * adresseData, const int size, const int nb_procs){
		int i, partage = size;

		/* Calcul de nombre de ligne à envoyer par colonne (pour tous les cas)*/
		#pragma omp parallel for
		for (i = 0; i < nb_procs; i++) {
				lignes[i] = partage / nb_procs;
		}
		partage = partage % nb_procs;
		#pragma omp parallel for
		for (i = 0; i < partage; i++) {
				lignes[i]++;
		}

		/* Calcul de nombre de cases à envoyer et leur adresse pour MPI_Scatterv */
		int accumulateur = 0;
		#pragma omp parallel for
		for(i = 0; i < nb_procs; i++){
				adresseData[i] = accumulateur;
				dataCount[i] = lignes[i] * size;
				accumulateur += lignes[i] * size;
		}
}

void count_lignes_prec(int * lignes, int * lignes_prec, const int nb_procs){
		int i;
		#pragma omp parallel for
		for (i = 0; i < nb_procs; i++){
				lignes_prec[i] = 0;
		}
		#pragma omp parallel for
		for (i = 1; i < nb_procs; i++){
				lignes_prec[i] = lignes_prec[i - 1] + lignes[i - 1];
		}
}

int get_matrice_size(char * filename){
		FILE *file = fopen(filename, "r");
		int size = 0;
		long long int valeur = 0;

		if (file == NULL) {
				printf("Erreur : l'ouverture du fichier '%s' a échoué !\n", filename);
				exit(1);
		}
		/* Compteur du nombre d'entier pour connaitre la taille de la matrice à allouer */
		while(valeur != '\n'){
			if (valeur == ' '){
				size++;
			}
			valeur = fgetc(file);
		}
		size++; // compte la dernière valeurs
		fclose(file);
		return size;
}


/**/
/**/
/**/
/*    ====== 	======	 ======		======	==	 ==	==	========		*/
/*		==	===	==	===	==		==	==	 ==	==	 ==	==		 ==				*/
/*		======	======	==		==	==	 ==	==	 ==	==		 ==				*/
/*		==			==	 ==	 ======		======	 =====	==		 ==				*/
/**/
/**/
/**/

/* Renvoie le résultat du produit matriciel de la case (ligne, colonne)*/
long long int produit_matriciel_par_case(const struct Matrice * a, const struct Matrice * b, const int ligne, const int colonne){
    int k;
    long long int c = 0;
    for(k = 0; k < a->colonne; k++){
        c = c + a->matrice[get_case(a, ligne, k)] * b->matrice[get_case(b, k, colonne)];
    }
    return c;
}


/**/
/**/
/**/
/*     =====	===== == 		==	=====	=====			 ===		======	=====	==	==	=====		*/
/*		==			==		====	==	==		==	==	 	==	==	 	==		==		==	==	==  ==	*/
/*		==  ===	=== 	==  ====	===		=====		 ==	== == 	==		===		==	==	=====		*/
/*		 =====	=====	==	  ==	=====	==	==	==			==	==		=====	 ====	 	==  ==	*/
/**/
/**/
/**/

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

void generation_matrice_from_file(struct Matrice * m, const char * filename, const int size){
    FILE *file = fopen(filename, "r");
    int i;
    long long int valeur;

    if (file == NULL) {
        printf("Erreur : l'ouverture du fichier '%s' a échoué !\n", filename);
        exit(1);
    }
    /* Remettre le curseur au début du fichier */
    rewind(file);
    allocation_matrice(m, size, size);

    /* Insersion des valeurs */
    for(i = 0; i < m->ligne * m->colonne; i++){
        fscanf(file, "%lld", &valeur);
        m->matrice[i] = valeur;
    }
    fclose(file);
}

/**/
/**/
/**/
/*				=      =		 ===			==	==		==  		*/
/*				===	 ===		==  ==		==	====	==			*/
/*				== ==	==	 ==	== ==		==	==	====			*/
/*				==		==	==		 	==	==	==		==			*/
/**/
/**/
/**/

int main (int argc, char *argv[]){
    int numprocs, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


		/************************************************/
		/*										Usage											*/
		/************************************************/

		if (argc - 1 < 2){
				printf("Usage : mpirun -oversubscribe -np <num> %s <filename1> <filename2>\n ", argv[0]);
				MPI_Finalize();
				exit(0);
		}


		/************************************************/
		/*							Allocation de A et B						*/
		/************************************************/

    int size, i, j, l_actuelle, tour;
    int suivant, precedent;

		/* Matrice C = A * B */
		struct Matrice *A = (struct Matrice *) malloc(sizeof(struct Matrice));
		struct Matrice *B = (struct Matrice *) malloc(sizeof(struct Matrice));
		struct Matrice *extracte = (struct Matrice *) malloc(sizeof(struct Matrice));

		size = get_matrice_size(argv[1]);

		/************************************************/
		/*			Préparation scatter et circulation			*/
		/************************************************/

    int nb_procs = numprocs > size ? size : numprocs;

		/* Redéfinir MPI_COMM_WORLD avec le bon nombre de processus */
		MPI_Comm_size(MPI_COMM_WORLD, &nb_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Répartiton des lignes en fonction de la taille de la matrice et du nombre de processus */
    int dataCount[nb_procs], adresseData[nb_procs], lignes[nb_procs], lignes_prec[nb_procs];
    scatterv(lignes, dataCount, adresseData, size, nb_procs);
		count_lignes_prec(lignes, lignes_prec, nb_procs);

    if (rank == 0){
				generation_matrice_from_file(A, argv[1], size);
				generation_matrice_from_file(B, argv[2], size);

        allocation_matrice(extracte, B->ligne, B->colonne);
        echange_ligne_colonne(B, extracte);

        A->ligne = lignes[0];	// je redimensionne la matrice A pour le scatterv (du root)
        B->colonne = lignes[0];
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


		/************************************************/
    /* 										Scatter										*/
		/************************************************/

		/* Envoie aux processeurs différentes lignes et colonnes des matrices A et B */
    MPI_Scatterv(A->matrice, dataCount, adresseData, MPI_LONG_LONG, A->matrice, dataCount[rank], MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    MPI_Scatterv(extracte->matrice, dataCount, adresseData, MPI_LONG_LONG, tmpB->matrice, dataCount[rank], MPI_LONG_LONG, 0, MPI_COMM_WORLD);
    echange_ligne_colonne(tmpB, B); // On met la bonne matrice dans B

    free(tmpB->matrice);
    if (rank == 0){
        /* elle a été allouée seulement dans le processus 0 */
        free(extracte->matrice);
    }
    free(extracte);
    allocation_matrice(tmpB, B->ligne, B->colonne); // on va mettre dans cette matrice le résultat de calcul de chaque processus (les colonnes de la matrice C)
    generation_matrice_zero(tmpB);

    struct Matrice *tmpCirculation = (struct Matrice *) malloc(sizeof(struct Matrice)); // Matrice duplique de A, pour la circulation des lignes_prec
    allocation_matrice(tmpCirculation, lignes[0], size); //allocation de la taille max possible de la matrice A
    generation_matrice_zero(tmpCirculation);


		/************************************************/
    /* 							Création du datatype 						*/
		/************************************************/

    MPI_Datatype Ligne_TYPE;
    MPI_Type_contiguous(lignes[0] * size, MPI_LONG_LONG, &Ligne_TYPE); //allocation de la taille max possible de la matrice A
    MPI_Type_commit(&Ligne_TYPE);

    suivant = (rank + nb_procs + 1) % nb_procs;
    precedent = (rank + nb_procs - 1) % nb_procs;


		/************************************************/
    /* 						Calculs et circulation						*/
		/************************************************/

		for(tour = 0; tour < nb_procs; tour++){
            l_actuelle = (rank - tour + nb_procs) % nb_procs;
            A->ligne = lignes[l_actuelle]; // on redimensionne la taille de la matrice après réception

            /* Calculs des matrices dans tmpB (à la bonne case) */
						#pragma omp parallel for
            for(i = 0; i < A->ligne; i++){
										#pragma omp parallel for
                    for (j = 0; j < B->colonne; j++) {
                            tmpB->matrice[get_case(tmpB, i + lignes_prec[l_actuelle], j)] = produit_matriciel_par_case(A, B, i, j);
                    }
            }
            if (tour >= nb_procs - 1){
                /* Il n'y a pas de circulation à faire au dernier tour */
                    free(tmpCirculation->matrice);
                    free(tmpCirculation);
            }
            else {
                /* Effectuer la circulation de lignes dans les autres tours */
                    copie_matrice(A, tmpCirculation);
                    if (rank == 0){
                        // le premier envoie puis recois
                            MPI_Send(&tmpCirculation->matrice[0], 1, Ligne_TYPE, suivant, 100+rank, MPI_COMM_WORLD);
                            MPI_Recv(&A->matrice[0], 1, Ligne_TYPE, precedent, 100+precedent, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    }
                    else if (rank <= nb_procs) {
                        // les autres reçoivent puis envoient
                            MPI_Recv(&A->matrice[0], 1, Ligne_TYPE, precedent, 100+precedent, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            MPI_Send(&tmpCirculation->matrice[0], 1, Ligne_TYPE, suivant, 100+rank, MPI_COMM_WORLD);
                    }
            }
    }
    B->ligne = tmpB->colonne; // On met dans cette matrice la matrice final tmpB, pour l'envoyer, puis la ré-inverser
    B->colonne = tmpB->ligne;

    echange_ligne_colonne(tmpB, B);
    free(tmpB->matrice);
    free(tmpB);

    if (rank == 0){
        /* Réajustement de la taille */
        A->ligne = size;
        A->colonne = size;
    }

		/************************************************/
    /* 					Gather et affichage final 					*/
		/************************************************/

    MPI_Gatherv(B->matrice, dataCount[rank], MPI_LONG_LONG, A->matrice, dataCount, adresseData, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    if (rank == 0){
        struct Matrice *C = (struct Matrice *) malloc(sizeof(struct Matrice));
        allocation_matrice(C, A->ligne, B->colonne);

        echange_ligne_colonne(A, C);
        afficher_matrice(C);

				free(C->matrice);
				free(C);
    }

		/************************************************/
		/* 										Free											*/
		/************************************************/
    free(A->matrice);
    free(A);
    free(B->matrice);
    free(B);
    MPI_Type_free(&Ligne_TYPE);
    MPI_Finalize();
}
