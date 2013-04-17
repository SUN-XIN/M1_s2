#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAGINIT 0
#define NB_SITE 6

#define DIAMETRE 5

#define MAX_VOISINS 2
#define min(a,b) ((a > b) ? b : a)

void calcul_min(int rank);

void __phase_on_receive(int rank);

int __phase_send(int rank);

void __init_phase();

int nb_in, nb_out;
int min_local, final_min;
int* voisins_in;
int* voisins_out;
int scpt;
int* rcpt;

void simulateur(void) {
   int i;

   /* nb_voisins_in[i] est le nombre de voisins entrants du site i */
   /* nb_voisins_out[i] est le nombre de voisins sortants du site i */
   int nb_voisins_in[NB_SITE+1] = {-1, 2, 1, 1, 2, 1, 1};
   int nb_voisins_out[NB_SITE+1] = {-1, 2, 1, 1, 1, 2, 1};

   int min_local[NB_SITE+1] = {-1, 4, 7, 1, 6, 2, 9};

   /* liste des voisins entrants */
   int voisins_in[NB_SITE+1][MAX_VOISINS] = {{-1, -1},
{4, 5}, {1, -1}, {1, -1},
{3, 5}, {6, -1}, {2, -1}};
                               
   /* liste des voisins sortants */
   int voisins_out[NB_SITE+1][MAX_VOISINS] = {{-1, -1},
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

void calcul_min(int rank) {
  __init_phase();
  int send;
  send = 0;
  if(rank == 1) {
    // Noeud initiateur
    send = __phase_send(rank);
  }
  while(send != -1) {
    __phase_on_receive(rank);
    send = __phase_send(rank);
  }
  printf("[%d] Le minimum calculé est %d\n", rank, final_min);
}

void __phase_on_receive(int rank) {
    MPI_Status status;
    int i;
    int value;
    MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, TAGINIT, MPI_COMM_WORLD, &status);
    for(i = 0 ; i < nb_in ; i++) {
      if(voisins_in[i] == status.MPI_SOURCE) {
        rcpt[i]++;
        printf("[%d] Réception de %d par %d. J'ai reçu %d message(s) de %d.\n", rank, value, status.MPI_SOURCE, rcpt[i], status.MPI_SOURCE);
        final_min = min(final_min, value);
        return;
      }
    }
}

int __phase_send(int rank) {
  int i;
  int send;
  send = 0;
  // On a envoyé au moins D messages, on ne pourra plus jamais rien envoyé
  if(scpt >= DIAMETRE) {
    return -1;
  }
  for(i = 0 ; i < nb_out ; i++) {
    if(rcpt[i] >= scpt) {
      // On envoie notre minimum
      printf("[%d] Envoi de %d à %d. J'ai envoyé %d message(s).\n", rank, final_min, voisins_out[i], scpt);
      MPI_Send(&final_min, 1, MPI_INT, voisins_out[i], TAGINIT, MPI_COMM_WORLD);
      ++send;
    }
  }
  scpt++;
  return send;
}

void __init_phase() {
  MPI_Status status;
  MPI_Recv(&nb_in, 1, MPI_INT, MPI_ANY_SOURCE, TAGINIT, MPI_COMM_WORLD, &status);
  MPI_Recv(&nb_out, 1, MPI_INT, MPI_ANY_SOURCE, TAGINIT, MPI_COMM_WORLD, &status);
  voisins_in = malloc(sizeof(int) * MAX_VOISINS);
  voisins_out = malloc(sizeof(int) * MAX_VOISINS);
  MPI_Recv(voisins_in, sizeof(voisins_in), MPI_INT, MPI_ANY_SOURCE, TAGINIT, MPI_COMM_WORLD, &status);
  MPI_Recv(voisins_out, sizeof(voisins_out), MPI_INT, MPI_ANY_SOURCE, TAGINIT, MPI_COMM_WORLD, &status);
  MPI_Recv(&final_min, 1, MPI_INT, MPI_ANY_SOURCE, TAGINIT,MPI_COMM_WORLD, &status);
  scpt = 0;
  rcpt = malloc(sizeof(int) * MAX_VOISINS);
  memset(rcpt, 0, (sizeof(int) * MAX_VOISINS));
}



/******************************************************************************/

int main (int argc, char* argv[]) {
   int nb_proc,rang;
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
