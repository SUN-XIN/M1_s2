package TME4.Exo1.CalculPackage;

/**
* TME4/Exo1/CalculPackage/NotZeroHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from calculatrice.idl
* mercredi 20 mars 2013 16 h 47 CET
*/

public final class NotZeroHolder implements org.omg.CORBA.portable.Streamable
{
  public TME4.Exo1.CalculPackage.NotZero value = null;

  public NotZeroHolder ()
  {
  }

  public NotZeroHolder (TME4.Exo1.CalculPackage.NotZero initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = TME4.Exo1.CalculPackage.NotZeroHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    TME4.Exo1.CalculPackage.NotZeroHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return TME4.Exo1.CalculPackage.NotZeroHelper.type ();
  }

}