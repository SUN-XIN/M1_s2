package TME4.Exo3;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContext;
import org.omg.CosNaming.NamingContextHelper;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;

import TME4.Exo1.CalculServer;

public class Server {

	public static void main(String[] args) {
		try {

			ORB orb = ORB.init(args, null); 

			POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));  
			rootpoa.the_POAManager().activate(); 

			HachageServer helloImpl = new HachageServer();

			org.omg.CORBA.Object ref = rootpoa.servant_to_reference(helloImpl);
			
			org.omg.CORBA.Object objRef = 
					orb.resolve_initial_references("NameService");
			NamingContext ncRef = NamingContextHelper.narrow(objRef);

			String name = "Hachage";
			NameComponent path[] = new NameComponent[] {new NameComponent(name, "")};
			ncRef.rebind(path, ref);
			
			
			orb.run();
		}

		catch(Exception e) {
			System.err.println("ERROR : " + e);
			e.printStackTrace(System.out);
		}

	}
}
