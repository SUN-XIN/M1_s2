package Game;

import java.sql.*;
import java.io.*;


/**
 *   NOM, Prenom 1 :BARRY, Mamadou Sita
 *   NOM, Prenom 2 :SUN , Xin
 *   Groupe    : 1
 *   Binome    : A11
 *
 * La classe MaxPrime
 **/
public class MaxPrime2 {

    String server = "oracle.ufr-info-p6.jussieu.fr";
    String port = "1521";
    String database = "ora10";
    String user = "E1234567";            // votre login
    String password = "E1234567";         // le mot de passe
    Connection connexion = null;
    public static PrintStream out = System.out;    // affichage des r�sulats � l'ecran

    /**
     * methode main: point d'entr�e
     **/
    public static void main(String a[]) {
    	MaxPrime2 c = new MaxPrime2();
	c.traiteRequete();
    }
 
   
    /**
     * Constructeur : initialisation
     **/
    private MaxPrime2(){
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
     *  
     */
    public void traiteRequete() {
        try {
	    String url = "jdbc:oracle:thin:@" + server + ":" + port + ":" + database;
	    out.println("Connexion avec l'URL: " + url);
	    out.println("utilisateur: " + user + ", mot de passe: " + password);
        
	    connexion = DriverManager.getConnection(url, user, password);
        String a1 = Outil.lireValeur("Saisir une annee");
            
        String requete = "select nom, max(prime) from Gain2 g, Joueur2 j"
        			+ " where g.nujoueur = j.nujoueur and annee = ? group by nom";

            PreparedStatement prep = connexion.prepareStatement(requete);

            prep.setInt(1, Integer.parseInt(a1));
            
           // Statement lecture =  connexion.createStatement();
            ResultSet resultat = prep.executeQuery();

            
        while (a1!=null) {
		    /* Commentaire: construir 1 requete SQL parametre a partir de l'entree standard*/
	            
		    /* Commentaire: on ouvre un canal de communication pour envoyer la requete. Puis on obtient le resultat et le parcourit pour l'affihchage */
		    out.println("statement...");	            
		    out.println("execute la requete : " + requete);
	        
		    out.println("resultat...");
		    
	            while (resultat.next()) {
	                String tuple = resultat.getString(1) + "\t" + resultat.getString(2);
	                out.println(tuple);
	            }
	           a1 = Outil.lireValeur("Saisir une annee");
	           prep.setInt(1, Integer.parseInt(a1));
	           
		   
        }
        resultat.close();
	    //lecture.close();
        prep.close();
	    connexion.close();
	    
        }

        /* getion des exceptions */
        catch(Exception e){ Outil.gestionDesErreurs(connexion, e);}
    }
}
