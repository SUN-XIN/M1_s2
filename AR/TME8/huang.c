#define N 3           /* Number of processes */ 
#define ACTIF 1
#define PASSIF 0

mtype = {m,term,agr,ack};

chan channel_IN[N] = [1] of {mtype, byte, byte};  /*<type,sender,horloge>*/

inline broadcast(mtype t,byte h,byte j) {
   byte p=0;
   do
     :: p < N ->
        channel_IN[p]!t(j,h);
        p++
     :: proc == N -> break 
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
	broadcast(term,hLocal,id);
}

inline recevoir(mtype t,byte h,byte j,byte i) {
	if
	  :: (t==m) ->
			channel_IN[j]!ack(id,hLocal);
			if 
			  :: (etat==PASSIF) -> etat=ACTIF;
			fi;
	  :: (t==ack) ->
			unack--;
			if 
			  :: (hLocal<h) -> hLocal=h;
			fi;
			if
			  :: ((unack==0) && (etat==ACTIF)) ->
			     hLocal++;
			     hFIn=hLocal;
			     last=i
			     broadcast(term,hLocal,id);
			fi;
	  :: (t==term) ->
			if
			  :: ((unack>0) || (etat==ACTIF)) ->
			     if 
			       :: (hLocal<h) -> hLocal=h;
			     fi;
			  :: else ->
			     if
			       :: ((h>hFin) || ((h==hFin) && (j>last))) ->
			          hFIn=h;
			          last=j;
			          if 
						:: (hLocal<h) -> hLocal=h;
					  fi;
					  channel_IN[j]!ack(id,hLocal);
			     fi
			fi
	  :: (t==agr) ->
			if
			  :: (hLocal==hFin) nbragr++;
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
	boolean fin=0;
	mtype type;
	byte hRecu,sender;
	chan cLocal = channel_IN[id];
	if
	  :: (id==0) -> unack++;
					channel_IN[1]!m(id,0);
	  :: (id==1) -> unack++;
					channel_IN[2]!m(id,0);
	fi;
	desactivation();
	do
	  :: (fin==0) ->
		 cLocal?type(sender,hRecu);
		 recevoir(type hRecu,sender,id);
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
            proc++
         :: proc == N -> break 
      od;
   }
}

