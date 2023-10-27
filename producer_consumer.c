#include<stdio.h>
#include<stdlib.h>
#include "mpi.h"
#include<time.h>
#include<string.h>

#define MAXPROC 100

int main(int argc, char **argv){
    int a;
    clock_t start_time, end_time;
    double time_taken;
    int time_rec;

    time_rec = atoi(argv[1]);
    const int tag =0;
    char myname[MPI_MAX_PROCESSOR_NAME];
    int namelen;
    char hostname[MAXPROC][MPI_MAX_PROCESSOR_NAME];  
    int no_work = -1;
    char work[10] = "Work";
    char ack[10] = "ACK";
    char req[50] = "Req";
    char msg_consumer[50] = "c";
    int work_consumer[50];
    char abrt[10] = "ABORT";
    char nabrt[10] = "-1";
    char *response_type;

    int work_produced=-1;
    int work_produced_broker=-1;
    int work_produced_consumer=-1;
    int work_consumed=-1;
    int work_rec[20];
    work_rec[0]='\0';
    int rank, nproc, i, index;
    int work_processed;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    MPI_Request recv_req[MAXPROC];  
    MPI_Status status;
    MPI_Request request = MPI_REQUEST_NULL;
    int token = 19;

    if (rank != 0) {
        if((rank%2) == 1){
            work_produced=7;
            for(int j =5; j<100; j++){
                work_produced = ((work_produced+j)/3 ) + 4*rank;
            }
            MPI_Isend(&work_produced, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &request);
            printf("Producer %d sent message %d to Broker\n", rank, work_produced);
            MPI_Recv(&ack, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            MPI_Waitany(1, &request, &index, &status);

            printf("Producer %d received message %s from Broker\n", rank, ack);
        } else {
            int consume_count = 0;

            MPI_Isend(&req, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &request);
            printf("Comsumer %d sent message %s to Broker\n", rank, req);

            MPI_Recv(&work_produced_consumer, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(work_produced_consumer != -1){
                for(int c_i=0;c_i<1000;c_i++)
                work_consumed = ((work_consumed*work_produced_consumer)+rank)/c_i;
            }
            MPI_Waitany(1, &request, &index, &status);

            printf("Consumer %d received message %d from Broker\n", rank, work_produced_consumer);           
        }
        
    } else {
    token = (token/6 + 7 )/3;
  }

  if (rank == 0) {
    start_time = clock();
    for(i=1;i<nproc;i++){
        if(i%2==1){
            int flag =-1, count=0;
            MPI_Recv(&work_produced_broker, 1, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            end_time = clock();
            time_taken = ((double)(end_time-start_time))/CLOCKS_PER_SEC;
            if(time_taken >=time_rec){
                response_type = abrt;
            } else response_type = nabrt;
            
            int w_size = sizeof(work_rec);
            int w_i=0;
            while(w_i != w_size){
                if(work_rec[w_i] != '\0')
                    w_i++;
                else {
                    flag++;
                    work_rec[w_i] = work_produced_broker;
                    printf("Broker recived message %d from Producer %d\n", work_rec[w_i], i);
                    MPI_Send(&ack, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
                    printf("Broker sent message %s to Producer %d\n", ack, i);
                    break;
                }
                /*
                if(flag<0){
                    if(response_type == abrt)
                        printf("Broker sent message %s to Producer %d\n", abrt, i);
                    else {
                        int q[10];
                        int q_p[10];
                        q[count] = work_produced;
                        q_p[count] = i;
                        count = count++;
                    }
                }
                */
                
            }
        }
        else{
            MPI_Recv(&req, 1, MPI_INT, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            end_time = clock();
            time_taken = ((double)(end_time-start_time))/CLOCKS_PER_SEC;
            
            if(time_taken>=time_rec){
                response_type = abrt;
            }else{
                response_type = nabrt;
                if (response_type == abrt)
                    MPI_Isend(&abrt, 1, MPI_CHAR, i, 4, MPI_COMM_WORLD, &request);//ABORTMESSAGE
                else if(work_rec[0] != '\0'){
                    msg_consumer[i] = work_produced_broker+'0';
                    printf("Broker recived message %s from Consumer %d\n", req, i);
                    //MPI_Send(&msg_consumer, 1, MPI_CHAR, i, 4, MPI_COMM_WORLD); 
                    //printf("1st Element in Buffer %d\n", work_rec[0]);
                        
                       
                    MPI_Send(&work_rec[0], 1, MPI_INT, i, 5, MPI_COMM_WORLD);    
                    printf("Broker sent Work %d to Consumer %d\n", work_rec[0], i);
                } else MPI_Send(&no_work, 1, MPI_INT, i, 5, MPI_COMM_WORLD);
                    
                
                
            }
            
        }
    }
    
  }

    MPI_Finalize(); 
}
