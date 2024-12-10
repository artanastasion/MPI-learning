#include <mpi.h>
#include <stdio.h>
#include <algorithm>

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double startwtime = 0.0;
    double endwtime;
    int N = 4*size;
    int data[N];

    //synchronous
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = i + 1;
        }
        startwtime = MPI_Wtime();
    }

    int recvData[N/size];
    MPI_Scatter(data, N / size, MPI_INT, recvData, N / size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < N/size; i++) {
        recvData[i] *= rank;
    }

    int gatherData[N];
    MPI_Gather(recvData, N/size, MPI_INT, gatherData, N/size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        endwtime = MPI_Wtime();
        std::cout << "Synchronous data exchange took:" << endwtime - startwtime << " sec."<< std::endl;
        std::cout << "Resulting array of data (synchronous): ";
        for (int i = 0; i < N; i++) {
            std::cout << gatherData[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    //asynchronous
    if (rank == 0) {
      startwtime = MPI_Wtime();
    }

    int IrecvData[N/size];
    MPI_Request request;

    MPI_Iscatter(gatherData, N/size, MPI_INT, IrecvData, N/size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);

    for (int i = 0; i < N/size; i++) {
        IrecvData[i] *= rank;
    }

    int IGatherData[N];
    MPI_Igather(IrecvData, N/size, MPI_INT, IGatherData, N/size, MPI_INT, 0, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    if (rank == 0) {
      endwtime = MPI_Wtime();
      std::cout << "Asynchronous data exchange took:" << endwtime - startwtime << " sec."<< std::endl;
      std::cout << "Resulting array of data (asynchronous): ";
      for (int i = 0; i < N; i++) {
         std::cout << IGatherData[i] << " ";
      }
      std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}