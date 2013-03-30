package Game;

import java.sql.*;
import java.io.*;



/**
 *  NOM, Prenom 1 :BARRY, Mamadou Sita
 *  NOM, Prenom 2 :SUN , Xin
 *
 * La classe Joueur
 **/
public class Joueur {
    
    /* Commentaire: remplir tous les attributs necessaires */
    
    String server = "oracle.ufr-info-p6.jussieu.fr";
    String port = "1521";
    String database = "ora10";
    String user = "1234567";
    String password = "1234567";
    String requete = "select nom, prenom from Joueur2";

    Connection connexion = null;
    public static PrintStream out = System.out;    // affichage des r�sulats � l'ecran
    
    /**
     * methode main: point d'entr�e
     **/
    public static void main(String a[]) {

        /* Commentaire: créer 1 nouveau objet typé par Joueur utilisant construiteur defaut */
        Joueur c = new Joueur();
        
	c.traiteRequete();
    }
    
    /**
     * Constructeur : initialisation
     **/
    protected Joueur(){
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
     *  Commentaire :
     *
     */
    public void traiteRequete() {
        try {

            /* Commentaire: connexion à la base de donnée database*/
	    String url = "jdbc:oracle:thin:@" + server + ":" + port + ":" + database;
            connexion = DriverManager.getConnection(url, user, password);
            
            /* Commentaire:Créee un object Statement pour envoyer des SQL statements vers la base de donnée */
            Statement lecture =  connexion.createStatement();
            
            /* Commentaire:execution de la requete */
            ResultSet resultat = lecture.executeQuery(requete);
            
            /* Commentaire:parcourir toute la table pour affichiffer le resultat */
            while (resultat.next()) {
                String tuple = resultat.getString(1) + "\t" + resultat.getString(2);
                out.println(tuple);
            }

            /* Commentaire: deconnexion de la base de donnée */
	    resultat.close();
	    lecture.close();
	    connexion.close();
        }

        /* Commentaire: Gerer les erreurs de connexion*/
        catch(Exception e){ Outil.gestionDesErreurs(connexion, e);}
    }
}
