#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAGINIT 0
#define NB_SITE 6
#define DIAMETRE 5	/* !!!!! valeur a initialiser !!!!! */
#define INITIATEUR 3

int nb_proc,rang;
MPI_Status status;
int tag=1,flag;

int nbVoisinIn=0;
int nbVoisinOut=0;
int voisinIn[2];
int voisinOut[2];
int minLocal=0;

int cptS=0;
int cptR[2];
int recu;

void simulateur(void) {
   int i;

   /* nb_voisins_in[i] est le nombre de voisins entrants du site i */
   /* nb_voisins_out[i] est le nombre de voisins sortants du site i */
   int nb_voisins_in[NB_SITE+1] = {-1, 2, 1, 1, 2, 1, 1};
   int nb_voisins_out[NB_SITE+1] = {-1, 2, 1, 1, 1, 2, 1};

   int min_local[NB_SITE+1] = {-1, 4, 7, 1, 6, 2, 9};

   /* liste des voisins entrants */
   int voisins_in[NB_SITE+1][2] = {{-1, -1},
				{4, 5}, {1, -1}, {1, -1},
				{3, 5}, {6, -1}, {2, -1}};
                               
   /* liste des voisins sortants */
   int voisins_out[NB_SITE+1][2] = {{-1, -1},
				{2, 3}, {6, -1}, {4, -1},
				{1, -1}, {1, 4}, {5,-1}};

   for(i=1; i<=NB_SITE; i++){
      MPI_Send(&nb_voisins_in[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(&nb_voisins_out[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(voisins_in[i], nb_voisins_in[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(voisins_out[i], nb_voisins_out[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(&min_local[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD); 
   }
}

/******************************************************************************/
void calcul_min(int s) {
	MPI_Recv(&nbVoisinIn, 1, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	MPI_Recv(&nbVoisinOut, 1, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	MPI_Recv(&voisinIn, 2, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	MPI_Recv(&voisinOut, 2, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	MPI_Recv(&minLocal, 1, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	
	int i,envoyer,decision=0;
	
	if (rang==INITIATEUR) {
		printf("SITE %d INITIATEUR\n",rang);
		for (i=0;i<nbVoisinOut;i++) {
			MPI_Send(&minLocal, 1, MPI_INT, voisinOut[i], tag, MPI_COMM_WORLD); 
			printf("site%d envoie %d à site%d    ",rang,minLocal,voisinOut[i]);
		}printf("\n");fflush(stdout);
		cptS++;
	}else {
		MPI_Recv(&recu, 1, MPI_INT, MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);
		printf("site%d recoit %d de site%d\n",rang,recu,voisinIn[i]);
		fflush(stdout);
		cptR[i]++;
		if (recu<minLocal) minLocal=recu;
	}
	
	while (!decision) {
		for (i=0;i<nbVoisinIn;i++) {
			MPI_Iprobe(voisinIn[i], tag, MPI_COMM_WORLD, &flag, &status);
			if (flag) { // il y a msg dans le buff recept
				recu=-1;
				MPI_Recv(&recu, 1, MPI_INT, MPI_ANY_SOURCE,tag,MPI_COMM_WORLD,&status);
				printf("site%d recoit %d de site%d\n",rang,recu,voisinIn[i]);
				fflush(stdout);
				cptR[i]++;
				if (recu<minLocal) minLocal=recu;
				break;
			}
		}
		//envoyer
		envoyer=1;
		for (i=0;i<nbVoisinIn;i++) {
			if ((cptR[i]<cptS) || (cptS>=DIAMETRE)) {
				envoyer=0;
				break;
			}
		}
		if (envoyer) {
			for (i=0;i<nbVoisinOut;i++) {
				MPI_Send(&minLocal, 1, MPI_INT, voisinOut[i], tag, MPI_COMM_WORLD); 
				printf("site%d envoie %d à site%d    ",rang,minLocal,voisinOut[i]);
			}printf("\n");fflush(stdout);
			cptS++;
		}
		//decision
		decision=1;
		for (i=0;i<nbVoisinIn;i++) {
			if (cptR[i]<DIAMETRE) {
				//printf("%d %d\n",rang,cptR[i]);
				decision=0;
				break;
			}
		}
	}
	printf("SITE %d => min = %d\n",rang,minLocal);
	fflush(stdout);
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
