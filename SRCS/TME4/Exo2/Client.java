package TME4.Exo2;

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

import TME4.Exo1.Calcul;
import TME4.Exo1.CalculHelper;

public class Client {
	
	
	public static void main(String[] args) {

		try {
		    ORB orb = ORB.init(args, null);
		    
		    String url = "corbaname::localhost:2048#Calculatrice";
            org.omg.CORBA.Object obj = orb.string_to_object(url);
            
		   
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