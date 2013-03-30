package TME4.Exo3;


/**
* TME4/Exo3/HachagePOA.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from hashMap.idl
* mercredi 27 mars 2013 18 h 00 CET
*/

public abstract class HachagePOA extends org.omg.PortableServer.Servant
 implements TME4.Exo3.HachageOperations, org.omg.CORBA.portable.InvokeHandler
{

  // Constructors

  private static java.util.Hashtable _methods = new java.util.Hashtable ();
  static
  {
    _methods.put ("containsKey", new java.lang.Integer (0));
    _methods.put ("get", new java.lang.Integer (1));
    _methods.put ("put", new java.lang.Integer (2));
  }

  public org.omg.CORBA.portable.OutputStream _invoke (String $method,
                                org.omg.CORBA.portable.InputStream in,
                                org.omg.CORBA.portable.ResponseHandler $rh)
  {
    org.omg.CORBA.portable.OutputStream out = null;
    java.lang.Integer __method = (java.lang.Integer)_methods.get ($method);
    if (__method == null)
      throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);

    switch (__method.intValue ())
    {
       case 0:  // TME4/Exo3/Hachage/containsKey
       {
         String key = in.read_string ();
         boolean $result = false;
         $result = this.containsKey (key);
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 1:  // TME4/Exo3/Hachage/get
       {
         String key = in.read_string ();
         org.omg.CORBA.Object $result = null;
         $result = this.get (key);
         out = $rh.createReply();
         org.omg.CORBA.ObjectHelper.write (out, $result);
         break;
       }

       case 2:  // TME4/Exo3/Hachage/put
       {
         String key = in.read_string ();
         org.omg.CORBA.Object value = org.omg.CORBA.ObjectHelper.read (in);
         this.put (key, value);
         out = $rh.createReply();
         break;
       }

       default:
         throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
    }

    return out;
  } // _invoke

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:TME4/Exo3/Hachage:1.0"};

  public String[] _all_interfaces (org.omg.PortableServer.POA poa, byte[] objectId)
  {
    return (String[])__ids.clone ();
  }

  public Hachage _this() 
  {
    return HachageHelper.narrow(
    super._this_object());
  }

  public Hachage _this(org.omg.CORBA.ORB orb) 
  {
    return HachageHelper.narrow(
    super._this_object(orb));
  }


} // class HachagePOA