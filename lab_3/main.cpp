#include <mpi.h>
#include <stdio.h>
#include <algorithm>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int N = 4*size;
    int data[N];
    if (rank == 0) {
        std::cout << "Process " << rank << " scatter data: ";
        for (int i = 0; i < N; i++) {
            data[i] = i + 1;
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    int recvData[N/size];
    MPI_Scatter(data, N / size, MPI_INT, recvData, N / size, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (int i = 0; i < 4; i++) {
        recvData[i] *= rank;
        sum += recvData[i];
    }

    int gatherData[N];
    MPI_Gather(recvData, 4, MPI_INT, gatherData, 4, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Process " << rank << " gathered data: ";
        for (int i = 0; i < N; i++) {
          std::cout << gatherData[i] << " ";
        }
        std::cout << std::endl;
    }
    int SUM;
    MPI_Allreduce(&sum, &SUM, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    std::cout << "Process " << rank << " local sum : " << sum << std::endl;
    std::cout << "Process " << rank << " got total sum of all multiplied elements: " << SUM << std::endl;

    MPI_Finalize();
    return 0;
}
