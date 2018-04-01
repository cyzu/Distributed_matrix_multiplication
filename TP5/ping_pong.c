#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]){
	int numprocs, rank;
			
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	int message = 0, i;
	int *message_receive = malloc(sizeof(int));
	MPI_Status status;
	
	if(rank == 0) {	
		for(i = 1; i < numprocs; i++) {
			int return_send = MPI_Send(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			if(return_send != MPI_SUCCESS) {
				printf("ECHEC envoi de message\n");
				exit(1);
			}
			/*else {
				printf("SUCCÈS envoi de message\n");
			}*/
			message = message + 1000;
		}
		
		for(i = 1; i < numprocs; i++) {
			MPI_Recv(message_receive, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			printf("Pong, depuis %d : %d\n", status.MPI_SOURCE, *message_receive);
		}
	}
	else {
			MPI_Recv(message_receive, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("%d Message recu : %d puis renvoie %d\n", rank, *message_receive, *message_receive + 1);
			
			*message_receive = *message_receive + 1;
			int return_send = MPI_Send(message_receive, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			if(return_send != MPI_SUCCESS) {
				printf("ECHEC envoi de message\n");
				exit(1);
			}
	}
	
	
	MPI_Finalize();
}
/* Executer avec 2 processus */