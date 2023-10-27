#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>

#define MAXPROC 100

int main(int argc, char* argv[]) {
    int TAG_MSG = 1;
    int TAG_ACK = 2;
    int TAG_REQ = 3;
    int TAG_ABORT = 0;
    int TAG_NO_WORK = 15;
    int TAG_END = 6;
    int broker = 0;
    

    int nproc, rank;
    MPI_Request request;
    MPI_Status status;

    int dest = nproc-1;
    int buffer = -1;
    int buffer_consumer = -1;
    int buffer_producer = -1;
    int consumed_total = 0;
    int received_work=0;
    int rec_ack = 0;
    int consume_count=0;
    MPI_Status work_status;
    MPI_Status rcv_status;
    MPI_Request work_request;
    MPI_Request irecv_request;
    MPI_Request w_request;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int time_allocated = atoi(argv[1]);
    double start_time = MPI_Wtime();
    dest = nproc -1;
        while(1){
            for(int i=0;i<10000000;i++)
                buffer_producer = buffer_producer*i;
                
            if(rank != dest){
                MPI_Isend(&buffer_producer, 1, MPI_INT, dest, TAG_MSG, MPI_COMM_WORLD, &request);
                dest++;
                if(dest>=nproc && dest >=0)
                    dest = 0;
             }else {
                dest = dest+1;
                if(dest >= nproc && dest >=0)
                    dest = 1;
                MPI_Isend(&buffer_producer, 1, MPI_INT, dest, TAG_MSG, MPI_COMM_WORLD, &request);
            }

            MPI_Irecv(&buffer_producer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &irecv_request);
            while(!rec_ack){
                MPI_Irecv(&buffer_producer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &work_request);
                MPI_Test(&work_request, &received_work, &work_status);
                if(received_work){
                    for(int j=0;j<10000000;j++)
                        received_work = received_work*j;
                    MPI_Isend(&buffer_producer, 1, MPI_INT, work_status.MPI_SOURCE, TAG_ACK, MPI_COMM_WORLD, &w_request);
                    consume_count++;    
                } else MPI_Cancel(&work_request);
                
                MPI_Test(&irecv_request, &rec_ack, &rcv_status);
                if(received_work)
                    MPI_Wait(&w_request, &status);
                if((MPI_Wtime()-start_time)>time_allocated){
                    break;
                }            
            }
            rec_ack = 0;
            if((MPI_Wtime()-start_time)>time_allocated)
                break;
        }
        MPI_Reduce(&consume_count, &consumed_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        printf("Total number of messages consumed: %d\n", consumed_total);
        MPI_Finalize();

}
