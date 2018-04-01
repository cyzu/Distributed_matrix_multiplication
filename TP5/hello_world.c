#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]){
	int numprocs, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0){
		printf("%d : hello\n", rank);
	}
	else if(rank == 1){
		printf("%d : goodbye\n", rank);
	}
	
	//printf("rank : %d\n", rank);

	MPI_Finalize();
	//return 0;
}
