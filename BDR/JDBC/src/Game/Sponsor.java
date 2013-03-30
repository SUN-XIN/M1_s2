package Game;

import java.sql.*;
import java.io.*;


/**
 *  NOM, Prenom 1 :
 *  NOM, Prenom 2 :
 *  Binome        :
 *  Groupe        :
 *
 * La classe Sponsor
 **/
public class Sponsor {
    
    /* parametres de connexion */

    String url1 = "jdbc:oracle:thin:@oracle.ufr-info-p6.jussieu.fr:1521:ora10"; // base tennis
    String user1 = "E1234567";            // votre login
    String password1 = "E1234567";         // le mot de passe
    Connection connexionTennis = null;

    String url2 = "jdbc:oracle:thin:@oracle2.ufr-info-p6.jussieu.fr:1521:ora2"; // base des sponsors
    String user2 ="anonyme";             // acces anonyme
    String password2 ="anonyme";
    Connection connexionSponsor = null;

    PrintStream out = System.out;    // affichage des resulats a l'ecran
    
    /**
     * methode main: point d'entr�e
     **/
    public static void main(String a[]) {
        
        /* initialisation */
        Sponsor c = new Sponsor();
        
        /* test des m�thodes que vous avez �crites */
        c.traiteRequete();
    }
    
    /**
     * Constructeur : initialisation
     **/
    private Sponsor(){
        try {
            /* Chargement du pilote JDBC */
	    Class.forName("oracle.jdbc.driver.OracleDriver");
        }
        catch(Exception e) {
            Outil.erreurInit(e);
        }
    }
    
    
    /**
     *  La methode traiteRequete
     *  affiche le resultat d'une requete
     */
    public void traiteRequete() {
        try {
        	    
        	connexionTennis = DriverManager.getConnection(url1, user1, password1);
      	    out.println("statement...");
            Statement lecture1 =  connexionTennis.createStatement();   
            String requete1="select g.Sponsor, j.Nom, j.Nationalite,j.NuJoueur from GAIN2 g, JOUEUR2 j where g.NuJoueur=j.NuJoueur order by j.Nom";
            out.println("execute la requete : " + requete1);
            ResultSet resultat1 = lecture1.executeQuery(requete1);
            ResultSetMetaData rsm1 = resultat1.getMetaData();
            int nbr1=rsm1.getColumnCount();
            String tuple;
            
            connexionSponsor = DriverManager.getConnection(url2, user2, password2);
      	    out.println("statement...");
            Statement lecture2 =  connexionSponsor.createStatement();
           
            String requete2;
             
            out.println("resultat...");
            for (int i=1;i<=nbr1;i++){
                tuple = rsm1.getColumnName(i)+"\t\t";
                out.print(tuple);}
            out.print("\n");
            
            String nomJ,nationJ,nomS,nationS,n;
            
            while (resultat1.next()) {
            	
                tuple = resultat1.getString(1);
                requete2="select * from SPONSOR where Nom='"+tuple+"'";
                //out.println("execute la requete2 : " + requete2);
                ResultSet resultat2 = lecture2.executeQuery(requete2);
                ResultSetMetaData rsm2 = resultat2.getMetaData();
                while (resultat2.next()) {
	                nomJ=resultat1.getString(2);
	                nationJ=resultat1.getString(3);
	                nomS=resultat2.getString(1);
	                nationS=resultat2.getString(2);
	                n=resultat1.getString(4);
	                out.print("num J:"+n+" nom Joueur:"+nomJ+" nation Joueur:"+nationJ+" nom Sponsor:"+nomS+" nation Sponsor:"+nationS);
	                out.print("\n");
                }
            	}
   
            out.print("\n");
            
 		    resultat1.close();
 		    lecture1.close();
 		   connexionTennis.close();
 		   
 		   


	}
	/* getion des exceptions */
	catch(Exception e){ gestionDesErreurs(e);}
    }
    
    //---------------------------------------------------------------------
    
    /* m�thodes pour la gestion des erreurs */
    
    protected void gestionDesErreurs(Exception e) {
	out.println("Probleme d'acces a la base: " + e);
	
	/* pour facilier le d�bogage,
	   afficher la ligne ou l'erreur s'est produite*/
	e.printStackTrace();
	
	/* En cas de pb d'acces, on ferme la connexion */
	try {
	    if (connexionTennis != null)
		connexionTennis.close();
	    if (connexionSponsor != null)
		connexionSponsor.close();
	}
	catch(Exception se) {
	    out.println("Tout autre probleme: " + se);
	}
        throw new RuntimeException("Arret immediat");
    }
    
}
