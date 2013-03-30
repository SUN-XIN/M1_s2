package Game;

import java.sql.*;
import java.io.*;


/**
 *   NOM, Prenom 1 :BARRY, Mamadou Sita
 *   NOM, Prenom 2 :SUN , Xin
 *   Groupe    : 1
 *   Binome    : A11
 *
 * La classe Generique
 **/
public class Schema {
    
    /* les attributs */
    
    String server = "oracle.ufr-info-p6.jussieu.fr";
    String port = "1521";
    String database = "ora10";
    String user = "E1234567";            // votre login
    String password = "E1234567";         // le mot de passe
    Connection connexion = null;
    
    static PrintStream out = System.out;    // affichage des r�sulats � l'ecran
    
    /**
     * methode main: point d'entr�e
     **/
    public static void main(String param[]) {

	if (param.length == 0) {
	    throw new RuntimeException("Pas de  requete, arret immediat");
	}


	/* requete */
	String requete = param[0];
	out.println("La requete est " + requete);

        /* initialisation */
        Schema c = new Schema();
        
	/* requete */
	c.traiteRequete(requete);
    }
    
    /**
     * Constructeur : initialisation
     **/
    private Schema(){
        try {

            /* Chargement du pilote JDBC */
	    /* driver Oracle */
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
    public void traiteRequete(String requete) {
        try {
        	String url = "jdbc:oracle:thin:@" + server + ":" + port + ":" + database;
     	    out.println("Connexion avec l'URL: " + url);
     	    out.println("utilisateur: " + user + ", mot de passe: " + password);
            
     	   connexion = DriverManager.getConnection(url, user, password);
     	  DatabaseMetaData meta = connexion.getMetaData();
          ResultSet res = meta.getColumns(null, null, requete, null);
          System.out.println("List of columns: "); 
          while (res.next()) {
             System.out.println( "  "+res.getString("COLUMN_NAME")+ "\t"+res.getString("TYPE_NAME")); 
             res.next();
          }
          res.close();

          connexion.close();
           
        }

        /* getion des exceptions */
        catch(Exception e){ Outil.gestionDesErreurs(connexion, e);}
    }
}
