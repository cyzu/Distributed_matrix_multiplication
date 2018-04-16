//
//  brouillon.c
//  
//
//  Created by Chloé 雪乃 BENSOUSSAN 下村 on 16/04/2018.
//
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main (int argc, char *argv[]){
    int numprocs, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int * tab = malloc (sizeof(int) * 10);
    
    int i;
    for (i = 0; i < 10; i++) {
        tab[i] = i;
    }
    
    int * recv = malloc(sizeof(int) * 5);
    int dataCount[] = {5, 5};
    int adresses[] = {0, 5};
    
    MPI_Scatterv(tab, dataCount, adresses, MPI_INT, recv, dataCount[rank], MPI_INT, 0, MPI_COMM_WORLD);
    
    for (i = 0; i < 10; i++) {
        if (i < 5 && rank == 0) printf("%d - recv[%d] = %d\n", rank, i, recv[i]);
        else if (i > 4 && rank == 1) printf("%d - recv[%d] = %d\n", rank, i - 5, recv[i - 5]);
        usleep(100000);
    }
    
    MPI_Finalize();
}
