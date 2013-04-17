#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define SIZE 128

int main(int argc, char **argv){
  int my_rank;
  int nb_proc;
  int source;
  int dest;
  int tag =0;
  char message[SIZE];
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  
  if (my_rank < nb_proc-1) {
	sprintf(message, "c'est msg du proc %d envoie vers succ ", my_rank);
	dest = my_rank+1;
	MPI_Send(message, strlen(message)+1, MPI_CHAR,dest,tag,MPI_COMM_WORLD);
	printf("PROC %d termine\n",my_rank);
	
	if(my_rank !=0){
		source=my_rank-1;
		MPI_Recv(message, SIZE, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
		printf("PROC %d => msg recu est : %s\n",my_rank, message);
	}
  }
  
  
  MPI_Finalize();
  return 0;
}
