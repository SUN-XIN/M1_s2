#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

#define NB_P 5
#define NB_MEALS 3

void envoyerMSG(int dest,int msg){
	int tag =0;
	MPI_Send(&msg, 1, MPI_CHAR,dest,tag,MPI_COMM_WORLD);
}

int recuMSG(int source){
	int msg;
	int tag =0;
	MPI_Status status;
	MPI_Recv(&msg, 1, MPI_CHAR,source,tag,MPI_COMM_WORLD,&status);
	return msg;
}

int testDemander(int my_rank,int* nbBag,int* nbD) {
	printf("\n    verrifier la demande : ");
	int tag=0;
	int flag;
	MPI_Status status;
	MPI_Iprobe((my_rank + NB_P -1)%NB_P, tag, MPI_COMM_WORLD, &flag, &status);
	if (flag) { // il y a msg dans le buff recept
		nbD++;
		printf("mon voisin demande, ");
		if (nbBag) {
			printf("je le donne\n");
			nbBag--;
			nbD--;
			envoyerMSG((my_rank + NB_P -1)%NB_P,1);
		}
		else { 
			printf("je n'en pas, je demande à mon voisin\n");
			envoyerMSG((my_rank +1)%NB_P,0);
		}
		return 1;}
	else printf("il n'y en pas.\n");
	return 0;
}

int testRecu(int my_rank,int* nbBag,int* nbD) {
	printf("\n    verrifier la recu : ");
	int tag=0;
	int flag;
	MPI_Status status;
	MPI_Iprobe((my_rank + 1)%NB_P, tag, MPI_COMM_WORLD, &flag, &status);
	if (flag) { // il y a msg dans le buff recept
		(*nbBag)++;
	}
	else printf("pas de recu\n");
	while ((*nbD) && (*nbBag)) {
		(*nbD)--;
		(*nbBag)--;
		printf("P %d : je le donne\n",my_rank);
		envoyerMSG((my_rank + NB_P -1)%NB_P,1);
	}
	return flag;
}

int main(int argc, char **argv){
  int my_rank;
  int nb_proc;
  int tag=0;;
  int flag;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

	int msg; /* 0 - demande baguette
				1 - baguette */
	int etat=0;/*  	0 - THINGKING
					1 - HUNGTY
					2 - EATING */
	int meal=0;
	int nbBag=1;
	int faim=1;
	int nbD=0;
	while (1) {
	
	if (faim) {
		srand(my_rank);
		etat = rand() % 2;
	}else {etat=0;}
	if (etat==0) { // Etat => THINGKING
		printf("P %d (T), ",my_rank);
		testRecu(my_rank,&nbBag,&nbD);
		sleep(1);
		printf("P %d termine T, ",my_rank);
		testDemander(my_rank,&nbBag,&nbD);
	}
	else if (etat==1) { // Etat => HUNGTY
			printf("P %d (H), ",my_rank);
			sleep(1);
			if (nbBag<2) { //pas assez paquette
				printf("j'ai %d baguett, ",nbBag);
				while (nbBag<2) {
					if (!testDemander) {//attendre la baguette
						printf("j'envoie msg vers mon voisin %d, ",(my_rank+1)%NB_P);
						envoyerMSG((my_rank+1)%NB_P,0);
						printf("je suis en attente.\n");
						do {
							sleep(1);
							testDemander(my_rank,&nbBag,&nbD);
							MPI_Iprobe((my_rank + 1)%NB_P, tag, MPI_COMM_WORLD, &flag, &status);
						}while (!flag);
						msg=recuMSG((my_rank+1)%NB_P);
						nbBag++;
						printf("P %d a recu 1 baguettes, j'en ai %d\n",my_rank,nbBag);
					}
				}
			}
				printf("P %d a 2 baguettes, il commence manger\n",my_rank);
				etat=3;
				sleep(1);
				meal++;
				printf("P %d termine %d EATING\n",meal);
				if (meal==NB_MEALS) {
					faim=0;
					printf("P %d termine\n");
				}}}
	MPI_Finalize();
}
