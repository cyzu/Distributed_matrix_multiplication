#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]){
	int numprocs, rank;
			
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int message = 1001;
	int *message_receive = malloc(sizeof(int));
	int return_send;
	
	// Premier envoie	
	if(rank == 0) {	
			return_send = MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			if(return_send != MPI_SUCCESS) {
				printf("ECHEC envoi de message\n");
				exit(1);
			}
	}
	else if (rank == numprocs -1){
		//Dernier qui reçoie et affiche
		
			MPI_Recv(message_receive, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Message recu : %d (1000 + %d)\n", *message_receive, numprocs-1);
	}
	else {
		/* Recoivent une valeur, ++, ré-envoie*/
		
		MPI_Recv(message_receive, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		*message_receive = *message_receive + 1;
		
		return_send = MPI_Send(message_receive , 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		if(return_send != MPI_SUCCESS) {
			printf("ECHEC envoi de message\n");
			exit(1);
		}
	}
	
	MPI_Finalize();
}