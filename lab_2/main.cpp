#include <mpi.h>
#include <stdio.h>
#include <algorithm>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int N = 10;
    int data[N];

    if (rank == 0) {
      for (int i = 0; i < N; i++) {
        data[i] = i;
      }
      std::cout << "Process " << rank << " broadcasting data..." << std::endl;
    }

    MPI_Bcast(data, N, MPI_INT, 0, MPI_COMM_WORLD);


    for (int i = 0; i < N; i++) {
      data[i] *= rank;
    }

    int sum = 0;
    for (int j = 0; j < N; j++) {
      sum += data[j];
    }

    int maxElement = data[0];
    for (int k = 0; k < N; k++) {
      if (data[k] > maxElement) {
        maxElement = data[k];
      }
    }

    int SUM;
    int MAX;
    MPI_Reduce(&sum, &SUM, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&maxElement, &MAX, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
      std::cout << "Total sum of all elements:  " << SUM << std::endl;
      std::cout << "Maximum element::  " << MAX << std::endl;
    }
    MPI_Finalize();
    return 0;
}
