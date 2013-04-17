#define N 6           /* Number of processes */ 
#define NB_V_MAX 3    /* maximum degree of a node */ 

mtype = {calcul};

chan channel_IN[N] = [N] of {mtype, byte, byte}; 
   /*  All messages to process i are received on channel_IN[i] 
       a message contains information <type, sender, value>   */
   
/**************************************************************/

inline Simulateur() {
   if
      :: (id == 0) ->
	     minloc = 3; nb_voisins = 2;
	     voisins[0] = 1; voisins[1] = 2;
      :: (id == 1) ->
	     minloc = 11; nb_voisins = 3;
	     voisins[0] = 0; voisins[1] = 3; voisins[2] = 4;
      :: (id == 2) ->
	     minloc = 8; nb_voisins = 2;
	     voisins[0] = 0; voisins[1] = 5;
      :: (id == 3) ->
	     minloc = 14; nb_voisins = 1;
	     voisins[0] = 1;
      :: (id == 4) ->
	     minloc = 5; nb_voisins = 1;
	     voisins[0] = 1;
      :: (id == 5) ->
	     minloc = 17; nb_voisins = 1;
	     voisins[0] = 2;
   fi;
   mincal = minloc	  
}

/**************************************************************/
inline Test_Emission () {
   /* determine si l'on peut emettre et identifie le destinataire q */
   //condition : 1.on a 1 seul voisin 		2.recu depuis tous mes voisins sauf 1
   if 
	  :: (nb_voisins==1) -> 
		 /* 1 seul voisin*/
	     channel_IN[voisins[0]]!calcul(id,minloc);
	     deja_emis=1;
	     wait=1;
	  :: else -> 
	     /* + de voisins */
	      nb_recus=0;
	     do
	       :: (nb_recus<(nb_voisins-1))->
	          /* attendre msg */
	          canal_IN?type,sender,mincal;
	          recu[sender]=1;
	          nb_recus++;
	          if 
	            :: (mincal<minloc) -> minloc=mincal;
	          fi;
	       :: nb_recus==(nb_voisins-1) -> break;
	     od;
		 byte v=0,dest=-1;
	     do  
	     /* trouver dest à envoyer */
	       :: (v<nb_voisins) ->
	          if
	            :: (recu[voisins[v]]==0) -> 
	               dest=voisins[v];
				   break;
	          fi;
	          v++;
	       :: else -> break;
	     od;
	     /* envoyer vers dest */
	     channel_IN[dest]!calcul(id,minloc);
	     deja_emis=1;
	     wait=1;
   fi;
}

inline Test_Decision () {
   /* determine si l'on peut decider */
   if  
     :: (recu[sender]==1) ->
        decision=1;
        printf("%d : decision\n", id);
   fi;
}

/**************************************************************/

proctype node(byte id) {
    
byte nb_voisins;
byte voisins[NB_V_MAX];

chan canal_IN = channel_IN[id];
xr canal_IN;            /* only id reads on this channel */

mtype type;
byte i, nb_recus;
byte sender, receiver, minloc, mincal;

/* tableau initialise a 0 ; recu[i] = 1 si on a recu de i */
byte recu[N];  
bool emission, wait = 0, decision, deja_emis = 0;

   Simulateur();

/* Each process executes a finite loop */
   do
      :: (deja_emis==0) ->
              /* test emission et traitement correspondant 
              - on ne doit emettre qu'une fois
              - si on ne peut pas emettre, on attend (wait = 1) que la 
                condition ait pu etre modifiee                          */
	     Test_Emission();
      :: (wait==1) ->
         /* test reception et traitement correspondant */
         canal_IN?type,sender,mincal;
		 Test_Decision();
      :: (decision==1) -> /* test terminaison */
		 break;
   od;
   printf("%d : le minimum est %d\n", id, minloc);
}

/**************************************************************/
/* Creates a network of 6 nodes 
   The structure of the network is given by array voisins */ 

init{
   byte proc; 
   
   atomic {
      proc=0;
      do
         :: proc <N ->
            run node(proc); 
            proc++
         :: proc == N -> break 
      od
   }
}
