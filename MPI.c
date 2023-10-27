#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[]){
    int nproc, rank;

    int src =0, dst = 1, data_send = 9876, data_rcv =-1;
    MPI_Status status;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    
    if(rank == 0){
        MPI_Send(&data_send, 1, MPI_INT, dst, 123, MPI_COMM_WORLD);
        printf("Process id %d sending message %d to %d\n", rank, data_send, dst);
    } else{
        MPI_Recv(&data_rcv, 1, MPI_INT, src, 123, MPI_COMM_WORLD, &status);
        printf("Process id %d received message %d from %d\n", rank, data_rcv, src);
    }

    MPI_Finalize();
    return 0;
}
