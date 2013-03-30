package TME4.Exo3;

import org.omg.CORBA.ORB;



public class Client {

	public static void main(String[] args) {

		try {
		    ORB orb = ORB.init(args, null);
		    
		    String url = "corbaname::localhost:2048#Hachage";
            org.omg.CORBA.Object obj = orb.string_to_object(url);
            
            System.out.println("obj: " + obj);
            Hachage h = HachageHelper.narrow(obj);

		    h.put("abc", obj);
		    System.out.println("get " + h.get("abc"));
		    System.out.println("contains 'abc': " + h.containsKey("abc"));
		

		} catch(Exception e){
		    System.out.println("ERROR : " + e);
		    e.printStackTrace(System.out);
		}

	}

}
