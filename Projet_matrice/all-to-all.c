#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]){
	int numprocs, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int i, return_send;
	int *tablo = malloc(sizeof(int) * numprocs);
	
	/* initialisation du tablo */
	for (i = 0; i < numprocs; i++){
		if (i == rank){
			tablo[i] = rank;
		}
		else{
			tablo[i] = 0;
		}
	}
	/* Envoie la valeur de la case rank aux autres */
	for (i = 0; i < numprocs; i++){
		return_send = MPI_Bcast(&tablo[i], 1, MPI_INT, i, MPI_COMM_WORLD);
		if (return_send != MPI_SUCCESS){
			printf("Echec d'envoi en broadcast\n");
			exit(1);
		}
	}
	/* affiche le résultats pour un processeur */
	if (rank == numprocs - 3){
		for (i = 0; i < numprocs; i++){
			printf("tablo[%d] = %d\n", i, tablo[i]);
		}
	}
	MPI_Finalize();
}
