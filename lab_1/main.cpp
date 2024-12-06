#include <iostream>
#include <mpi.h>

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int sender = 0;
    const int message = 10;
        if (rank == sender) {
          for (int i = 1; i < size; i++) {
              MPI_Send(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
          }
        } else {
            int received_message;
            MPI_Recv(&received_message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process " << sender << " sent message " << message << " to process " << rank << std::endl;
            std::cout << "Process " << rank << " received message " << received_message << " from process 0" << std::endl;
        }
    MPI_Finalize();
    return 0;
}
