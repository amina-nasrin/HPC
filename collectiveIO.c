/* noncontiguous access with a single collective I/O function */
#include "mpi.h"
#include <stdlib.h>
#include <string.h>
#include<stdio.h>


//#define FILESIZE      1048576
#define FILESIZE 128
#define INTS_PER_BLK  1

int main(int argc, char **argv)
{
  int *buf, *bufr, rank, nprocs, nints, bufsize, input_letter;
  MPI_File fh;
  MPI_Datatype filetype;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  bufsize = FILESIZE/nprocs;
  buf = (int *) malloc(bufsize);
  nints = bufsize/sizeof(int);

    input_letter = 'A'+rank;
    memset(buf, input_letter, nints * sizeof(int));
    MPI_Type_vector(nints/INTS_PER_BLK, INTS_PER_BLK, INTS_PER_BLK*nprocs, MPI_INT, &filetype);
    MPI_Type_commit(&filetype);
  
  MPI_File_open(MPI_COMM_WORLD, "myfile.txt", MPI_MODE_CREATE| MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
  MPI_File_set_view(fh, rank*nints*sizeof(int), MPI_INT, filetype, "native", MPI_INFO_NULL);
  MPI_File_write_all(fh, buf, 2, MPI_INT, MPI_STATUS_IGNORE); 

  MPI_File_read_all(fh, buf, nints, MPI_INT, MPI_STATUS_IGNORE);
  printf("%s\n", buf);
  MPI_File_close(&fh);


  MPI_Type_free(&filetype);
  free(buf);
  
  //Finalize
  //
  MPI_Finalize();
//printf("%s\n", buf);
  
  return 0; 
}
