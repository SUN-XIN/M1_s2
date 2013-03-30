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
public class Generique2 {
    
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
        Generique2 c = new Generique2();
        
	/* requete */
	c.traiteRequete(requete);
    }
    
    /**
     * Constructeur : initialisation
     **/
    private Generique2(){
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
     	    
     	   out.println("statement...");
           Statement lecture =  connexion.createStatement();
           
           out.println("execute la requete : " + requete);
           ResultSet resultat = lecture.executeQuery(requete);
           
           ResultSetMetaData rsm = resultat.getMetaData();
           int nbr=rsm.getColumnCount();
           String tuple;
           
           out.println("resultat...");
           for (int i=1;i<=nbr;i++){
               tuple = rsm.getColumnName(i) + "\t\t";
               out.print(tuple);}
           out.print("\n");
           
           while (resultat.next()) {
        	   for (int i=1;i<=nbr;i++){
               tuple = resultat.getString(i) + "\t\t";
               out.print(tuple);}
        	   out.print("\n");
           }
           
		    resultat.close();
		    lecture.close();
		    connexion.close();

        }

        /* getion des exceptions */
        catch(Exception e){ Outil.gestionDesErreurs(connexion, e);}
    }
}
