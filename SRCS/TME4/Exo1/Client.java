package TME4.Exo1;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import org.omg.CORBA.ORB;
import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContext;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.CosNaming.NamingContextHelper;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;

public class Client {
	
	static org.omg.CORBA.Object restoreIOR(String fileName, ORB orb) {
		  // ouverture du flux d’entrée
			String ior = null;
		  try {
			BufferedReader br = new BufferedReader(new FileReader(fileName));
			ior = br.readLine(); // récupère l’ior enregistrée dans le fichier
			br.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} // ferme le flux
		  return orb.string_to_object(ior); // convertit la chaîne en object CORBA
		}

	
	public static void main(String[] args) {

		try {
		    ORB orb = ORB.init(args, null);
		   
		    /** Récupération IOR et construction de la référence Corba **/
		    org.omg.CORBA.Object obj = restoreIOR("compte.ior", orb);

            Calcul cal = CalculHelper.narrow(obj);

		    System.out.println("add: " + cal.add(1.0, 2.0));
		    System.out.println("sub: " + cal.sub(5.0, 2.0));
		    System.out.println("mult: " + cal.mult(18.0, 2.0));
		    System.out.println("div: " + cal.div(1.0, 2.0));
		

		} catch(Exception e){
		    System.out.println("ERROR : " + e);
		    e.printStackTrace(System.out);
		}

	}

}

