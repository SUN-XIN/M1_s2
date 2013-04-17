#define N      3    /* Number of processes */ 
#define L     10    /* Buffer size */
#define NIL (N+1)   /* for an undefined value */

#define MSG    5

mtype = {req, tk};

/* le processus i recoit ses messages dans canal[i] */ 
chan canal[N] = [L] of {mtype, byte};    /* pour un message de type tk, 
                                            on mettra la valeur associee a 0 */

byte SharedVar = 0;                      /* la variable partagee */ 

inline Initialisation ( ) {
   /* initialise les variables locales, sauf reqId, val et typ_mes */
   requesting=false;   
   if 
	:: id==0 -> token=true; 
				father=NIL;
	:: else -> 	token=false;
				father=0;
   fi;
   next=NIL;
 }

inline Request_CS() {
  /* operations effectuees lors d'une demande de SC */
  chan canal_env = canal[father];
  requesting=true;
  if
	::father!=NIL -> 	canal_env ! req(id);
						father=NIL;
  fi;
}

inline Release_CS() {
  /* operations effectuees en sortie de SC */
  chan canal_env = canal[next];
  requesting=false;
  if 
	::(next!=NIL)-> 	canal_env ! tk(id);
						token=false;
						next=NIL;
  fi;
}

inline Receive_Request_CS() {
  /* traitement de la reception d'une requete */
  chan canal_env = canal[father];
  if 
   ::father == NIL ->
	if
	 :: requesting==true -> next=val;
	 :: else -> val ! tk(id);
	fi;
	::else -> canal_env ! req(val);
   fi;
   father=val;
}

inline Receive_Token () {
  /* traitement de la reception du jeton */
  token=true;
}


proctype node(byte id; byte Initial_Token_Holder){
    
   bit requesting;    /* indique si le processus a demande la SC ou pas  */
   bit token;         /* indique si le processus possede le jeton ou non */

   byte father;       /* probable owner */
   byte next;         /* next node to whom send the token */
   byte val;          /* la valeur contenue dans le message */
   mtype typ_mes;     /* le type du message recu */
   byte reqId;        /* l'Id du demandeur, pour une requete */

   chan canal_rec = canal[id];     /* un alias pour mon canal de reception */
   xr canal_rec;                   /* je dois etre le seul a le lire */

   /* Chaque processus execute une boucle infinie */

   Initialisation();
   do
       :: ((token == true) && empty(canal_rec) && (requesting == true)) ->
	      /* on oblige le detenteur du jeton a consulter les messages recus */
	      in_SC :
				/* acces a la ressource critique (actions sur SharedVar), puis sortie de SC */
				SharedVar++;
				assert(SharedVar==1);
				SharedVar--;
				Release_CS();
       :: canal_rec ? typ_mes(val) ->
				/* traitement du message recu */ 
				Receive_Token();
       :: (requesting == false) -> /* demander la SC */
				Request_CS();
   od ;
}

/* Cree un ensemble de N processus */ 

init {
   byte proc; 
   atomic {
      proc=0;
      do
         :: proc < N ->
            run node(proc, 0); 
            proc++
         :: proc == N -> break 
      od
   }
}
