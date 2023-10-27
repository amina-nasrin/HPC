#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define MAXPROC 100

int main(int argc, char* argv[]) {
  int i, nproc, rank, index;
  const int tag  = 42;    
  const int root = 0;     

  MPI_Status status;              
  MPI_Request recv_req[MAXPROC];  
  
  char hostname[MAXPROC][MPI_MAX_PROCESSOR_NAME];  
  char myname[MPI_MAX_PROCESSOR_NAME]; 
  int namelen; 

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  MPI_Get_processor_name(myname, &namelen);  
  if (rank == 0) {    
    MPI_Bcast(&rank, 1, MPI_INT, root, MPI_COMM_WORLD);
    for(i=1;i<nproc;i++){
      MPI_Irecv(hostname[i], namelen, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &recv_req[i]);
      }
      printf("I am a very busy professor.\n");	
    
      for(i=1; i<nproc; i++){
        MPI_Waitany(nproc-1, &recv_req[1], &index, &status);
        printf("Received a message from process %d on %s\n", status.MPI_SOURCE, hostname[index+1]);
      }
  } else { 
      MPI_Bcast(&rank, 1, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Send(&myname, namelen, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }

  MPI_Finalize();  
  exit(0);
}
