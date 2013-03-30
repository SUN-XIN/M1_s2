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
public class GeneriqueXHTML {
    
    /* les attributs */
    
    String server = "oracle.ufr-info-p6.jussieu.fr";
    String port = "1521";
    String database = "ora10";
    String user = "E1234567";            // votre login
    String password = "E1234567";         // le mot de passe
    Connection connexion = null;
    PrintWriter ecrivain;
    
    static PrintStream out = System.out ;    // affichage des r�sulats � l'ecran
    
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
	GeneriqueXHTML c = new GeneriqueXHTML();
        
	/* requete */
	c.traiteRequete(requete);
    }
    
    /**
     * Constructeur : initialisation
     **/
    private GeneriqueXHTML(){
        try {

        	ecrivain =  new PrintWriter(new BufferedWriter (new FileWriter("resultat.html")));
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
           /******************/
           ecrivain.println("<?xml version='1.0' encoding='ISO-8859-1' ?>");
           ecrivain.println("<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1 - transitional.dtd'>");
           ecrivain.println("<html>");
           ecrivain.println("<head ><title>Résultat</title></head>");
           ecrivain.println("<body>");
           ecrivain.println("<h3>La requete est : </h3>"+requete);
           ecrivain.println("<h3>le resultat est : </h3>");
           ecrivain.println("<table border='2'>");
           /*****************/
           
           
           ResultSet resultat = lecture.executeQuery(requete);
           
           ResultSetMetaData rsm = resultat.getMetaData();
           int nbr=rsm.getColumnCount();
           String tuple;
           
           out.println("resultat...");
           ecrivain.println("<tr>");
           for (int i=1;i<=nbr;i++){
               tuple = rsm.getColumnName(i);
               ecrivain.println("<th>"+tuple+"</th>");
               out.print(tuple);}
           ecrivain.println("</tr>");
           out.print("\n");
           
           while (resultat.next()) {
        	   ecrivain.println("<tr>");
        	   for (int i=1;i<=nbr;i++){
        		   tuple = resultat.getString(i);
        		   out.print(tuple);
        		   ecrivain.println("<td>"+tuple+"</td>");
        		   }
        	   out.print("\n");
        	   ecrivain.println("</tr>");
           }
           
		    resultat.close();
		    lecture.close();
		    
		    connexion.close();
		    ecrivain.println("</table>");
		    ecrivain.println("</html>");
		    ecrivain.close();
        }

        /* getion des exceptions */
        catch(Exception e){ Outil.gestionDesErreurs(connexion, e);}
    }
}
