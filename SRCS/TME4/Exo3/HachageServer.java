package TME4.Exo3;

import java.util.HashMap;
import java.util.Map;

public class HachageServer extends HachagePOA {
	
	Map<String,org.omg.CORBA.Object> hash = new HashMap<String,org.omg.CORBA.Object>();
	
	@Override
	public boolean containsKey(String key) {
		return hash.containsKey(key);
	}

	@Override
	public org.omg.CORBA.Object get(String key) {
		return hash.get(key);
	}

	@Override
	public void put(String key, org.omg.CORBA.Object value) {
		hash.put(key, value);
	}


}
