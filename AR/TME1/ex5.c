#include "mpi_server.h"

static server the_server;
int fini;

void* boucle(void (*callback)(int tag, int source)){
	while (1) {
		pthread_mutex_lock(&(the_server.mutex));
		MPI_Status stat;
		int res;
		MPI_Iprobe(source, tag, MPI_COMM_WORLD, &res, stat);
		pthread_mutex_unlock(&(the_server.mutex));
		if (res!=0) {
			(*callback)(res.tag, res.source);
		}
		if (fini) break;
	}
}

void start_server(void (*callback)(int tag, int source)){
	fini=0;
	if (pthread_create(&(the_server.listener),NULL,boucle,(*callback)(tag, source))!=0) {
		printf("ERREUR:creation pthread\n");
		exit(1);
	}
	int stat;
	if (pthread_join(the_server.listener,(void **)&stat)!=0) {
		printf("ERREUR:joindre pthread\n");
		exit(1);
	}
}

void destroy_server(){
	fini++;
}

pthread_mutex_t* getMutex(){	
	pthread_mutex_t m=PTHREAD_MUTEX_INITIALIZER;
	return &m;
}

void (*callback)(int t,int src) {
	char msg[SIZE];
	MPI_Status stat;
	MPI_Recv(msg, SIZE, MPI_CHAR, src, t, MPI_COMM_WORLD, &stat);
	printf("PROC server => msg recu est : %s\n",msg);
}
