#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAGINIT 0
#define TAGINIC 1 //msg calculer
#define NB_SITE 6

MPI_Status status;
int nb_proc,rang;
int flag=0;

	int nbVoisins=0;
	int minLocal=0;
	int mesVoisins[3];
	int i;
	int decideur=0;
	int last=-1;
	int recu[NB_SITE],envoyer[NB_SITE];
	typedef struct message {
		int val;
		int type; // 0 - calculer 1 - annonce 
	}msg;
	msg r,e;
	int taille=sizeof(msg);
	
void simulateur(void) {
   int i;

   /* nb_voisins[i] est le nombre de voisins du site i */
   int nb_voisins[NB_SITE+1] = {-1, 2, 3, 2, 1, 1, 1};
   int min_local[NB_SITE+1] = {-1, 3, 11, 8, 14, 5, 17};

   /* liste des voisins */
   int voisins[NB_SITE+1][3] = {{-1, -1, -1},
         {2, 3, -1}, {1, 4, 5}, 
         {1, 6, -1}, {2, -1, -1},
         {2, -1, -1}, {3, -1, -1}};
                               
   for(i=1; i<=NB_SITE; i++){
      MPI_Send(&nb_voisins[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD);    
      MPI_Send(voisins[i],nb_voisins[i], MPI_INT, i, TAGINIT, MPI_COMM_WORLD);
      MPI_Send(&min_local[i], 1, MPI_INT, i, TAGINIT, MPI_COMM_WORLD); 
   }
}

/******************************************************************************/
void calcul_min(int s){
	//recuper les val init
	MPI_Recv(&nbVoisins, 1, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	//printf("site %d : nbrVoisin %d\n",s,nbVoisins);
	MPI_Recv(mesVoisins, nbVoisins, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	MPI_Recv(&minLocal, 1, MPI_INT, 0,TAGINIT,MPI_COMM_WORLD,&status);
	//for (i=0;i<3;i++) printf("site %d : voisins %d\n",s,mesVoisins[i]);
	
	if (nbVoisins==1) {
		e.type=0;
		e.val=minLocal;
		MPI_Send(&e, taille, MPI_INT, mesVoisins[0], TAGINIC, MPI_COMM_WORLD);
		envoyer[mesVoisins[0]]=1;
		printf("feuille : site %d envoie %d vers site %d\n",rang,minLocal,mesVoisins[0]);
	}
	else {
		for (i=0;i<nbVoisins-1;i++) {
			MPI_Recv(&r, taille, MPI_INT, MPI_ANY_SOURCE,TAGINIC,MPI_COMM_WORLD,&status);
			last=status.MPI_SOURCE;
			recu[last]=1;
			//Calculer
			if (r.val<minLocal) minLocal=r.val;
			printf("calcul : site %d a recu %d depuis site %d\n",rang,r.val,status.MPI_SOURCE);
		}
		for (i=0;i<nbVoisins;i++) {
			if (!recu[mesVoisins[i]]) {
				e.type=0;
				e.val=minLocal;
				MPI_Send(&e, taille, MPI_INT, mesVoisins[i], TAGINIC, MPI_COMM_WORLD);
				envoyer[mesVoisins[i]]=1;
				printf("site %d envoie %d vers site %d\n",rang,minLocal,mesVoisins[i]);
				break;
			}	
		}
	}
	MPI_Recv(&r, taille, MPI_INT, MPI_ANY_SOURCE,TAGINIC,MPI_COMM_WORLD,&status);
	last=status.MPI_SOURCE;
	if (r.type)  { // type annonce
		printf("annonce : site %d a recu %d depuis site %d\n",rang,r.val,status.MPI_SOURCE);
		minLocal=r.val;
		for (i=0;i<nbVoisins;i++)
			if (last!=mesVoisins[i]) {
				e.type=1;
				e.val=minLocal;
				MPI_Send(&e, taille, MPI_INT, mesVoisins[i], TAGINIC, MPI_COMM_WORLD);
				envoyer[mesVoisins[i]]=1;
			}
	}
	else { // cas decideur
		decideur=1;
		//Calculer
		if (r.val<minLocal) minLocal=r.val;
		printf("DECIDEUR : site %d a recu %d depuis site %d\n",rang,r.val,status.MPI_SOURCE);
		for (i=0;i<nbVoisins;i++)
			if (last!=mesVoisins[i]) {
				e.type=1;
				e.val=minLocal;
				MPI_Send(&e, taille, MPI_INT, mesVoisins[i], TAGINIC, MPI_COMM_WORLD);
				envoyer[mesVoisins[i]]=1;
			}
	}
	printf("FIN de site %d : min = %d \n",rang,minLocal);
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
