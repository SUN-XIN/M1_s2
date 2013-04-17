#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

#define NB_P 5
#define NB_MEALS 3
#define PRED (my_rank + NB_P -1)%NB_P
#define NEXT (my_rank + 1)%NB_P

int my_rank;
int nb_proc;
int tag =0;
MPI_Status status;

int nbBag=1;
int faim=1;
int li[NB_P];

void envoyerMSG(int dest,int msg){
	MPI_Send(&msg, 1, MPI_CHAR,dest,tag,MPI_COMM_WORLD);
}

int recuMSG(int source){
	int msg;
	MPI_Recv(&msg, 1, MPI_CHAR,source,tag,MPI_COMM_WORLD,&status);
	return msg;
}

int testRecu() {
	MPI_Iprobe( NEXT, tag, MPI_COMM_WORLD, &flag, &status);
	if (flag) { // il y a msg dans le buff recept
		recuMSG((my_rank + 1)%NB_P);
		return 1;
	}
	else return 0;
}

int testDemande() {
	MPI_Iprobe(PRED, tag, MPI_COMM_WORLD, &flag, &status);
	if (flag) { // il y a msg dans le buff recept
		return recuMSG(PRED);
	}
	else return 0;
}

void ajouter(int d) {
	int i=0;
	int tmp,t;
	if (!li[0]) {
		li[0]=d;
		return;
	}
	while (li[i]<d) {i++;}
	tmp=li[i];
	li[i]=d;
	while (li[i]){
		t=li[i+1];
		li[i+1]=tmp;
		tmp=t;
		i++;
	}
}

void supprimer() {
	int i=0;
	while (li[i+1]) {
		li[i]=li[i+1];
		i++;
	}
	li[i]=0;
}

int main(int argc, char **argv){

  int flag;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  
	int msg; /* id(>0) - num P qui demande baguette
				0 - baguette */
	int etat=0;/*  	0 - THINGKING
					1 - HUNGTY
					2 - EATING */
	int meal=0;
	int d=0;
	
	while (1) {
		if (faim) {
			srand(my_rank);
			etat = rand() % 2;
		}
		if (etat==0) { // Etat => THINGKING
			sleep(1);
			if (testRecu()) nbB++;
			if (d=testDemande()) {
				ajouter(d);
				if (nbB) {
					suprrimer();
					envoyerMSG(PRED,0);
				}
			}
		}
		else { // Etat => HUNGTY
			while (nbB<2) {
				envoyerMSG(NEXT,my_rank+1);
				sllep(1);
				if (testRecu()) nbB++;
				if (d=testDemande()) {
					if ((nbB) && (li[0]< my_rank)) {
						supprimer();
						envoyerMSG(PRED,0);
					}
				}
			}
			etat=3; // Etat => EATING
			sleep(1);
		}
	}
	
}
