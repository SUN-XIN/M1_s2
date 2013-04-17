#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAGINIT 0
#define NB_SITE 6

#define TAGINIC 1
#define INITIATEUR 2

MPI_Status status;
int nb_proc,rang;
int flag=0;

int nbVoisins=0;
int minLocal=0;
int mesVoisins[5];
int i;
int recu[NB_SITE];
int pere=0;

void simulateur(void) {
   int i;

   /* nb_voisins[i] est le nombre de voisins du site i */
   int nb_voisins[NB_SITE+1] = {-1, 3, 3, 2, 3, 5, 2};
   int min_local[NB_SITE+1] = {-1, 12, 11, 8, 14, 5, 17};

   /* liste des voisins */
   int voisins[NB_SITE+1][5] = {{-1, -1, -1, -1, -1},
            {2, 5, 3, -1, -1}, {4, 1, 5, -1, -1}, 
            {1, 5, -1, -1, -1}, {6, 2, 5, -1, -1},
            {1, 2, 6, 4, 3}, {4, 5, -1, -1, -1}};
                               
   for(i=1; i<=NB_SITE; i++){
      MPI_Send(&nb_voisins[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(voisins[i],nb_voisins[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
      MPI_Send(&min_local[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD); 
   }
}

/******************************************************************************/
void calcul_min(int s) {
	//recuper les val init
	MPI_Recv(&nbVoisins, 1, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	//printf("site %d : nbrVoisin %d\n",s,nbVoisins);
	MPI_Recv(mesVoisins, nbVoisins, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	MPI_Recv(&minLocal, 1, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	//for (i=0;i<5;i++) printf("site %d : voisins %d\n",s,mesVoisins[i]);
	
	int valRecu=-1;
	if (rang==INITIATEUR) {
		for (i=0;i<nbVoisins;i++) {
			MPI_Send(&minLocal, 1, MPI_INT, mesVoisins[i], TAGINIC, MPI_COMM_WORLD);
			printf("INITIATEUR envoie %d vers site%d\n",minLocal,mesVoisins[i]);  
		}
		for (i=0;i<nbVoisins;i++) {
			MPI_Recv(&valRecu, 1, MPI_INT, MPI_ANY_SOURCE,TAGINIC,MPI_COMM_WORLD,&status);
			recu[status.MPI_SOURCE]=1;
			//Calculer
			if (valRecu<minLocal) {
				minLocal=valRecu;
				printf("INITIATEUR change min local à %d\n",minLocal);
			}
			printf("INITIATEUR a recu %d depuis site%d\n",valRecu,status.MPI_SOURCE);  
		}
		printf("Decision - FIN DE PROG\n");
	}
	else {
		int fin;
		do {
			MPI_Recv(&valRecu, 1, MPI_INT, MPI_ANY_SOURCE,TAGINIC,MPI_COMM_WORLD,&status);
			recu[status.MPI_SOURCE]=1;
			printf("site%d a recu %d depuis site %d\n",rang,valRecu,status.MPI_SOURCE);
			//Calculer
			if (valRecu<minLocal) {
				minLocal=valRecu;
				printf("site %d change min local à %d\n",rang,minLocal);
			}
			if (!pere) { //pere null
				pere=status.MPI_SOURCE;
				printf("site%d a nouveau pere site%d\n",rang,pere);
				for (i=0;i<nbVoisins;i++) {
					if (mesVoisins[i]!=pere) {
						MPI_Send(&minLocal, 1, MPI_INT, mesVoisins[i], TAGINIC, MPI_COMM_WORLD);
						printf("site%d envoie %d vers site%d\n",rang,minLocal,mesVoisins[i]);
					}
				}
			}
			fin=1;
			for (i=0;i<nbVoisins;i++) {
				if (!recu[mesVoisins[i]]) {
					fin=0;
					break;
				}
			}
		}while(!fin);
		printf("site%d a recu msg de tous voisin, envoyer %d vers pere site%d\n",rang,minLocal,pere);
		MPI_Send(&minLocal, 1, MPI_INT, pere, TAGINIC, MPI_COMM_WORLD);
	}
}

int main (int argc, char* argv[]) {

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);

   if (nb_proc != NB_SITE+1) {
      printf("Nombre de processus incorrect !\n");
      MPI_Finalize();
      exit(2);
   }
  
   MPI_Comm_rank(MPI_COMM_WORLD, &rang);
  
   if (rang == 0) {
      simulateur();
   } else {
      calcul_min(rang);
   }
  
   MPI_Finalize();
   return 0;
}
