package TME4.Exo1;

import java.io.FileWriter;
import java.io.IOException;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContext;
import org.omg.CosNaming.NamingContextHelper;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;

public class Server {
	
	static void saveIOR(String fileName, ORB orb, org.omg.CORBA.Object obj) {
		  String ior = orb.object_to_string(obj); // représentation sous forme de string de l’ior
		  try {
			FileWriter fw = new FileWriter(fileName); // un flux de sortie vers un fichier
			fw.write(ior); // rempli avec l’ior
			fw.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} // et fermé
	}


	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {

			ORB orb = ORB.init(args, null); 

			POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));  
			rootpoa.the_POAManager().activate(); 

			CalculServer helloImpl = new CalculServer();  

			org.omg.CORBA.Object ref = rootpoa.servant_to_reference(helloImpl);
		
			saveIOR("compte.ior", orb, ref); /** sauvegarde l’ior de obj dans compte.ior **/
			orb.run();
		}

		catch(Exception e) {
			System.err.println("ERROR : " + e);
			e.printStackTrace(System.out);
		}

	}
}
