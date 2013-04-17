#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_CS 3
#define NBR_PROC 3

#define REQ 1
#define ACK 2
#define FIN 3
#define NotRequesting 0
#define Requesting 1
#define SC 2

typedef struct massage {
	int type; /* 1 - REQ
				 2 - ACK */
	int h;
	int j;
}msg;

  int my_rank;
  int nb_proc;
  int tag=0;;
  int flag;
  MPI_Status status;
  
  int nbrAck=0;
  int etat;/* 	0 - not-requesting
				1 - requesting
				2 - SC */
  int h=0;
  int date_req;
  int file[NBR_PROC-1];
  msg mE,mR;
  int nbrAttendre=0;
  int nbrEnd=0;
  
void attendreMSG(){
	MPI_Recv(&mR, sizeof(mR), MPI_CHAR, MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);
	//printf("proc %d req_date %d, msg : h %d  type %d site %d\n",my_rank,date_req,mR.h,mR.type,mR.j);
	if (h<mR.h) h=mR.h;
	h++;
	if (mR.type==REQ) {
		printf("Proc %d a recu de REQ de proc %d à instant %d,",my_rank,mR.j,h);
		if ((etat==NotRequesting) || ((etat==Requesting && (mR.h<date_req || (mR.h==date_req && mR.j<my_rank))))) {
			h++;
			mE.h=h;
			mE.j=my_rank;
			mE.type=ACK;
			MPI_Send(&mE, sizeof(mE), MPI_CHAR,mR.j,tag,MPI_COMM_WORLD);
			printf(" J'envoie de ACK\n");
		}
		else {
			file[nbrAttendre]=mR.j;
			nbrAttendre++;
			printf(" Je l'ajoute dans file\n");
		}
	}
	else {
		if (mR.type==ACK) {
			nbrAck++;
			printf("Proc %d a recu de ACK de proc %d à instant %d => %d en total\n",my_rank,mR.j,h,nbrAck);
		}
		else { // type==FIN
			nbrEnd++;
		}
	}
}

void entreeSC(){
	h++;
	date_req=h;
	nbrAck=0;
	etat=Requesting;
	int i;
	for (i=0;i<NBR_PROC;i++) {
		if (i==my_rank) continue;
		mE.h=h;
		mE.j=my_rank;
		mE.type=REQ;
		MPI_Send(&mE, sizeof(mE), MPI_CHAR,i,tag,MPI_COMM_WORLD);
	}
	while (nbrAck<NBR_PROC-1) {
		attendreMSG();
	}
	etat=SC;
}

void sortirSC() {
	//printf("proc %d,NA %d\n",my_rank,nbrAttendre);
	h++;
	etat=NotRequesting;
	int i;
	for (i=0;i<nbrAttendre;i++) {
		mE.h=h;
		mE.j=my_rank;
		mE.type=ACK;
		MPI_Send(&mE, sizeof(mE), MPI_CHAR,file[i],tag,MPI_COMM_WORLD);
		file[i]=0;
	}
	nbrAttendre=0;
}
	
int main(int argc, char **argv){
	
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
 
  int i;
  for (i=0;i<MAX_CS;i++) {
	  entreeSC();
	  h++;
	  printf("Proc %d est en %dem SC\n",my_rank,i+1);
	  sortirSC();
  }
  printf("******************\nPROC %d termine.\n******************\n",my_rank);
  for (i=0;i<NBR_PROC;i++) {
	  if (i!=my_rank) {
		  mE.type=FIN;
		  mE.h=h;
		  mE.j=my_rank;
		  MPI_Send(&mE, sizeof(mE), MPI_CHAR,i,tag,MPI_COMM_WORLD);
	  }
  }
  while (nbrEnd<NBR_PROC-1) {
	  MPI_Recv(&mR, sizeof(mR), MPI_CHAR, MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);
	  if (mR.type==FIN) {
		  nbrEnd++;
		  continue;
	  }
	  h++;
	  mE.h=h;
	  mE.type=ACK;
	  mE.j=my_rank;
	  MPI_Send(&mE, sizeof(mE), MPI_CHAR,mR.j,tag,MPI_COMM_WORLD);
  }
  MPI_Finalize();
}
