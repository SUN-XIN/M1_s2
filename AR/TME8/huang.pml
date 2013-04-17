#define N 3           /* Number of processes */ 
#define MAX 10
#define ACTIF 1
#define PASSIF 0

mtype = {m,term,agr,ack};

chan channel_IN[N] = [MAX] of {mtype, byte, byte};  /*<type,sender,horloge>*/

inline Broadcast (type,h) {
   byte p=0;
   do
     :: p < N ->
        channel_IN[p]!type,id,h;
        p++
     :: p == N -> break 
   od;
}

inline desactivation() {
	etat=PASSIF;
	if 
	  :: (unack==0) ->
	     hLocal++;
	     hFin=hLocal;
	     last=id;
	fi;
	Broadcast (term,hLocal);
}

inline recevoir(type,hRecu,sender) {
	if
	  :: (type==m) ->
			channel_IN[sender]!ack,id,hLocal;
			if 
			  :: (etat==PASSIF) -> etat=ACTIF;
			fi;
	  :: (type==ack) ->
			unack--;
			if 
			  :: (hLocal<hRecu) -> hLocal=hRecu;
			fi;
			if
			  :: ((unack==0) && (etat==ACTIF)) ->
			     hLocal++;
			     hFin=hLocal;
			     last=id;
			     Broadcast (term,hLocal);
			fi;
	  :: (type==term) ->
			if
			  :: ((unack>0) || (etat==ACTIF)) ->
			     if 
			       :: (hLocal<hRecu) -> hLocal=hRecu;
			     fi;
			  :: else ->
			     if
			       :: ((hRecu>hFin) || ((hRecu==hFin) && (sender>last))) ->
			          hFin=hRecu;
			          last=sender;
			          if 
						:: (hLocal<hRecu) -> hLocal=hRecu;
					  fi;
					  channel_IN[sender]!ack,id,hLocal;
			     fi
			fi
	  :: (type==agr) ->
			if
			  :: (hLocal==hFin) -> nbragr++;
			fi;
			if
			  :: (nbragr==N-1) ->
			      fin=1;
			      printf("FIN\n");
			fi;
		
	fi;
}

proctype fonc(byte id) {
	byte etat=ACTIF;
	byte unack=0;
	byte hLocal=0;
	byte hFin=0;
	byte last=0;
	byte nbragr=0;
	bool fin=0;
	mtype type;
	byte hRecu,sender;
	chan cLocal = channel_IN[id];
	if
	  :: (id==0) -> unack++;
					channel_IN[1]!m,id,0;
	  :: (id==1) -> unack++;
					channel_IN[2]!m,id,0;
	fi;
	desactivation();
	do
	  :: (fin==0) ->
		 cLocal?type,sender,hRecu;
		 recevoir(type,hRecu,sender);
	     desactivation();
	  :: else -> break;
	od;
}

init{
   byte proc; 
   atomic {
      proc=0;
      do
         :: proc < N ->
            run fonc(proc); 
            proc++;
         :: proc == N -> break ;
      od;
   }
}

